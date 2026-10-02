<?php
/**
 * @brief CaMykS request answer object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Nov 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * RequestAnswer class.
 */
final class RequestAnswer extends CaMykSLibrary {
    /**
     * @var array $params
     * @brief Build default object parameters.
     */
    protected array $params = array(
        /**< Output value : json / xml */
        'output' => 'json',
        /**< Answer status : success / failure ( / fail ) */
        'status' => '',
        /**< Executed action resulting this answer */
        'action' => '',
        /**< Result message, for success or failure */
        'message' => '',
        /**< Failure error code */
        'error' => '',
    );

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct(array $params=array()) {
        parent::__construct($params);
    }

    /* Mark: Public methods */

    /**
     * Set answer as success.
     * @param string $msg
     * @return void
     */
    public function set_asSuccess(string $msg=''): void {
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
    public function set_asFailure(string $msg='', string $error=''): void {
        $this->params['status'] = 'failure';
        if ($msg != '')
            $this->params['message'] = $msg;
        if ($error != '')
            $this->params['error'] = $error;
    }

    /**
     * Return is request a success.
     * @return bool
     */
    public function is_success(): bool {
        return ($this->params['status'] == 'success');
    }

    /**
     * Return is request a failure.
     * @return bool
     */
    public function is_failure(): bool {
        return (in_array($this->params['status'], array('failure', 'fail')));
    }

    /**
     * Send object as final content.
     * @param array $params
     * @return void
     */
    public function send(array $params=array()): void {
        if ($this->get_param('output') == 'xml')
            $this->send_asXML($params);
        else
            $this->send_asJSON($params);
    }

    /**
     * Send object data as XML (for ajax request).
     * @param array $params
     * @return void
     */
    public function send_asXML(array $params=array()): void {
        global $CaMykS;

        /* Load XML library */
        $CaMykS->load_library('xml', 'Tools');

        /* Prepare for sending */
        $this->prepare_forSending($params);

        /* Send XML */
        header("Content-type: text/xml");
        echo xml_encode($this->params);

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Send object data as JSON (for ajax request).
     * @param array $params
     * @return void
     */
    public function send_asJSON(array $params=array()): void {
        global $CaMykS;

        /* Prepare for sending */
        $this->prepare_forSending($params);

        /* Ensure that data are correctly handled by PHP json method */
        setLocale(LC_NUMERIC, 'en_US.utf8');

        /* Send JSON */
        header("Content-Type: text/javascript; charset=utf8");
        echo json_encode($this->params);

        /* Close CaMykS */
        $CaMykS->close();
    }

    /* Mark: Private methods */

    /**
     * Prepare object for sending.
     * @param array $params
     * @return void
     */
    private function prepare_forSending(array $params): void {
        /* Check for new parameters */
        if (count($params) > 0)
            $this->update_params($params);

        /* Remove private server-side variables */
        unset($this->params['output']);
    }
}
?>
