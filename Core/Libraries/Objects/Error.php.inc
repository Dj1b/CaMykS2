<?php
/**
 * @brief CaMykS error library, initiated by ErrorHandler.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre6
 * @date Creation: Nov 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Error class.
 */
final class Error {
    /**
     * @var string $origin
     * @brief Error origin.
     */
    private string $origin;

    /**
     * @var string $type
     * @brief Error type.
     */
    private string $type;

    /**
     * @var integer $code
     * @brief Error code.
     */
    private int $code;

    /**
     * @var string $file
     * @brief File where error was initiated.
     */
    private string $file = '';

    /**
     * @var integer $line
     * @brief Line where error was initiated.
     */
    private int $line = 0;

    /**
     * @var string $message
     * @brief Message associated with error.
     */
    private string $message;

    /**
     * @var string $details
     * @brief Details attached to message.
     */
    private string $details;

    /**
     * @var integer $count
     * @brief Count error occurences.
     */
    private int $count = 1;

    /**
     * @var array $backtrace
     * @brief Stored generated backtrace.
     */
    private array $backtrace;

    /**
     * @var integer $level
     * @brief Error level (default 3: Error).
     */
    private int $level = 3;

    /**
     * Object constructor.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param integer $line
     * @param string $message
     * @param string $details
     * @param array $backtrace
     * @param integer $level
     * @return void
     */
    public function __construct(string $origin, string $type, int $code=0, string $file='', int $line=0, string $message='', string $details='', array $backtrace=array(), int $level=3) {
        $this->origin = $origin;
        $this->type = $type;
        $this->code = $code;
        $this->file = $file;
        $this->line = $line;
        $this->message = trim($message);
        $this->details = trim($details);
        $this->backtrace = $backtrace;
        $this->level = $level;
    }

    /* Mark: Magic methods */

    /**
     * Read property value as variable.
     * @param string $name
     * @return mixed
     */
    public function __get(string $name): mixed {
        if (isset($this->$name))
            return $this->$name;
        return null;
    }

    /* Mark: Data related methods */

    /**
     * Return error header value.
     * @return string
     */
    public function get_header(): string {
        $s = '';

        /* Add origin */
        if ($this->origin != '')
            $s .= $this->origin.' ';

        /* Add error */
        if ($this->type != '') {
            $s .= $this->type;
            if ($this->code > 0)
                $s .= ' ['.$this->code.']';
        }

        /* Add count */
        if ($this->count > 1)
            $s .= ' x'.$this->count;

        /* Add file and line */
        if ($this->file != '') {
            $s .= ' in '.$this->file;
            if ($this->line > 0)
                $s .= ' (Line '.$this->line.')';
        }

        /* Return result */
        return $s;
    }

    /**
     * Check error has location values.
     * @return boolean result
     */
    public function has_location(): bool {
        return ($this->file != '');
    }

    /**
     * Return error location value.
     * @return string
     */
    public function get_location(): string {
        if ($this->file === '')
            return '';
        $s = $this->file;
        if (!($this->line > 0))
            return $s;
        $s .= ' (L'.$this->line.')';
        return $s;
    }

    /**
     * Check error has a message.
     * @return boolean result
     */
    public function has_message(): bool {
        return ($this->message != '');
    }

    /**
     * Return error message value.
     * @param boolean $HTMLFormat
     * @return string
     */
    public function get_message(bool $HTMLFormat=false): string {
        /* Build full message with details */
        $message = $this->message;
        if ($this->details !== '')
            $message .= ' ('.$this->details.')';

        /* Check format */
        if ($HTMLFormat)
            return nl2br($message);
        return $message;
    }

    /**
     * Check error has a backtrace.
     * @return boolean result
     */
    public function has_backtrace(): bool {
        return (count($this->backtrace) > 0);
    }

    /**
     * Return error trace value.
     * @param boolean $HTMLFormat
     * @return string
     */
    public function get_backtrace(bool $HTMLFormat=false): string {

        $lines = array();

        $cnt = 0;
        foreach($this->backtrace as $trace) {
            $m = ++$cnt.'/ ';
            if (isset($trace['file'])) {
                $m .= $trace['file'].' ';
                if (isset($trace['line']))
                    $m .= '(Line '.$trace['line'].') ';
                $m .= '- ';
            }

            if (isset($trace['class']))
                $m .= $trace['class'].' '.$trace['type'];

            if (isset($trace['function']))
                $m .= $trace['function'];

            if (isset($trace['args']) and is_string($trace['args']))
                $m .= '('.implode(', ', $trace['args']).')';
            elseif (isset($trace['args']) and is_object($trace['args']))
                $m .= '('.implode(', ', get_class($trace['args'])).')';

            $lines[] = $m;
        }

        if ($HTMLFormat)
            return '<br />&nbsp;&nbsp;&nbsp;&nbsp;'.implode('<br />&nbsp;&nbsp;&nbsp;&nbsp;', $lines);
        return "\n\t".implode("\n\t", $lines);
    }

    /**
     * Update error count.
     * @return void
     */
    public function update_count(): void {
        $this->count ++;
    }

    /* Mark: Check methods */

    /**
     * Check error is similar.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param integer $line
     * @param string $message
     * @return boolean success
     */
    public function is_similar(string $origin, string $type, int $code=0, string $file='', int $line=0, string $message=''): bool {
        if ($this->origin != $origin)
            return false;

        if ($this->type != $type)
            return false;

        if ($this->code != $code)
            return false;

        if ($this->file != $file)
            return false;

        if ($this->line != $line)
            return false;

        if ($this->message != trim($message))
            return false;

        /* All test failed, return error is similar */
        return true;
    }

    /* Mark: Export methods */

    /**
     * Export error as HTML.
     * @return string
     */
    public function export_asHTML(): string {
        $s = '<strong style="color:inherit;">'.$this->get_header();
        if ($this->has_message())
            $s .= '<br /> Error: '.$this->get_message(true);
        $s .= '</strong>';
        if ($this->has_backtrace())
            $s .= $this->get_backtrace(true);

        return $s;
    }

    /**
     * Export error as array.
     * @return array
     */
    public function export_asArray(): array {
        return get_object_vars($this);
    }
}
?>
