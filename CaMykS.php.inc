<?php
/**
 * @brief CaMykS version 2 CMS master class.
 * @details Engine / Master Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a3
 * @date Creation: Feb 2017
 * @date Modification: Nov 2019
 * @copyright 2017 - 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

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
     * @var array $openedDBonnections
     * @brief CaMykS opened database connections.
     */
    private $openedDBonnections = array();

    /**
     * @var array $eventsHandlers
     * @brief CaMykS event handlers.
     */
    private $eventsHandlers = array(
        'onInitialise'=>array(),
        'onVisitPageLoad'=>array(),
        'onVisitBodyOpen'=>array(),
        'onVisitBodyClose'=>array(),
        'onAdminPageLoad'=>array(),
        'onAdminBodyOpen'=>array(),
        'onAdminBodyClose'=>array(),
        'onTerminate'=>array(),
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

        /* Update start microtime value */
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
    }

    /* CaMykS main sub methods */

    /**
     * Initialise CaMykS.
     * @return void
     */
    private function initialise() {
        /* Initialise engine */
        $this->initialise_engine();

        /* Initialise mode */
        $this->bench->add_step('CaMykS initialise mode.');
        switch ($this->mode) {
            case CAMYKS_MODE_VISIT:
            case CAMYKS_MODE_ADMIN:
                $this->initialise_visit();
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
    }

    /**
     * Display CaMykS.
     * @return void
     */
    private function display() {

    }

    /**
     * Close CaMykS engine.
     * @return void
     */
    public function terminate() {
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
        $this->bench = new CBench();
        $this->bench->start('CaMykS loading.', $this->startMicroTime);
        unset($this->startMicroTime);

        /* Load errorhandler object */
        //$this->errorHandler = new CErrorHandler();

        /* Define engine public file URLs */
        $this->define_URLs();
    }

    /**
     * Initialise visit/admin mode.
     * @return void
     */
    private function initialise_visit() {

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


    }

    /**
     * Initialise install mode.
     * @return void
     */
    private function initialise_install() {

    }

    /**
     * Define CaMykS and website required URLs.
     * @return void
     */
    private function define_URLs() {
        /* Define website URL */
        $this->websiteURL = $this->get_configValue('Website_URL');
        if ($this->websiteURL === false) {
            $url = $_SERVER['REQUEST_URI'];
            $this->websiteURL = parse_url($url, PHP_URL_SCHEME).'://'.parse_url($url, PHP_URL_HOST);
        }

        /* Check website installation mode */
        $installationMode = $this->get_configValue('Website_CaMykSMode');

        if ($installationMode == 'single') {
            /* Website is installed in single mode, files are directly accessible */
            $this->CaMykSAssetsURL = $this->get_configValue('Website_CaMykSPath').'/'.$this->CaMykSAssetsPath;
            $this->CaMykSPluginsURL = $this->get_configValue('Website_CaMykSPath').'/'.$this->CaMykSPluginsPath;
        } elseif ($installationMode == 'shared') {
            /* Website is installed in shared mode, files are accessible through folder sim links or request mode */
            $this->CaMykSAssetsURL = 'SharedAssets/'.$this->CaMykSAssetsPath;
            $this->CaMykSPluginsURL = 'SharedPlugins/'.$this->CaMykSPluginsPath;
        } else {
            /* No installation mode defined, CaMykS is in Installation or Manager mode */
            $this->CaMykSAssetsURL = '/'.$this->CaMykSAssetsPath;
            $this->CaMykSPluginsURL = '/'.$this->CaMykSPluginsPath;
        }
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
        if (isset($this->websiteConfiguration[$name]))
            return $this->websiteConfiguration[$name];

        /* Check CaMykS configuration */
        if (isset($this->engineConfiguration[$name]))
            return $this->engineConfiguration[$name];

        /* Return default value */
        return $default;
    }

    /* Library related methods */

    /**
     * Load given library.
     * @param string $libraryPath
     * @return boolean success
     */
    public function load_library($libraryPath) {
        /* Define file */
        $file = '/Core/Libraries/'.$libraryPath.'.php.inc';

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
        $this->load_library('Tools/array');
        $this->load_library('Tools/country');
        $this->load_library('Tools/currency');
        $this->load_library('Tools/html');
        $this->load_library('Tools/language');

        /* Load parent object library */
        $this->load_library('Objects/CLibrary');

        /* Load object libraries */
        $this->load_library('Objects/CBench');
        $this->load_library('Objects/CDatabaseConnector');
        $this->load_library('Objects/CDatabaseDriverInterface');
        $this->load_library('Objects/CQuery');
        $this->load_library('Objects/Plugin');

        /* Load site libraries */
        if ($this->websitePath != $this->CaMykSPath and file_exists($this->websitePath.'./Assets/Libraries')) {
            foreach (glob ($this->websitePath.'./Assets/Libraries/*'.$this->libraryExtension) as $file)
                require_once($file);
        }

        /* Clean captured extra code in live mode */
        if ($this->get_configValue('Website_Status') == false)
            ob_clean();
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
        switch ($mode) {
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
     * @return string
     */
    public function get_assetFileURL($asset, $URLMode=false) {
        /* Define file */
        $file = $this->CaMykSAssetsPath.$asset;

        /* Check file */
        if ($this->check_engineFileExists($file))
            return $this->get_baseURL($URLMode).$file;

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
