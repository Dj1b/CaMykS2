<?php
/**
 * @brief CaMykS errors handler library.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ErrorHandler class.
 */
final class ErrorHandler extends CaMykSLibrary {
    /**
     * @var array $params
     * @brief ErrorHandler parameters.
     */
    protected $params = array('output'    => 'auto');

    /**
     * @var array $errors
     * @brief ErrorHandler saved steps.
     */
    private $errors = array();

    /**
     * Object constructor.
     * @param array $params
     * @parblock - Available parameters are <br />
     * \e string output - \b none / \b shared / \b session / \b logs / \b html  <br />
     * @endparblock
     * @return void
     */
    public function __construct($params=array()) {
        /* Merge parameters in parent object */
        parent::__construct($params);

        /* Define error reporting level following configuration */
        error_reporting($this->get_errorReportingLevel());

        /* Override PHP error handler */
        set_error_handler(array($this, 'handle_PHPError'));

        /* Override PHP exception handler */
        set_exception_handler(array($this, 'handle_PHPException'));
    }

    /* Error handling methods */

    /**
     * Handle PHP error.
     * @param integer $code
     * @param string $message
     * @param string $file
     * @param integer $line
     * @return void
     */
    public function handle_PHPError($code, $message, $file, $line) {
        /* Define type from error code */
        $type = $this->get_typeFromCode($code);

        /* Create error */
        $error = new Error('PHP', $type, $code, $file, $line, $message);

        /* Save new error */
        $this->errors[] = $error;

        /* Check for fatal errors */
        if (in_array($code, array(E_ERROR, E_PARSE, E_CORE_ERROR, E_COMPILE_ERROR, E_USER_ERROR, E_STRICT)))
            $this->throw_fatalError($error);
    }

    /**
     * Handle PHP exception.
     * @param object $exception
     * @return void
     */
    public function handle_PHPException($exception) {
        /* Create error */
        $error = new Error(
            'PHP',
            'Exception',
            $exception->getCode(),
            $exception->getFile(),
            $exception->getLine(),
            $exception->getMessage()
        );

        /* Save new error */
        $this->errors[] = $error;
    }

    /**
     * Handle error in CaMykS.
     * @param string $origin
     * @param integer $code
     * @param string $message
     * @param string $details
     * @param string $file
     * @param integer $line
     * @param boolean $isFatal
     * @return void
     */
    public function handle_CaMykSError($origin, $code, $message, $details, $file, $line, $isFatal) {
        /* Create error */
        $error = new Error('CaMykS '.$origin, 'Error', $code, $file, $line, $message, $details);

        /* Save new error */
        $this->errors[] = $error;

        /* Check for fatal errors */
        if ($isFatal)
            $this->throw_fatalError($error);
    }

    /**
     * Handle Database error in CaMykS.
     * @param integer $code
     * @param string $message
     * @param string $details
     * @param string $file
     * @param integer $line
     * @return void
     */
    public function handle_CaMykSDatabaseError($code, $message, $details, $file, $line) {
        /* Save new error */
        $this->errors[] = new Error('CaMykS', 'Database Error', $code, $file, $line, $message, $details);
    }

    /* Exportation method */

    /**
     * Export results.
     * @param string $output
     * @return mixed
     */
    public function export($output='') {
        global $CaMykS;

        /* Check output method */
        $method = $output != '' ? $output : $this->params['output'];

        /* Export case of method */
        switch ($method) {
            case 'shared': return $this->export_toSharedData();
            case 'session': return $this->export_toSession();
            case 'logs': return $this->export_toCaMykSLogs();
            case 'html': return $this->export_toHTML();
            case 'none': return;
            case 'auto': default:
                if ($CaMykS->get_configValue('Website_Status') == 1)
                    return $this->export('logs');
                else
                    return $this->export('html');
        }
        return;
    }

    /* Private error methods */

    /**
     * Return error reporting level from engine and website configuration.
     * @return integer
     * @todo
     */
    private function get_errorReportingLevel() {
        return E_ALL;
    }

    /**
     * Return error type from PHP error code.
     * @param integer $code
     * @return string
     */
    private function get_typeFromCode($code) {
        /* Return value from known errors */
        switch ($code) {
            case E_ERROR: // 1
                return 'Error';
            case E_WARNING: // 2
                return 'Warning';
            case E_PARSE: // 4
                return 'Parse';
            case E_NOTICE: // 8
                return 'Notice';
            case E_CORE_ERROR: // 16
                return 'Core-Error';
            case E_CORE_WARNING: // 32
                return 'Core Warning';
            case E_COMPILE_ERROR: // 64
                return 'Compile Error';
            case E_COMPILE_WARNING: // 128
                return 'Compile Warning';
            case E_USER_ERROR: // 256
                return 'User Error';
            case E_USER_WARNING: // 512
                return 'User Warning';
            case E_USER_NOTICE: // 1024
                return 'User Notice';
            case E_STRICT: // 2048
                return 'Strict';
            case E_RECOVERABLE_ERROR: // 4096
                return 'Recoverable Error';
            case E_DEPRECATED: // 8192
                return 'Deprecated';
            case E_USER_DEPRECATED: // 16384
                return 'User Deprecated';
        }

        /* Default value */
        return 'Unknow Error';
    }

    /**
     * Throw a fatal error through CaMykS.
     * @param object $error
     * @return void
     */
    private function throw_fatalError($error) {
        global $CaMykS;

        /* Log fatal error */
        $CaMykS->log_error($error);

        /* Select layout to display */
        if ($CaMykS->get_configValue('Website_CaMykSMode') == 1)
            $layout = 'Error/Simplified';
        else
            $layout = 'Error/Complete';

        /* Display fatal error */
        $CaMykS->display_splashMessage($layout, $error, 500);
    }

    /* Private exporting methods */

    /**
     * Export results to CaMykS shared data.
     * @return void
     * @todo
     */
    private function export_toSharedData() {
        global $CaMykS;

        /* Save errors to shared data  */
        $CaMykS->set_sharedData('Errors', $this->errors);
    }

    /**
     * Export results to session.
     * @return void
     * @todo
     */
    private function export_toSession() {
        global $CaMykS;

        /* Save errors to session */
        $CaMykS->set_sessionValue('Errors', $this->errors);
    }

    /**
     * Export results to CaMykS logs.
     * @return void
     * @todo
     */
    private function export_toCaMykSLogs() {
        global $CaMykS;

        $messages = array();
        foreach ($this->errors as $error) {
            $message = array();
            if ($error->has_location())
                $message[] = $error->get_location();
            if ($error->has_message())
                $message[] = $error->get_message();
            $message = implode(' - ', $message);

            $messages[] = array(
                'action' => $error->get_header(),
                'message' => $message,
            );
        }

        /* Log errors to CaMykS logs */
        $CaMykS->log_messages('Errors', $messages);
    }

    /**
     * Export results to HTML.
     * @return void
     */
    private function export_toHTML() {
        /* display results as HTML */
        echo '<div style="display:none">'."\n";
        foreach ($this->errors as $error) {
            $error->export_asHTML()."<br />\n";
        }
        echo '</div>';
    }
}
?>