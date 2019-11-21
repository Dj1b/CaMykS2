<?php
/**
 * @brief CaMykS version 2 CMS master class.
 * @details Engine / Master Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a8
 * @date Creation: Feb 2017
 * @date Modification: Nov 2019
 * @copyright 2017 - 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * @def CAMYKS_MODE_VISIT
 */
define('CAMYKS_MODE_VISIT',     0);

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
     * @brief CaMykS current mode.
     */
    private $mode;

    /**
     * @var array $engineConfiguration
     * @brief CaMykS configuration.
     */
    private $engineConfiguration = array();

    /**
     * @var array $websiteConfiguration
     * @brief Website configuration.
     */
    private $websiteConfiguration = array();

    /**
     * @var array $plugins
     * @brief Loaded plugins.
     */
    private $plugins = array('Components'=>array(), 'Modules'=>array(), 'Templates'=>array(), 'Themes'=>array());

    /**
     * @var array $currentLanguage
     * @brief Optimised language currently used.
     */
    private $currentLanguage = 'en_GB';

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
     * @var array $openedDBonnections
     * @brief CaMykS opened database connections.
     */
    private $openedDBonnections = array();

    /**
     * @var array $eventsHandlers
     * @brief CaMykS event handlers.
     */
    private $eventsHandlers = array(
        'onInitialise'      => array(),
        'onVisitPageLoad'   => array(),
        'onVisitBodyOpen'   => array(),
        'onVisitBodyClose'  => array(),
        'onAdminPageLoad'   => array(),
        'onAdminBodyOpen'   => array(),
        'onAdminBodyClose'  => array(),
        'onTerminate'       => array(),
    );

    /**
     * @var Object $bench
     * @brief CaMykS internal bench object.
     */
    private $bench = false;

    /**
     * @var Object $errorHandler
     * @brief CaMykS internal error handler.
     */
    private $errorHandler = false;

    /**
     * var string $CaMykSPath
     * @brief CaMykS path.
     */
    private $CaMykSPath;

    /**
     * var string $websitePath
     * @brief Website path.
     */
    private $websitePath;

    /**
     * var string $websiteURL
     * @brief Website URL.
     */
    private $websiteURL;

    /**
     * @var boolean $falseRef
     * @brief CaMykS false value as reference.
     */
     private $falseRef = false;

    /**
     * Class constructor.
     * @param array $configuration
     * @return void
     */
    public function __construct($configuration=array()) {
        /* Define website configuration */
        $this->websiteConfiguration = $configuration;

        /* Set start microtime value */
        $this->startMicroTime = microtime(true);

        /* Register shutdown internal function */
        register_shutdown_function(array($this, 'terminate'));
    }

    /* CaMykS main method */

    /**
     * Execute CaMykS engine.
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

        /* Close */
        $this->close();
    }

    /* CaMykS main sub methods */

    /**
     * Initialise CaMykS.
     * @return void
     */
    private function initialise() {
        /* Initialise engine */
        $this->initialise_engine();

        /* Add bench step */
        $this->bench->add_step('CaMykS initialise mode.');

        /* Initialise mode */
        switch ($this->mode) {
            case CAMYKS_MODE_VISIT:
                $this->initialise_visit();
                break;
            case CAMYKS_MODE_ADMIN:
                $this->initialise_admin();
                break;
            case CAMYKS_MODE_REQUEST:
                $this->initialise_request();
                break;
            case CAMYKS_MODE_MANAGER:
                $this->initialise_manager();
                break;
            case CAMYKS_MODE_INSTALL:
                $this->initialise_install();
                break;
        }

        /* Run onInitialise event handlers */
        $this->run_eventHandlers('onInitialise');
    }

    /**
     * Display CaMykS.
     * @return void
     */
    private function display() {

        /* Add bench step */
        $this->bench->add_step('CaMykS display mode.');

        /* Dsiplay mode */
        switch ($this->mode) {
            case CAMYKS_MODE_VISIT:
                $this->display_visit();
                break;
            case CAMYKS_MODE_ADMIN:
                $this->display_admin();
                break;
            case CAMYKS_MODE_REQUEST:
                $this->display_request();
                break;
            case CAMYKS_MODE_MANAGER:
                $this->display_manager();
                break;
            case CAMYKS_MODE_INSTALL:
                $this->display_install();
                break;
        }
    }

    /**
     * Close CaMykS.
     * @return void
     */
    public function close() {
        /* Kill processus to call terminate through registered shutdown function */
        //die();
    }

    /**
     * Terminate CaMykS.
     * @return void
     */
    public function terminate() {
        /* Add bench step */
        if ($this->bench !== false) $this->bench->add_step('CaMykS terminate.');

        /* Close database connections */
        foreach ($this->openedDBonnections as $connection)
            $connection->close();

        /* Run onTerminate event handlers */
        $this->run_eventHandlers('onTerminate');

        /* Export errorHandler result */
        if ($this->errorHandler !== false) $this->errorHandler->export();

        /* Export bench result */
        if ($this->bench !== false) $this->bench->export();
    }

    /* CaMykS initialisation methods */

    /**
     * Initialise engine.
     * @return void
     */
    private function initialise_engine() {
        /* Set up CaMykS files path */
        $this->CaMykSPath = dirname(__FILE__);

        /* Set up website files path */
        $this->webitePath = getcwd();

        /* Load CaMykS configuration */
        require_once $this->CaMykSPath.'/Core/Configuration/CaMykS.php.inc';

        /* Check if website is installed */
        if (in_array($this->mode, array(CAMYKS_MODE_VISIT, CAMYKS_MODE_ADMIN, CAMYKS_MODE_REQUEST)) and $this->get_configuration('Website_Status') == 0)
            $this->mode = CAMYKS_MODE_INSTALL;

        /* Load engine configuration */
        $this->load_configuration('Engine');

        /* Define time zone */
        date_default_timezone_set($this->get_configValue('Engine_TimeZone', 'Europe/Paris'));

        /* Load libraries */
        $this->load_engineLibraries();

        /* Load bench object */
        $this->bench = new Bench();
        $this->bench->start('CaMykS loading.', $this->startMicroTime);
        unset($this->startMicroTime);

        /* Load errorhandler object */
        //$this->errorHandler = new ErrorHandler();
    }

    /**
     * Initialise visit mode.
     * @return void
     */
    private function initialise_visit() {
        /* Load visit configuration */
        $this->load_configuration('Visit');

        /* Set up base URL */
        $this->define_baseURL($this->get_configValue('Visit_SSL'));

        /* Make common initialisation */
        $this->initialise_common();
    }

    /**
     * Initialise admin mode.
     * @return void
     */
    private function initialise_admin() {
        /* Load admin configuration */
        $this->load_configuration('Admin');

        /* Set up base URL */
        $this->define_baseURL($this->get_configValue('Admin_SSL'));

        /* Make common initialisation */
        $this->initialise_common();
    }

    /**
     * Execute common initialisation from visit and admin mode, can eventually be called from request mode.
     * @return void
     */
    public function initialise_common() {

    }

    /**
     * Initialise request mode.
     * @return void
     */
    private function initialise_request() {

    }

    /**
     * Initialise manager mode.
     * @return void
     */
    private function initialise_manager() {
        /* Load manager configuration */
        $this->load_configuration('Manager');

        /* Set up base URL */
        $this->define_baseURL($this->get_configValue('Manager_SSL'));

        /* Load language */
        $this->load_currentLanguage();


    }

    /**
     * Initialise install mode.
     * @return void
     */
    private function initialise_install() {

    }

    /* CaMykS display methods */

    /**
     * Display visit mode.
     * @return void
     */
    private function display_visit() {

    }

    /**
     * Display admin mode.
     * @return void
     */
    private function display_admin() {

    }

    /**
     * Display visit mode.
     * @return void
     */
    private function display_request() {

    }

    /**
     * Display manager mode.
     * @return void
     */
    private function display_manager() {

    }

    /**
     * Display install mode.
     * @return void
     */
    private function display_install() {

    }

    /* Configuration related methods */

    /**
     * Load configuration.
     * @param string $configFile
     * @return void
     */
    private function load_configuration($configFile) {
        /* Load engine configuration file */
        $file = 'Core/Configuration/'.$configFile.$this->libraryExtension;
        if ($this->check_engineFileExists($file)) {
            require_once($this->get_engineFileFullPath($file));
            $this->engineConfiguration = array_merge($this->engineConfiguration, $configuration);
        }

        /* Load website configuration file */
        $file = 'Configuration/'.$configFile.$this->libraryExtension;
        if ($this->check_websiteFileExists($file)) {
            require_once($this->get_websiteFileFullPath($file));
            $this->websiteConfiguration = array_merge($this->websiteConfiguration, $configuration);
        }
    }

    /**
     * Return configuration value.
     * @param string $name
     * @return mixed
     */
    public function get_configValue($name, $default=false) {
        /* Check website configuration */
        if (isset($this->websiteConfiguration[$name]) and $this->websiteConfiguration[$name] != null)
            return $this->websiteConfiguration[$name];

        /* Check CaMykS configuration */
        if (isset($this->engineConfiguration[$name]) and $this->engineConfiguration[$name] != null)
            return $this->engineConfiguration[$name];

        /* Return default value */
        return $default;
    }

    /**
     * Define or update temporary configuration value.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function set_configValue($name, $value) {
        $this->websiteConfiguration[$name] = $value;
    }

    /* Library related methods */

    /**
     * Load given library.
     * @param string $library
     * @param string $folder
     * @return boolean success
     */
    public function load_library($library, $folder='') {
        /* Define file */
        $file = 'Core/Libraries/'.($folder == '' ? '' : $folder.'/').$library.'.php.inc';

        /* Check file */
        if (!$this->check_engineFileExists($file))
            return false;

        /* Load library */
        require_once($this->get_engineFileFullPath($file));
    }

    /**
     * Load engine libraries.
     * @return void
     */
    private function load_engineLibraries() {
        /* Start capturing extra code in live mode */
        if ($this->get_configValue('Website_Status') == false)
            ob_start();

        /* Load tool libraries */
        $this->load_library('array', 'Tools');
        $this->load_library('country', 'Tools');
        $this->load_library('currency', 'Tools');
        $this->load_library('date', 'Tools');
        $this->load_library('html', 'Tools');
        $this->load_library('language', 'Tools');

        /* Load parent object libraries */
        $this->load_library('CaMykSLibrary', 'Objects');
        $this->load_library('Plugin', 'Objects');

        /* Load object libraries */
        $this->load_library('Bench', 'Objects');
        $this->load_library('DatabaseConnector', 'Objects');
        $this->load_library('DatabaseDriverInterface', 'Objects');
        $this->load_library('DbQuery', 'Objects');
        $this->load_library('Error', 'Objects');
        $this->load_library('ErrorHandler', 'Objects');
        $this->load_library('RequestAnswer', 'Objects');

        /* Load site libraries */
        if ($this->websitePath != $this->CaMykSPath and file_exists($this->websitePath.'./Assets/Libraries')) {
            foreach (glob($this->websitePath.'./Assets/Libraries/*'.$this->libraryExtension) as $file)
                require_once($file);
        }

        /* Clean captured extra code in live mode */
        if ($this->get_configValue('Website_Status') == false)
            ob_clean();
    }

    /* Plugin related methods */

    /**
     * Load plugin.
     * @param string $name
     * @param string $type
     * @param string $location
     * @return boolean result
     */
    public function load_plugin($name, $type, $location=false) {
        /* Normalise type */
        $type = ucwords(strtolower($type).'s');

        /* Check is already loaded */
        if (isset($this->plugins[$type][$name]))
            return true;

        /* Check is in system plugins */
        if ($location == 'system' or ($location === false and $this->check_engineFileExists('Core/Plugins/'.$type.'/'.$name.'/'.$name.$this->libraryExtension))) {
            $location = 'system';
            $path = $this->get_engineFileFullPath('Plugins/'.$type.'/'.$name.'/'.$name.$this->libraryExtension);
        }

        /* Check is in shared plugins */
        elseif ($location == 'shared' or ($location === false and $this->check_engineFileExists('Plugins/'.$type.'/'.$name.'/'.$name.$this->libraryExtension))) {
            $location = 'shared';
            $path = $CaMykS->get_engineFileFullPath('Plugins/'.$type.'/'.$name.'/');
        }

        /* Check is in website plugins */
        elseif ($location == 'website' or ($location === false and  $this->check_websiteFileExists('Plugins/'.$type.'/'.$name.'/'.$name.$this->libraryExtension))) {
            $location = 'website';
            $path = $CaMykS->get_websiteFileFullPath('Plugins/'.$type.'/'.$name.'/');
        }

        /* Plugin not found */
        else {
            /* Log error ? */

            /* Return failure */
            return false;
        }

        /* Load plugin */
        try {
            require_once $path;
            $this->plugins[$type][$name] = new $$name($location);
        } catch (Exception $e) {
            /* Log error */

            /* Return failure */
            return false;
        }

        /* Return success */
        return true;
    }

    /**
     * Load component.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_component($name, $location=false) {
        return $this->load_plugin($name, 'Component', $location);
    }

    /**
     * Load module.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_module($name, $location=false) {
        return $this->load_plugin($name, 'Module', $location);
    }

    /**
     * Load template.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_template($name, $location=false) {
        return $this->load_plugin($name, 'Template', $location);
    }

    /**
     * Load theme.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_theme($name, $location=false) {
        return $this->load_plugin($name, 'Theme', $location);
    }

    /**
     * Return link to plugin object.
     * @param string $name
     * @param string $type
     * @param boolean $autoLoad
     * @return Plugin
     */
    public function &get_plugin($name, $type, $autoLoad=false) {
        /* Normalise type */
        $type = ucwords(strtolower($type).'s');

        /* Check is in loaded plugin */
        if (isset($this->plugins[$type][$name]))
            return $this->plugins[$type][$name];

        /* Try to load plugin */
        if ($autoLoad and $this->load_plugin($name, $type))
            return $this->plugins[$type][$name];

        /* Return failure */
        return $this->falseRef;
    }

    /**
     * Return link to component object.
     * @param string $name
     * @param boolean $autoLoad
     * @return Input
     */
    public function &get_component($name, $autoLoad=false) {
        return $this->get_plugin($name, 'Component', $autoLoad);
    }

    /**
     * Return link to module object.
     * @param string $name
     * @param boolean $autoLoad
     * @return Module
     */
    public function &get_module($name, $autoLoad=false) {
        return $this->get_plugin($name, 'Module', $autoLoad);
    }

    /**
     * Return link to theme object.
     * @param string $name
     * @param boolean $autoLoad
     * @return Theme
     */
    public function &get_theme($name, $autoLoad=false) {
        return $this->get_plugin($name, 'Theme', $autoLoad);
    }

    /**
     * Return link to template object.
     * @param string $templateName
     * @param boolean $autoLoad
     * @return Template
     */
    public function &get_template($templateName, $autoLoad=false) {
        return $this->get_plugin($name, 'Template', $autoLoad);
    }

    /**
     * Load system plugins.
     * @return boolean success
     * @todo
     */
    private function load_systemPlugins() {

    }

    /**
     * Load website plugins.
     * @return boolean success
     * @todo
     */
    private function load_websitePlugins() {

    }

    /* File related methods */

    /**
     * Return engine file full path.
     * @param string $filePath
     * @return string
     */
    public function get_engineFileFullPath($filePath) {
        return $this->CaMykSPath.'/'.$filePath;
    }

    /**
     * Check if given engine file exists.
     * @param string $filePath
     * @return boolean result
     */
    public function check_engineFileExists($filePath) {
        return file_exists($this->get_engineFileFullPath($filePath));
    }

    /**
     * Return website file full path.
     * @param string $filePath
     * @return string
     */
    public function get_websiteFileFullPath($filePath) {
        return $this->websitePath.'/'.$filePath;
    }

    /**
     * Check if given website file exists.
     * @param string $filePath
     * @return boolean result
     */
    public function check_websiteFileExists($filePath) {
        return file_exists($this->get_websiteFileFullPath($filePath));
    }

    /* URL related methods */

    /**
     * Define website required URL.
     * @param integer $SSLMode
     * @return void
     */
    private function define_baseURL($SSLMode=0) {
        /* Load current URL */
        $url = $_SERVER['REQUEST_URI'];

        /* Load website URL */
        $this->websiteURL = $this->get_configValue('Website_URL');
        if ($this->websiteURL === false) {
            $this->websiteURL = substr($url, 0, strrpos($url, '/'));
        }

        /* Check scheme from SSLMode */
        switch ($SSLMode) {
            case 1 : // Forced to HTTPS
                $scheme = 'https';
                break;
            case 2 : // Forced to HTTP
                $scheme = 'http';
                break;
            case 3 : // Autocheck
                /* @todo */
                break;
            case 0 : // default / client's choice
            default :
                $scheme = parse_url($url, PHP_URL_SCHEME);
        }

        /* Check host */
        $host = parse_url($this->websiteURL, PHP_URL_HOST);

        /* Check path */
        $path = parse_url($this->websiteURL, PHP_URL_PATH);

        /* Finalise URL */
        $this->websiteURL = $scheme.'://'.$host.'/'.($path != '' ? $path.'/' : '');
    }

    /**
     * Return base URL, case of given mode.
     * @param mixed $URLMode
     * @return $string
     */
    public function get_baseURL($URLMode=false) {
        /* Check for website default URL mode */
        if ($URLMode == false)
            $URLMode = $this->get_configValue('Website_URLMode');

        /* Return base url from mode */
        switch ($URLMode) {
            case 'Absolute':
            case 'AbsoluteLong':
                return $this->websiteURL.'/';
            case 'AbsoluteShort':
                return '/';
            case 'Relative':
            default:
                return '';
        }
        return '';
    }

    /**
     * Return asset file URL.
     * @param string $asset
     * @param string $URLMode
     * @return string
     */
    public function get_assetFileURL($asset, $URLMode=false) {
        /* Check file in website */
        $file = 'Assets/'.$asset;
        if ($this->mode < 3) {
            if (file_exists($file))
                return $this->get_baseURL($URLMode).$file;
        }

        /* Check file in CaMykS */
        $file = 'Core/'.$file;
        if ($this->check_engineFileExists($file))
            return $this->get_baseURL($URLMode).$file;

        /* Default for files that don't exists */
        return '';
    }

    /* Language related methods */

    /**
     * Load current language from context.
     * @param integer $mode
     * @return void
     */
    private function load_currentLanguage($mode=null) {
        /* Check mode */
        if (is_null($mode)) $language = $this->get_currentLanguage();

        /* Load language from request */

        /* Load language from current session and given mode */

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
     * @param string $prefix
     * @param string $language
     * @return boolean success
     */
    public function load_locales($file, $prefix='', $language=null) {
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

            /* Try to load locale file */
            if ($this->load_localeFile($file, $prefix, $l))
                return true;
        }

        /* No locale found for given language, try to load default language locales */
        $languages = language_getLocaleLoadingSequence($this->defaultLanguage);

        /* Try language sequence */
        foreach ($languages as $l) {
            /* Check locale is already loaded and still active */
            if (isset($this->loadedLocales[$file]) and $this->loadedLocales[$file] == $l)
                return true;

            /* Try to load locale file */
            if ($this->load_localeFile($file, $prefix, $l))
                return true;
        }

        /* No locale file found, return failure */
        return false;
    }

    /**
     * Load engine locale file.
     * @param string $file
     * @param string $prefix
     * @param string $language
     * @return boolean success
     */
    public function load_engineLocales($file, $prefix='', $language=null) {
        return $this->load_locales($this->CaMykSPath.'/Core/Locales/'.$file, $prefix, $language);
    }

    /**
     * Check if locale exists.
     * @param string $key
     * @param string $prefix
     * @return boolean result
     */
    public function check_locale($key, $prefix='') {
        /* Check prefix */
        if ($prefix != '')
            $prefix .= '_';

        /* Do the check */
        return isset($this->locales[[$prefix.$key]);
    }

    /**
     * Return locale string.
     * @param string $key
     * @param string $prefix
     * @param mixed $data
     * @param string $default
     * @return string
     */
    public function get_locale($key, $prefix='', $data=null, $default=null) {
        /* Check prefix */
        if ($prefix != '')
            $prefix .= '_';

        /* Search and return locale from key */
        if (isset($this->locales[$prefix.$key]))
            return vsprintf($this->locales[$prefix.$key], $data);

        /* Key not found, return default value */
        if (!is_null($default))
            return vsprintf($default, $data);

        /* No default values, return key */
        return $key;
    }

    /**
     * Return keys from locale.
     * @param string $value
     * @param string $prefix
     * @return array
     */
    public function get_localeKeysByValue($value, $prefix='') {
        /* Search keys */
        $keys = array_keys($this->locales, $value);

        /* Remove prefix, if given */
        if ($prefix != '')
            $keys = preg_filter('/^'.preg_quote($prefix).'_(.*)/', '$1', $keys);

        /* Return keys */
        return $keys;
    }

    /**
     * Load locale file.
     * @param string $file
     * @param string $prefix
     * @param string $locale
     * @return boolean success
     */
    private function load_localeFile($file, $prefix, $language) {
        /* Build path */
        $path = $file .'/Locales.'.$language.$this->libraryExtension;

        /* Check if file exists */
        if (!file_exists($path))
            return false;

        /* Load file */
        require $path;

        /* Check locales variables */
        if (!isset($locales) or !is_array($locales))
            return false;

        /* Update locales with prefix */
        if (trim($prefix) != '')
            $locales = array_prefixKeys($locales, trim($prefix).'_');

        /* Add locales to existing locales list */
        $this->locales = array_merge($this->locales, $locales);

        /* Update locale files list */
        $this->loadedLocales[$file] = $language;

        /* Return success */
        return true;
    }

    /** Logging & errors related methods */

    /**
     * Log an information.
     * @param string $object
     * @param string $action
     * @param string $message
     * @return boolean success
     */
    public function log($object, $action, $message='') {
        $logManager = $this->get_module('Admin_SystemLogManager', true);
        if ($logManager === false)
            return false;
        return $logManager->log($object, $action, $message);
    }

    /**
     * Throw fatal error.
     * @param CaMykS\Error $error
     * @return void
     */
    public function throw_fatalError($error) {
        /* Log fatal error */
        $this->log_error($error);

        /* Display fatal error */
        $this->display_fatalError($error);
    }

    /**
     * Log an error.
     * @param CaMykS\Error $error
     * @return boolean success
     */
    private function log_error($error) {
        $logManager = $this->get_module('Admin_SystemLogManager', true);
        if ($logManager === false)
            return false;
        return $logManager->log_error($error);
    }

    /**
     * Display a fatal error.
     * @param CaMykS\Error $error
     * return void
     * @todo
     */
    public function display_fatalError($error) {
        /* Load HTMLPage */

        /* Display error */

        /* Close CaMykS */
        $this->close();
    }

    /* Events handler related methods */

    /**
     * Register an event handler.
     * @param string $event
     * @param Object $plugin
     * @return void
     */
    public function register_eventHandler($event, &$plugin) {
        $this->eventsHandlers[$event][] = $plugin;
    }

    /**
     * Run event handlers.
     * @param string $event
     * @return void
     */
    private function run_eventHandlers($event) {
        foreach ($this->eventsHandlers[$event] as $plugin) {
            switch ($event) {
                case 'onInitialise' :       $plugin->execute_onCaMykSInitialise(); break;
                case 'onVisitPageLoad' :    $plugin->execute_onCaMykSVisitPageLoad(); break;
                case 'onVisitBodyOpen' :    $this->HTMLPage->write_HTMLContent($plugin->execute_onCaMykSVisitBodyOpen()); break;
                case 'onVisitBodyClose' :   $this->HTMLPage->write_HTMLContent($plugin->execute_onCaMykSVisitBodyClose()); break;
                case 'onAdminPageLoad' :    $plugin->execute_onCaMykSAdminPageLoad(); break;
                case 'onAdminBodyOpen' :    $this->HTMLPage->write_HTMLContent($plugin->execute_onCaMykSAdminBodyOpen()); break;
                case 'onAdminBodyClose' :   $this->HTMLPage->write_HTMLContent($plugin->execute_onCaMykSAdminBodyClose()); break;
                case 'onTerminate' :        $plugin->execute_onCaMykSTerminate(); break;
            }
        }
    }
}
?>
