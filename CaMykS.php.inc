<?php
/**
 * @brief CaMykS version 2 CMS master class.
 * @details Engine / Master Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a2
 * @date Creation: Feb 2017
 * @date Modification: Nov 2019
 * @copyright 2017 - 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * @def CAMYKS_MODE_SITE
 */
define('CAMYKS_MODE_SITE',      0);

/**
 * @def CAMYKS_MODE_ADMIN
 */
define('CAMYKS_MODE_ADMIN',     1);

/**
 * @def CAMYKS_MODE_REQUEST
 */
define('CAMYKS_MODE_REQUEST',   2);

/**
 * @def CAMYKS_MODE_MANAGER
 */
define('CAMYKS_MODE_MANAGER',   3);

/**
 * @def CAMYKS_MODE_INSTALL
 */
define('CAMYKS_MODE_INSTALL',   4);

/**
 * CaMykS class.
 */
final class CaMykS {
    /**
     * @var integer $mode
     * @brief CaMykS current mode
     */
    private $mode;

    /**
     * @var array $currentLanguage
     * @brief Optimised language currently used.
     */
    private $currentLanguage = array();

    /**
     * @var array $locales
     * @brief List of all available locales.
     */
    private $locales = array();

    /**
     * @var array $loadedLocales
     * @brief List all loaded locale files.
     */
    private $loadedLocales = array();

    /**
     * @var integer $startMicroTime
     * @brief Store CaMykS starting microtime
     */
    private $startMicroTime;

    /**
     * Class constructor.
     * @return void
     */
    public function __construct() {
        /* Update start microtime value */
        $this->startMicroTime = microtime(true);
    }

    /**
     * Execute CaMykS engine
     * @param integer $mode to run
     * @return void
     */
    public function execute($mode) {
        /* Store mode */
        $this->mode = $mode;

        /* Initialise */
        $this->initialise();

        /* Display */
        $this->display();
    }

    /* Library related methods */

    /**
     * Load given library.
     * @param string $libraryPath
     * @return boolean success
     */
    public function load_library($libraryPath) {
        /* TO DO SET in camyks initialisation */
        $rootPath = dirname(__FILE__);

        /* Define file */
        $file = $rootPath.'/Core/Libraries/'.$libraryPath.'.php.inc';

        /* Check file */
        if (!file_exists($file))
            return false;

        /* Load library */
        require_once($file);
    }

    /**
     * Return asset file URL.
     * @param string $asset
     * @return string
     */
    public function get_assetFileURL($asset) {
        /* TO DO SET in camyks initialisation */
        $rootPath = dirname(__FILE__);
        $rootURL = '/CaMykS2';

        /* Define file */
        $file = '/Core/Assets/'.$asset;

        /* Check file */
        if (file_exists($rootPath.$file))
            return $rootURL.$file;

        /* Default for files that don't exists */
        return '';
    }

    /* Language related methods */

    /**
     * Load current language from context.
     * @return void
     */
    private function load_currentLanguage() {
        /* Load language from current session and CaMykS mode */

        /* Load language from request */

        /* Load language from user agent */

        /* Define language to website default language */

        /* Apply language */
        $this->set_currentLanguage($language);
    }

    /**
     * Set current language.
     * @param mixed $language
     * @return boolean success
     */
    public function set_currentLanguage($language) {
        /* Get optimised language */
        $language = language_read($language);
        if ($language === false)
            return false;

        /* Store language */
        $this->currentLanguage = $language;

        /* Change systeme language */
        if (setlocale(LC_ALL, language_getSystemLanguages($language)) === false)
            return false;

        /* Return success */
        return true;
    }

    /**
     * Return current language.
     * @return array
     */
    public function get_currentLanguage() {
        return $this->currentLanguage;
    }

    /* Locale related methods */

    /**
     * Load locale file.
     * @param string $file
     * @param string $language
     * @return boolean success
     */
    public function load_locales($file, $language=null) {
        /* Check language */
        if ($language == null)
            $language = $this->get_currentLanguage();

        /* Load language sequence */
        $languages = language_getLocaleLoadingSequence($language);

        /* Try language sequence */
        foreach ($languages as $l) {
            /* Check locale is already loaded and still active */
            if (isset($this->loadedLocales[$file]) and $this->loadedLocales[$file] == $l)
                return true;

            /* Build path */
            $path = $file .'/Locale.'.$l.$this->libraryExtension;

            /* Check if file exists */
            if (file_exists($path)) {
                require $path;
                $this->loadedLocales[$file] = $l;
                return true;
            }
        }

        /* No locale found for given language, try to load default language locales */
        $languages = language_getLocaleLoadingSequence($this->defaultLanguage);

        /* Try language sequence */
        foreach ($languages as $l) {
            /* Check locale is already loaded and still active */
            if (isset($this->loadedLocales[$file]) and $this->loadedLocales[$file] == $l)
                return true;

            /* Build path */
            $path = $file .'/Locale.'.$l.$this->libraryExtension;

            /* Check if file exists */
            if (file_exists($path)) {
                require $path;
                $this->loadedLocales[$file] = $l;
                return true;
            }
        }

        /* No locale file found, return failure */
        return false;
    }

    /**
     * Return locale string.
     * @param string $key
     * @param string $default
     * @return boolean string
     */
    public function get_locale($key, $default=null) {
        /* Search and return locale from key */
        if (isset($this->locales[$key]))
            return $this->locales[$key];

        /* Key not found, return default value */
        if ($default != null)
            return $default;

        /* No default values, return key */
        return $key;
    }

    /**
     * Return keys from locale
     * @param string $value
     * @param string $prefix
     * @return array
     */
    public function get_localeKeysByValue($value, $prefix='') {
        /* Search keys */
        $keys = array_keys($this->locales, $value);

        /* Remove prefix, if given */
        if ($prefix != '')
            $keys = preg_filter('/^'.preg_quote($prefix).'(.*)/', '$1', $keys);

        /* Return keys */
        return $keys;
    }
}
?>
