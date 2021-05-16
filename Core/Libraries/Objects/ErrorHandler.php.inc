<?php
/**
 * @brief CaMykS errors handler library.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre10
 * @date Creation: Nov 2019
 * @date Modification: May 2021
 * @copyright 2019 - 2021 CaMykS Team
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
    protected $params = array('output'  => 'auto');

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
        $this->stack_error('PHP', $type, $code, $file, $line, $message);

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
        /* Save error */
        $this->stack_error('PHP', 'Exception', $exception->getCode(), $exception->getFile(), $exception->getLine(), $exception->getMessage(), '', $exception->getTrace());
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
        /* Save error */
        $this->stack_error('CaMykS '.$origin, 'Error', $code, $file, $line, $message, $details);

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
     * @param int $line
     * @return void
     */
    public function handle_CaMykSDatabaseError($code, $message, $details, $file, $line) {
        /* Save error */
        $this->stack_error('CaMykS', 'Database Error', $code, $file, $line, $message, $details);
    }

    /* Management methods */

    /**
     * Stack error in error list.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param int $line
     * @param string $message
     * @param string $details
     * @param array $backtrace
     * @return void
     */
    private function stack_error($origin, $type, $code, $file, $line, $message, $details='', $backtrace=null) {
        /* Check error exists */
        foreach ($this->errors as $error) {
            if ($error->is_similar($origin, $type, $code, $file, $line, $message)) {
                $error->update_count();
                return;
            }
        }

        /* Store backtrace */
        if ($backtrace == null)
            $backtrace = debug_backtrace();

        /* Remove this trace step */
        array_shift($backtrace);

        /* Create error in error list */
        $this->errors[] = new Error($origin, $type, $code, $file, $line, $message, $details, $backtrace);
    }

    /* Exportation methods */

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

        /* Remove output */
        $this->params['output'] = 'none';

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
                'action'    => $error->get_header(),
                'message'   => $message,
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
        /* Check if there are errors to display */
        if (count($this->errors) == 0) return;

        /* Check page output is HTML */
        /* @to do */

        /* Adds scripts */
        if (count($this->errors) > 1) {
            echo '<script>'."\n";
            echo 'errorCurrent = 0;'."\n";
            echo 'errorCount = '.count($this->errors).";\n";
            echo 'function display_previous_error() {document.getElementById("CaMykSError"+errorCurrent).style.display="none"; errorCurrent--; if (errorCurrent === -1) errorCurrent = errorCount-1; document.getElementById("CaMykSError"+errorCurrent).style.display="block"; document.getElementById("CaMykSErrorIndex").innerHTML = errorCurrent + 1;}'."\n";
            echo 'function display_next_error() {document.getElementById("CaMykSError"+errorCurrent).style.display="none"; errorCurrent++; if (errorCurrent === errorCount) errorCurrent = 0; document.getElementById("CaMykSError"+errorCurrent).style.display="block"; document.getElementById("CaMykSErrorIndex").innerHTML = errorCurrent + 1;}'."\n";
            echo '</script>'."\n";
        }

        /* Display results as HTML */
        echo '<div style="position:fixed; bottom:0; left:0; right:0; max-width:1200px; max-height:100%; overflow:auto; margin:0 auto; background:#57488e; border:solid 1px white; z-index:9999999;">'."\n";
        if (count($this->errors) > 1) {
            echo '<div style="padding:8px 12px; border-bottom:solid 1px white; border-left:solid 1px white; float:right; width:80px; text-align:center; box-sizing:border-box; line-height:15px;">'."\n";
            echo '<a href="javascript:display_previous_error();" style="color:white; font-family:arial; font-size:14px; font-weight:bold; text-decoration:none;"> &lt; </a>'."\n";
            echo '<span id="CaMykSErrorIndex" style="color:white; font-family:arial; font-size:14px;"> 1 </span><span style="color:white; font-family:arial; font-size:14px;">  / '.count($this->errors).'</span>'."\n";
            echo '<a href="javascript:display_next_error();" style="color:white; font-family:arial; font-size:14px; font-weight:bold; text-decoration:none;"> &gt; </a>'."\n";
            echo '</div>';
        }
        echo '<div style="padding:8px 12px; border-bottom:solid 1px white; color:white; font-family:arial; font-size:14px;">'."\n";
        echo 'CaMykS Error Console </div>'."\n";

        foreach ($this->errors as $index => $error) {
            echo '<div id="CaMykSError'.$index.'" style="padding:12px; color:white; font-family:arial; font-size:12px; line-height:16px; display:'.($index === 0 ? 'block' : 'none').';">';
            echo $error->export_asHTML()."\n";
            echo '</div>';
        }
        echo '</div>';
    }

    /* Configuration related methods */

    /**
     * Return available error logging output list.
     * @return array
     */
    public function get_availableLoggingOutputs() {
        return array(
            'auto'      => $this->get_locale('Output_Auto'),
            'html'      => $this->get_locale('Output_HTML'),
            'logs'      => $this->get_locale('Output_Logs'),
            'shared'    => $this->get_locale('Output_Shared'),
            'session'   => $this->get_locale('Output_Session'),
            'none'      => $this->get_locale('Output_None'),
        );
    }

    /**
     * Return available error logging level list.
     * @return array
     */
    public function get_availableLoggingLevels() {
        return array(
            'auto'      => $this->get_locale('Level_Auto'),
            'mini'      => $this->get_locale('Level_Mini'),
            'maxi'      => $this->get_locale('Level_Maxi'),
        );
    }
}
?>
