<?php
/**
 * @brief CaMykS generic abstract library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre13
 * @date Creation: Oct 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
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
    protected string $libraryName = '';

    /**
     * @var string $libraryNamespace
     * @brief Library namespace.
     */
    protected string $libraryNamespace;

    /* Mark: Parameters */

    /**
     * @var array $params
     * @brief Object "public" parameters.
     */
    protected array $params = array();

    /* Mark: Errors related variables */

    /**
     * @var array $errorList.
     * @brief List of recorded errors.
     */
    protected array $errorList = array();

    /**
     * @var array|null $errorCodes.
     * @brief List known errors with their codes.
     */
    protected ?array $errorCodes = null;

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct(array $params=array()) {
        /* Define library namespace and name */
        $className = get_class($this);
        if (($pos = strrpos($className, '\\')) !== false) {
            $this->libraryNamespace = substr($className, 0, $pos);
            $this->libraryName = substr($className, $pos + 1);
        } else {
            $this->libraryNamespace = '';
            $this->libraryName = $className;
        }

        /* Define initial parameters */
        $this->update_params($params);
    }

    /* Mark : Data display methods */

    /**
     * Return library name.
     * @return string
     */
    public function get_libraryName(): string {
        return $this->libraryName;
    }

    /* Mark: Public parameters related methods */

    /**
     * Update single parameter.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function update_param(string $name, mixed $value): void {
        $this->params[$name] = $value;
    }

    /**
     * Update multiple parameters.
     * @param array $params
     * @return void
     */
    public function update_params(array $params): void {
        $this->params = array_merge($this->params, $params);
    }

    /**
     * Return requested parameter value.
     * @param string $name
     * @return mixed
     */
    public function get_param(string $name): mixed {
        if (isset($this->params[$name]))
            return $this->params[$name];
        return false;
    }

    /* Mark: Public configuration related methods */

    /**
     * Return engine configuration value.
     * @param string $name
     * @param mixed $default
     * @return mixed
     */
    public function get_engineConfigValue(string $name, mixed $default=null): mixed {
        global $CaMykS;
        if (is_null($CaMykS))
            return $default;
        return $CaMykS->get_configValue($name, $default);
    }

    /* Mark: Public errors related methods */

    /**
     * Check if errors have been recorded.
     * @return boolean result
     */
    public function has_errors(): bool {
        return (count($this->errorList) > 0);
    }

    /**
     * Return last error.
     * @return array|false
     */
    public function get_lastError(): array|false {
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

    /* Mark: Public locales related methods */

    /**
     * Check if locale exists.
     * @param string $locale
     * @return boolean result
     */
    public function check_locale(string $locale=''): bool {
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
    public function get_locale(string $locale='', mixed $data=null): string {
        global $CaMykS;

        /* Load library locales in CaMykS */
        $this->load_locales();

        /* Return locale from CaMykS object */
        return $CaMykS->get_locale($locale, $this->libraryName, $data);
    }

    /* Mark: Protected locales related methods */

    /**
     * Load library locales.
     * @param string $folder
     * @param string $language
     * @return boolean success
     */
    protected function load_locales(string $folder='', ?string $language=null): bool {
        global $CaMykS;

        /* Check CaMykS object */
        if (!isset($CaMykS))
            return false;

        /* Load library locales in CaMykS */
        return $CaMykS->load_engineLocales('Libraries/'.$this->libraryName.($folder != '' ? '/'.$folder : ''), $this->libraryName, $language);
    }

    /* Mark: Protected errors related methods */

    /**
     * Add an error.
     * @param integer $errorId
     * @param string $details
     * @param string $file
     * @param integer $line
     * @param boolean $isFatal
     * @return false
     */
    protected function add_error(int $errorId, string $details, string $file='', int $line=0, bool $isFatal=false): false {
        global $CaMykS;

        /* Add error at the beginning of the list */
        array_unshift($this->errorList, $errorId);

        /* Send error to CaMykS */
        if (!is_null($CaMykS))
            $CaMykS->trigger_error($this->libraryName, $errorId, $this->get_errorCode($errorId), $details, $file, $line, $isFatal);

        /* Return false */
        return false;
    }

    /**
     * Define error code list, to be overrided by libraries.
     * @return void
     */
    protected function define_errorCodes(): void {
        $this->errorCodes = array();
    }

    /* Mark: Private errors related methods */

    /**
     * Return error code from its id.
     * @param integer $errorId
     * @return string
     */
    protected function get_errorCode(int $errorId): string {
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
    protected function get_errorMessage(int $errorId): string {
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
