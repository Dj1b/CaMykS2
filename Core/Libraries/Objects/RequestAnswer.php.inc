<?php
/**
 * @brief CaMykS request answer object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * RequestAnswer class.
 */
final class RequestAnswer {
    /**
     * @var array $params
     * @brief Build default object parameters.
     */
    private $params = array(
        /**< Answer status : success / failure ( / fail ) */
        'status' => '',
        /**< Parent module or module library */
        'parent' => '',
        /**< Executed action resulting this answer */
        'action' => '',
        /**< Result message, for success or failure */
        'message' => '',
        /**< Failure error code */
        'error' => '',
        /**< Request file */
        'file' => '',
        /**< Request line */
        'line' => 0,
    );

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct($params=array()) {
        /* Build object paramaters list */
        $this->params = array_merge($this->params, $params);
    }

    /* public methods */

    /**
     * Return parameter.
     * @param string $name
     * @return mixed
     */
    public function get_param($name='') {
        if (isset($this->params[$name]))
            return $this->params[$name];
        return false;
    }

    /**
     * Update single parameter.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function update_param($name, $value) {
        $this->params[$name] = $value;
    }

    /**
     * Update multiple parameters.
     * @param array $params
     * @return void
     */
    public function update_params($params=array()) {
        $this->params = array_merge($this->params, $params);
    }

    /**
     * Set answer as success.
     * @param string $msg
     * @return void
     */
    public function set_asSuccess($msg='') {
        $this->params['status'] = 'success';
        if ($msg != '')
            $this->params['message'] = $msg;
    }

    /**
     * Set answer as failure.
     * @param string $msg
     * @param string $error
     * @return void
     */
    public function set_asFailure($msg='', $error='') {
        $this->params['status'] = 'failure';
        if ($msg != '')
            $this->params['message'] = $msg;
        if ($error != '')
            $this->params['error'] = $error;
    }

    /**
     * Return is request a success.
     * @return boolean result
     */
    public function is_success() {
        return ($this->params['status'] == 'success');
    }

    /**
     * Return is request a failure.
     * @return boolean result
     */
    public function is_failure() {
        return (in_array($this->params['status'], array('failure', 'fail')));
    }

    /**
     * Throw fatal error for failure.
     * @return void
     */
    public function throw_fatalError() {
        global $CaMykS;

        /* Check if answer is a failure */
        if ($this->is_failure() === false)
            return;

        /* Build error */
        $error = array();

        /* Check for error title */
        if ($this->params['parent'] != '') {
            if (is_string($this->params['parent']))
                $error['title'] = $this->params['parent'];
            elseif (is_object($this->params['parent']) and isset($this->params['parent']->object_type)) {
                if ($this->params['parent']->object_type == 'ModuleLibrary')
                    $error['title'] = $this->params['parent']->_libParent->title;
                elseif (isset($this->params['parent']->title))
                    $error['title'] = $this->params['parent']->title;
            }
        }

        /* Check for description & content */
        if (is_string($this->params['message'])) {
            $error['description'] = $this->params['message'];
        } else {
            $error['description'] = $this->params['message'][0];
            for ($i=1; $i<count($this->params['message']); $i++)
                $error['content'.$i] = $this->params['message'][0];
        }

        /* Throw CaMykS fatal error */
        $CaMykS->throwFatalError($error);
    }

    /**
     * Log error for failure.
     * @return void
     */
    public function log_error() {
        global $CaMykS;

        /* Check parent */
        if (!is_object($this->params['parent']))
            return;

        /* Check description & content */
        if (is_string($this->params['message'])) {
            $msg1 = $this->params['message'];
        } else {
            $msg1 = implode('<br />', $this->params['message']);
        }

        /* Check error  */
        if ( $this->params['error'] != '') {
            $msg2 = $msg1;
            $msg1 = $this->params['error'];
        } else {
            $msg2 = '';
        }

        /* Send error */
        $CaMykS->log_internalerror($this->params['parent'], $msg1, $msg2, $this->params['file'], $this->params['line']);
    }

    /**
     * Send object data as XML (for ajax request).
     * @param array $params
     * @return void
     */
    public function send_asXML($params=array()) {
        /* Prepare for sending */
        $this->prepare_forSending($params);

        /* Send XML */
        header("Content-type: text/xml");
        die(xml_serialize2XML($this->params));
    }

    /**
     * Send object data as JSON (for ajax request).
     * @param array $params
     * @return void
     */
    public function send_asJSON($params=array()) {
        /* Prepare for sending */
        $this->prepare_forSending($params);

        /* Ensure that data are correctly handled by PHP json method */
        setLocale(LC_NUMERIC, 'en_US.utf8');

        /* Send JSON */
        header("Content-Type: text/javascript; charset=utf8");
        die(json_encode($this->params));
    }

    /**
     * Prepare object for sending.
     * @param array $params
     * @return void
     */
    private function prepare_forSending($params) {
        /* Check for new parameters */
        if (count($params) > 0)
            $this->update_params($params);

        /* Remove private server-side variables */
        unset($this->params['parent']);
        unset($this->params['file']);
        unset($this->params['line']);
    }
}
?>
