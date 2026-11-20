<?php
/**
 * @brief SystemLogManager admin module.
 * <br />Module to manage logs and system error reports.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Jan 2021
 * @date Modification: Nov 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemLogManager;

use CaMykS2\AdminModule;
use CaMykS2\Error;
use CaMykS2\DatabaseQueryWhereCondition;

/**
 * SystemLogManager class.
 */
final class SystemLogManager extends AdminModule {

    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected bool $hasConfiguration = true;

    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected array $libraries = array('Report', 'Configuration');

    /**
     * @var boolean $isLoggingError
     * @brief Re-entrancy guard to avoid infinite recursion when logging database errors.
     */
    private bool $isLoggingError = false;

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Error logging and Report methods */

    /**
     * Log system error object.
     * @param Error $error
     * @return boolean success
     */
    public function log_error(Error $error): bool {
        return $this->create_report($error);
    }

    /**
     * Create or update deduplicated error report in database.
     * @param Error $error
     * @return boolean success
     */
    public function create_report(Error $error): bool {
        /* Prevent recursive error logging */
        if ($this->isLoggingError)
            return false;

        $this->isLoggingError = true;

        try {
            /* Get Report library instance */
            $report = $this->get_libraryInstance('Report');
            if ($report === false or !$report->is_installed())
                return false;

            /* Build search parameters for unread report (Status = 0) with identical error characteristics */
            $whereConditions = array(
                new DatabaseQueryWhereCondition('Status', 0),
                new DatabaseQueryWhereCondition('Origin', $error->origin),
                new DatabaseQueryWhereCondition('Type', $error->type),
                new DatabaseQueryWhereCondition('Code', $error->code),
                new DatabaseQueryWhereCondition('File', $error->file),
                new DatabaseQueryWhereCondition('Line', $error->line),
                new DatabaseQueryWhereCondition('Message', $error->message),
            );

            /* Query existing unread report */
            $existingReports = $report->get_list(array('WhereConditions' => $whereConditions, 'Limit' => 1));

            if (!empty($existingReports)) {
                /* Update existing report: increment counter and update last date */
                $existing = reset($existingReports);
                $report->objectId = (int)$existing['id'];
                if (!$report->load())
                    return false;

                $errorCount = isset($error->count) ? (int)$error->count : 1;
                $report->Counter = (int)$report->Counter + $errorCount;
                $report->LastDate = time();
                return $report->save();
            }

            /* Create new unread report */
            $report->initialise();
            $report->Origin = $error->origin;
            $report->Type = $error->type;
            $report->Code = (int)$error->code;
            $report->File = $error->file;
            $report->Line = (int)$error->line;
            $report->Message = $error->message;
            $report->Details = $error->details;
            $report->Level = isset($error->level) ? (int)$error->level : 3;
            $report->Counter = isset($error->count) ? (int)$error->count : 1;
            $report->CreationDate = time();
            $report->LastDate = time();
            $report->Status = 0;

            return $report->save();
        } finally {
            $this->isLoggingError = false;
        }
    }

    /* Mark: Message logging methods */

    /**
     * Log single message to file.
     * @param string $object
     * @param string $action
     * @param string $message
     * @param mixed $level
     * @return boolean success
     */
    public function log_message(string $object, string $action, string $message='', mixed $level=null): bool {
        global $CaMykS;

        /* Check CaMykS configuration setting for error/message logging level */
        $loggingLevel = $CaMykS->get_configValue('Engine_ErrorLoggingLevel', null, 1);
        if ($loggingLevel === 0)
            return true;

        /* Write log entry to file */
        return $this->write_fileLog($object, $action, $message, $level);
    }

    /**
     * Log multiple messages to file.
     * @param string $object
     * @param array $messages
     * @param mixed $level
     * @return boolean success
     */
    public function log_messages(string $object, array $messages=array(), mixed $level=null): bool {
        $result = true;
        foreach ($messages as $action => $message) {
            if (is_array($message)) {
                $action = $message['Action'] ?? $message['action'] ?? (is_numeric($action) ? 'Log' : (string)$action);
                $message = $message['Message'] ?? $message['message'] ?? '';
            } elseif (is_numeric($action)) {
                $action = 'Log';
            }
            if (!$this->log_message($object, (string)$action, (string)$message, $level))
                $result = false;
        }
        return $result;
    }

    /* Mark: File log and rotation methods */

    /**
     * Write log entry into active File0.log and handle rotation.
     * @param string $object
     * @param string $action
     * @param string $message
     * @param mixed $level
     * @return boolean success
     */
    protected function write_fileLog(string $object, string $action, string $message, mixed $level=null): bool {
        /* Sanitize object folder name */
        $object = preg_replace('/[^a-zA-Z0-9_\-]/', '', $object);
        if ($object === '')
            $object = 'System';

        /* Ensure log directory exists */
        if (!$this->create_dataFilePath('Logs/'.$object))
            return false;

        /* Retrieve path for active log file File0.log */
        $activeFilePath = $this->get_dataFilePath('Logs/'.$object.'/File0.log');

        /* Read configuration parameters for rotation */
        $maxRotationSizeMo = (int)$this->get_configValue('File_MaxRotationSize', null, 5);
        $maxKeptFiles = (int)$this->get_configValue('File_MaxKeptFiles', null, 10);
        $maxSizeBytes = $maxRotationSizeMo * 1024 * 1024;

        /* Check size and perform rotation if needed */
        if (file_exists($activeFilePath) && filesize($activeFilePath) >= $maxSizeBytes) {
            $this->rotate_logFiles($object, $maxKeptFiles);
        }

        /* Format log entry */
        $dateStr = date('Y-m-d H:i:s');
        $levelStr = is_null($level) ? 'INFO' : strtoupper((string)$level);
        $logLine = '['.$dateStr.'] ['.$levelStr.'] ['.$action.'] '.$message;

        /* Append to File0.log */
        return (file_put_contents($activeFilePath, $logLine."\n", FILE_APPEND | LOCK_EX) !== false);
    }

    /**
     * Rotate index log files: File0 -> File1 -> File2 ... FileN.
     * @param string $object
     * @param integer $maxKeptFiles
     * @return boolean success
     */
    public function rotate_logFiles(string $object, int $maxKeptFiles=10): bool {
        /* Sanitize object name */
        $object = preg_replace('/[^a-zA-Z0-9_\-]/', '', $object);

        /* Remove oldest file if maximum kept files limit is reached */
        $oldestFilePath = $this->get_dataFilePath('Logs/'.$object.'/File'.$maxKeptFiles.'.log');
        if (file_exists($oldestFilePath)) {
            @unlink($oldestFilePath);
        }

        /* Shift intermediate archive files File{N-1}.log -> File{N}.log */
        for ($i = $maxKeptFiles - 1; $i >= 1; $i--) {
            $currentPath = $this->get_dataFilePath('Logs/'.$object.'/File'.$i.'.log');
            $nextPath = $this->get_dataFilePath('Logs/'.$object.'/File'.($i + 1).'.log');
            if (file_exists($currentPath)) {
                @rename($currentPath, $nextPath);
            }
        }

        /* Rename active File0.log to File1.log */
        $activeFilePath = $this->get_dataFilePath('Logs/'.$object.'/File0.log');
        $file1Path = $this->get_dataFilePath('Logs/'.$object.'/File1.log');
        if (file_exists($activeFilePath)) {
            @rename($activeFilePath, $file1Path);
        }

        return true;
    }
}
?>
