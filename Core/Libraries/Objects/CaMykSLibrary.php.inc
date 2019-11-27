<?php
/**
 * @brief CaMykS generic abstract library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.5
 * @date Creation: Oct 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */

namespace CaMykS2;

/**
 * CaMykSLibrary class.
 */
abstract class CaMykSLibrary {

    /**
     * @var string $libraryName
     * @brief Library name.
     */
    private $libraryName = '';

    /* Parameters */

    /**
     * @var array $params
     * @brief Object "public" parameters.
     */
    protected $params = array();

    /* Errors related variables */

    /**
     * @var array $errorList.
     * @brief List of recorded errors.
     */
    protected $errorList = array();

    /**
     * @var array $errorCodes.
     * @brief List known errors with their codes.
     */
    protected $errorCodes = null;

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct($params=array()) {
        /* Define library name */
        $this->libraryName = preg_replace('#^CaMykS2\\\#', '', get_class($this));

        /* Define initial parameters */
        $this->update_params($params);
    }

    /* Public parameters related methods */

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
    public function update_params($params) {
        $this->params = array_merge($this->params, $params);
    }

    /**
     * Return requested parameter value.
     * @param string $name
     * @return mixed
     */
    public function get_param($name) {
        if (isset($this->params[$name]))
            return $this->params[$name];
        return false;
    }

    /* Public errors related methods */

    /**
     * Check if errors have been recorded.
     * @return boolean result
     */
    public function has_errors() {
        return (count($this->errorList) > 0);
    }

    /**
     * Return last error.
     * @return array
     */
    public function get_lastError() {
        /* Check has error */
        if ($this->has_errors() === false)
            return false;

        /* Return built error */
        return array(
            'Library'   => $this->libraryName,
            'Error'     => $this->errorList[0],
            'Code'      => $this->get_errorCode($this->errorList[0]),
            'Message'   => $this->get_errorMessage($this->errorList[0]),
        );
    }

    /* Protected locales related methods */

    /**
     * Load library locales.
     * @param string $language
     * @return boolean success
     */
    protected function load_locales($language=null) {
        global $CaMykS;

        /* Check CaMykS object */
        if (!isset($CaMykS))
            return false;

        /* Load library locales in CaMykS */
        return $CaMykS->load_engineLocales('Libraries/'.$this->libraryName, $this->libraryName, $language);
    }

    /**
     * Check if locale exists.
     * @param string $locale
     * @return boolean result
     */
    protected function check_locale($locale='') {
        global $CaMykS;

        /* Load library locales in CaMykS */
        if (!$this->load_locales())
            return false;

        /* Check locale in CaMykS object */
        return $CaMykS->check_locale($locale, $this->libraryName);
    }

    /**
     * Return locale.
     * @param string $locale
     * @param mixed $data
     * @return string
     */
    protected function get_locale($locale='', $data=null) {
        global $CaMykS;

        /* Load library locales in CaMykS */
        if (!$this->load_locales())
            return $locale;

        /* Check locale in CaMykS object */
        return $CaMykS->get_locale($locale, $this->libraryName, $data);
    }

    /* Protected errors related methods */

    /**
     * Add an error.
     * @param integer $errorId
     * @param string $details
     * @param string $file
     * @param integer $line
     * @param boolean $isFatal
     * @return false
     */
    protected function add_error($errorId, $details, $file='', $line=0, $isFatal=false) {
        global $CaMykS;

        /* Add error at the beginning of the list */
        array_unshift($this->errorList, $errorId);

        /* Send error to CaMykS */
        $CaMykS->trigger_error($this->libraryName, $errorId, $this->get_errorCode($errorId), $details, $file, $line, $isFatal);

        /* Return false */
        return false;
    }

    /**
     * Define error code list, to be overrided by libraries.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array();
    }

    /* Private errors related methods */

    /**
     * Return error code from its id.
     * @param integer $errorId
     * @return string
     */
    private function get_errorCode($errorId) {
        /* Load error codes if needed */
        if ($this->errorCodes === null)
            $this->define_errorCodes();

        /* Check code exists, and return it */
        if (isset($this->errorCodes[$errorId]))
            return $this->errorCodes[$errorId];

        /* Return default value */
        return 'UnknownError';
    }

    /**
     * Return error message from its id.
     * @param integer $errorId
     * @return string
     */
    protected function get_errorMessage($errorId) {
        global $CaMykS;

        /* Load error code */
        $code = $this->get_errorCode($errorId);

        /* Build locale */
        $locale = 'Error_'.$code;

        /* Return localised message */
        if ($this->check_locale($locale))
            return $this->get_locale($locale);
        return $CaMykS->get_locale('GenericErrorMessage', '('.$this->libraryName.'::'.$errorId.') '.$code);
    }
}
?>
