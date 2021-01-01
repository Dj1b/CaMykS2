<?php
/**
 * @brief CaMykS version 2, CMS master class.
 * @details Engine / Master Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0pre17
 * @date Creation: Feb 2017
 * @date Modification: Dec 2020
 * @copyright 2017 - 2020 CaMykS Team
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
     * @brief Declare CaMykS variables that are set in CaMykS configuration file.
     */
    protected $name, $version, $versionMajor, $versionMinor, $lastModificationDate, $PHPMinVersion, $libraryExtension, $layoutExtension, $CMSWebsiteURL, $defaultLanguage;

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
    private $currentLanguage = 'fr_FR';

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
     * @var array $accessibilityFolders
     * @brief List of accessible folders.
     */
    private $accessibilityFolders;

    /**
     * @var boolean $falseRef
     * @brief CaMykS false value as reference.
     */
    private $falseRef = false;
    
    /* URLs and paths */

    /**
     * @var string $CaMykSPath
     * @brief CaMykS path.
     */
    private $CaMykSPath;

    /**
     * @var string $websitePath
     * @brief Website path.
     */
    private $websitePath;

    /**
     * @var string $websiteURL
     * @brief Website URL.
     */
    private $websiteURL;

    /**
     * @var string $corePluginLink
     * @brief Symbolic link to core plugin folder
     */
    private $corePluginLink = 'CorePlugins';

    /**
     * @var string $sharedPluginLink
     * @brief Symbolic link to shared plugin folder
     */
    private $sharedPluginLink = 'SharedPlugins';

    /**
     * @var string $coreAssetLink
     * @brief Symbolic link to core asset folder
     */
    private $coreAssetLink = 'CoreAssets';
    
    /* Links to internal object libraries */

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
     * @var Object $HTMLPage
     * @brief Main HTML page.
     */
    private $HTMLPage;
    
    /**
     * @var Object $localStorage
     * @brief Local storage object.
     */
    private $localStorage;
    
    /* Links to plugins */

    /**
     * @var Object $currentTheme
     * @brief Link to current theme used in CaMykS.
     */
    private $currentTheme;

    /**
     * @var Object $currentTemplate
     * @brief Link to current template used in CaMykS.
     */
    private $currentTemplate;

    /**
     * @var Object $manager
     * @brief Link to manager special plugin.
     */
    private $manager;

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

    /* CaMykS magic methods */

    /**
     * Makes some variables accessible in read-only
     * @param string $variableName
     * @return mixed
     */
    public function __get($variableName) {
        if (in_array($variableName, array('name', 'mode', 'version', 'versionMajor', 'versionMinor', 'lastModificationDate', 'CMSWebsiteURL', 'currentLanguage', 'currentTheme', 'currentTemplate', 'HTMLPage', 'libraryExtension', 'layoutExtension', 'manager')))
            return $this->$variableName;
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
        //$this->close();
    }

    /* CaMykS main sub methods */

    /**
     * Initialise CaMykS.
     * @return void
     */
    private function initialise() {
        /* Initialise engine */
        $this->initialise_engine();

        /* Run onInitialise event handlers */
        $this->run_eventHandlers('onInitialise');

        /* Add bench step */
        $this->bench->add_step('CaMykS initialise mode.');

        /* Create HTML page */
        $this->HTMLPage = new HTMLPage();

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
    }

    /**
     * Display CaMykS.
     * @return void
     */
    private function display() {

        /* Add bench step */
        $this->bench->add_step('CaMykS display mode.');

        /* Write HTML page header */
        $this->HTMLPage->write_header();

        /* Display mode */
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
        die();
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

        /* Write HTML page footer */
        $this->HTMLPage->write_footer();
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

        /* Load first libraries */
        $this->load_engineFirstLibraries();

        /* Load engine configuration */
        $this->load_configuration('Engine');

        /* Define time zone */
        date_default_timezone_set($this->get_configValue('Engine_TimeZone', 'Europe/Paris'));

        /* Define encoding */
        $this->define_encoding();

        /* Load libraries */
        $this->load_engineLibraries();

        /* Load errorhandler object */
        $this->errorHandler = new ErrorHandler();

        /* Check server requirements */
        $this->check_serverRequirements();

        /* Load bench object */
        $this->bench = new Bench();
        $this->bench->start('CaMykS loading.', $this->startMicroTime);
        unset($this->startMicroTime);
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


        /* Run onVisitPageLoad event handlers */
        $this->run_eventHandlers('onVisitPageLoad');
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

        /* Load admin locales */
        $this->load_engineLocales('Admin', 'Admin', $this->currentLanguage);



        /* Run onAdminPageLoad event handlers */
        $this->run_eventHandlers('onAdminPageLoad');
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

        /* Update folder links */
        $this->corePluginLink = 'Core';
        $this->sharedPluginLink = '';
        $this->coreAssetLink = '';

        /* Load language */
        $this->load_currentLanguage();

        /* Load manager plugin */
        $this->manager = &$this->get_plugin('CaMykSManager', 'Special', true, 'System');

        /* Load template */
        $this->currentTemplate = &$this->get_template('AdminPage', true, 'System');
        $this->currentTemplate->initialise();

        /* Load theme */
        $this->currentTheme = &$this->get_theme($this->get_configValue('Manager_Theme'), true);
        $this->currentTheme->initialise();
        $this->currentTheme->load_adminStyleFiles();

        /* Initialise manager */
        $this->manager->initialise();
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
        /* Run onVisitBodyOpen event handlers */
        $this->run_eventHandlers('onVisitBodyOpen');

        /* Display template main layout */
        $this->currentTemplate->display();

        /* Run onVisitBodyClose event handlers */
        $this->run_eventHandlers('onVisitBodyClose');
    }

    /**
     * Display admin mode.
     * @return void
     */
    private function display_admin() {
        /* Run onAdminBodyOpen event handlers */
        $this->run_eventHandlers('onAdminBodyOpen');

        /* Display template main layout */
        $this->currentTemplate->display();

        /* Run onAdminBodyClose event handlers */
        $this->run_eventHandlers('onAdminBodyClose');
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
        /* Display template main layout */
        $this->currentTemplate->display();
    }

    /**
     * Display install mode.
     * @return void
     */
    private function display_install() {

    }

    /* CaMykS display sub methods */

    /**
     * Display template slot content.
     * @param integer $slotIndex
     * @return void
     */
    public function display_templateSlotContent($slotIndex) {
        /* Display mode */
        switch ($this->mode) {
            case CAMYKS_MODE_VISIT:
                /* Display module */

                break;
            case CAMYKS_MODE_ADMIN:
                /* Display module in admin module */

                break;
            case CAMYKS_MODE_REQUEST:
                /* */

                break;
            case CAMYKS_MODE_MANAGER:
                /* Display manager */
                $this->manager->display();

                break;
            case CAMYKS_MODE_INSTALL:
                /* Display install */

                break;
        }
    }

    /* Configuration related methods */

    /**
     * Load configuration.
     * @param string $configFile
     * @return void
     */
    private function load_configuration($configFile) {
        $config = new ConfigurationFile();

        /* Load engine configuration file */
        $this->engineConfiguration = array_merge($this->engineConfiguration, $config->load($configFile, 'Engine'));

        /* Load website configuration file */
        $this->websiteConfiguration = array_merge($this->websiteConfiguration, $config->load($configFile, 'Website'));
    }

    /**
     * Return configuration value.
     * @param string $name
     * @param mixed $default
     * @return mixed
     */
    public function get_configValue($name, $default=null) {
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

    /* Session related method */

    /**
     * Save variable to session.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function set_sessionValue($name, $value='') {
        $_SESSION[$this->get_configValue('Website_Name').'_'.$name] = $value;
    }

    /**
     * Return variable from session.
     * @param string $name
     * @param mixed $default
     * @return mixed
     */
    public function get_sessionValue($name, $default=null) {
        /* Set up session variable */
        $var = $this->get_configValue('Website_Name').'_'.$name;

        /* Return value */
        return isset($_SESSION[$var]) ? $_SESSION[$var] : $default;
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
    private function load_engineFirstLibraries() {
        $this->load_library('ConfigurationFile', 'Objects');
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
        $this->load_library('client', 'Tools');
        $this->load_library('country', 'Tools');
        $this->load_library('currency', 'Tools');
        $this->load_library('date', 'Tools');
        $this->load_library('file', 'Tools');
        $this->load_library('folder', 'Tools');
        $this->load_library('html', 'Tools');
        $this->load_library('http', 'Tools');
        $this->load_library('language', 'Tools');
        $this->load_library('password', 'Tools');
        $this->load_library('server', 'Tools');
        $this->load_library('string', 'Tools');
        $this->load_library('util', 'Tools');

        /* Load parent object libraries */
        $this->load_library('CaMykSLibrary', 'Objects');
        $this->load_library('Plugin', 'Objects');

        /* Load object libraries */
        $this->load_library('Bench', 'Objects');
        $this->load_library('DatabaseConnector', 'Objects');
        $this->load_library('DatabaseDriverInterface', 'Objects');
        $this->load_library('DbQuery', 'Objects');
        $this->load_library('Component', 'Objects');
        $this->load_library('Error', 'Objects');
        $this->load_library('ErrorHandler', 'Objects');
        $this->load_library('File', 'Objects');
        $this->load_library('HTMLForm', 'Objects');
        $this->load_library('HTMLFormInput', 'Objects');
        $this->load_library('HTMLPage', 'Objects');
        $this->load_library('LocalStorage', 'Objects');
        $this->load_library('Module', 'Objects');
        $this->load_library('RequestAnswer', 'Objects');
        $this->load_library('Template', 'Objects');
        $this->load_library('Theme', 'Objects');
        
        /* Build object from libraries */
        $this->localStorage = new LocalStorage();

        /* Load site libraries */
        if ($this->websitePath != $this->CaMykSPath and file_exists($this->websitePath.'./Assets/Libraries')) {
            foreach (glob($this->websitePath.'./Assets/Libraries/*'.$this->libraryExtension) as $file)
                require_once($file);
        }

        /* Clean captured extra code in live mode */
        if ($this->get_configValue('Website_Status') == false)
            ob_clean();
    }

    /* Initialisation sub methods */

    /**
     * Define encoding.
     * @return void
     */
    private function define_encoding() {
        if (extension_loaded('mbstring'))
            mb_internal_encoding($this->get_configValue('Website_Encoding', 'UTF8'));
    }

    /**
     * Check server requirements.
     * @return void
     */
    private function check_serverRequirements() {
        /* Check PHP version */
        if (version_compare(PHP_VERSION, $this->PHPMinVersion, '<')) {
            $this->trigger_error('Fatal', null, 'Current PHP version ('.PHP_VERSION.') doesn\'t meet minimum requirements.'."\n".
            'To run CaMykS2 properly, PHP engine must be, at least, version '.$this->PHPMinVersion.".\n".
            'Please update your version of PHP, before running CaMykS2.',
            '', null, null, true);
        }

        /* Check mod_rewrite module */
        if (!server_checkModuleAvailability('mod_rewrite')) {
            $this->trigger_error('Fatal', null, 'URL rewriting module must be enabled.'."\n".
            'Please turn on the URL rewriting module of your web software, before running CaMykS2.',
            '', null, null, true);
        }
    }

    /* Plugin related methods */

    /**
     * Load plugin.
     * @param string $name
     * @param string $type
     * @param string $location
     * @return boolean result
     */
    public function load_plugin($name, $type, $location=null) {
        /* Define plugins folder */
        $folder = ucwords(strtolower($type)).'s';

        /* Check is already loaded */
        if (isset($this->plugins[$folder][$name]))
            return true;

        /* Check is in system plugins */
        if ($location == 'System' or ($location === null and $this->check_engineFileExists('Core/Plugins/'.$folder.'/'.$name.'/'.$name.$this->libraryExtension))) {
            $location = 'System';
            $path = $this->get_engineFileFullPath('Core/Plugins/'.$folder.'/'.$name.'/');
        }

        /* Check is in shared plugins */
        elseif ($location == 'Shared' or ($location === null and $this->check_engineFileExists('Plugins/'.$folder.'/'.$name.'/'.$name.$this->libraryExtension))) {
            $location = 'Shared';
            $path = $CaMykS->get_engineFileFullPath('Plugins/'.$folder.'/'.$name.'/');
        }

        /* Check is in website plugins */
        elseif ($location == 'Website' or ($location === null and  $this->check_websiteFileExists('Plugins/'.$folder.'/'.$name.'/'.$name.$this->libraryExtension))) {
            $location = 'Website';
            $path = $CaMykS->get_websiteFileFullPath('Plugins/'.$folder.'/'.$name.'/');
        }

        /* Plugin not found */
        else {
            /* Log error ? */

            /* Return failure */
            return false;
        }

        /* Load plugin */
        try {
            require_once $path.$name.$this->libraryExtension;
            $class = 'CaMykS2\\'.$name;
            $this->plugins[$folder][$name] = new $class($location);

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
    public function load_component($name, $location=null) {
        return $this->load_plugin($name, 'Component', $location);
    }

    /**
     * Load module.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_module($name, $location=null) {
        return $this->load_plugin($name, 'Module', $location);
    }

    /**
     * Load template.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_template($name, $location=null) {
        return $this->load_plugin($name, 'Template', $location);
    }

    /**
     * Load theme.
     * @param string $name
     * @param string $location
     * @return boolean result
     */
    public function load_theme($name, $location=null) {
        return $this->load_plugin($name, 'Theme', $location);
    }

    /**
     * Return link to plugin object.
     * @param string $name
     * @param string $type
     * @param boolean $autoLoad
     * @param string $location
     * @return Plugin
     */
    public function &get_plugin($name, $type, $autoLoad=false, $location=null) {

        /* Define plugins folder */
        $folder = ucwords(strtolower($type)).'s';

        /* Check is in loaded plugin */
        if (isset($this->plugins[$folder][$name]))
            return $this->plugins[$folder][$name];

        /* Try to load plugin */
        if ($autoLoad and $this->load_plugin($name, $type, $location))
            return $this->plugins[$folder][$name];

        /* Return failure */
        return $this->falseRef;
    }

    /**
     * Return link to component object.
     * @param string $name
     * @param boolean $autoLoad
     * @param string $location
     * @return Input
     */
    public function &get_component($name, $autoLoad=false, $location=null) {
        return $this->get_plugin($name, 'Component', $autoLoad, $location);
    }

    /**
     * Return link to module object.
     * @param string $name
     * @param boolean $autoLoad
     * @param string $location
     * @return Module
     */
    public function &get_module($name, $autoLoad=false, $location=null) {
        return $this->get_plugin($name, 'Module', $autoLoad, $location);
    }

    /**
     * Return link to theme object.
     * @param string $name
     * @param boolean $autoLoad
     * @param string $location
     * @return Theme
     */
    public function &get_theme($name, $autoLoad=false, $location=null) {
        return $this->get_plugin($name, 'Theme', $autoLoad, $location);
    }

    /**
     * Return link to template object.
     * @param string $name
     * @param boolean $autoLoad
     * @param string $location
     * @return Template
     */
    public function &get_template($name, $autoLoad=false, $location=null) {
        return $this->get_plugin($name, 'Template', $autoLoad, $location);
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
     * Check if given engine file exists.
     * @param string $filePath
     * @return boolean result
     */
    public function check_engineFileExists($filePath) {
        return file_exists($this->get_engineFileFullPath($filePath));
    }

    /**
     * Return engine file full path.
     * @param string $filePath
     * @return string
     */
    public function get_engineFileFullPath($filePath) {
        return $this->CaMykSPath.'/'.$filePath;
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
     * Return website file full path.
     * @param string $filePath
     * @return string
     */
    public function get_websiteFileFullPath($filePath) {
        return $this->websitePath.'/'.$filePath;
    }

    /**
     * Check if given path is autorised to be read or written.
     * @param string $path
     * @param string $action
     * @return boolean result
     */
    public function check_pathAccessibility($path, $action='Read') {
        /* Check action */
        if (!in_array($action, array('Read', 'Write', 'Delete')))
            return false;

        /* Load folders */
        if (is_null($this->accessibilityFolders))
            $this->load_accessibilityFolders();

        /* Check acessiblity */
        foreach ($this->accessibilityFolders as $folder => $permissions) {
            /* Check permission is available */
            if (!in_array($action, $permissions))
                continue;

            /* Check is in path */
            if (file_checkIsInPath($path, $folder))
                return true;
        }

        /* Return default value */
        return false;
    }

    /**
     * Load accessibility folders.
     * @return void
     */
    private function load_accessibilityFolders() {
        /* Build folder */
        $this->accessibilityFolders = array(
            /* Read permission on website root */
            $this->get_websiteFileFullPath('') => array('Read'),

            /* Read/write permission on website configuration */
            $this->get_websiteFileFullPath('Configuration') => array('Read', 'Write'),

            /* Read/write permission on website data */
            $this->get_websiteFileFullPath('Data') => array('Read', 'Write', 'Delete'),

            /* Read permission on CaMykS root */
            $this->get_engineFileFullPath('') => array('Read'),

            /* Read/write permission on tmp folder */
            folder_getTemporaryFolder() => array('Read', 'Write', 'Delete'),
        );

        /* Read/write permission on CaMykS configuration in manager mode */
        if ($this->mode === CAMYKS_MODE_MANAGER) {
            $this->accessibilityFolders[$this->get_engineFileFullPath('Core/Configuration')] = array('Read', 'Write');
            $this->accessibilityFolders[$this->get_configValue('Manager_WebsitesPath')] = array('Read', 'Write');
        }
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
    public function get_baseURL($URLMode=null) {
        /* Check for website default URL mode */
        if ($URLMode == null)
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
     * Return website file URL.
     * @param string $filePath
     * @param string $URLMode
     * @return string
     */
    public function get_websiteFileURL($filePath, $URLMode=null) {
        return $this->get_baseURL($URLMode).$filePath;
    }

    /* Plugin related files */

    /**
     * Return shared plugin file URL.
     * @param string $filePath
     * @param string $URLMode
     * @return string
     */
    public function get_sharedPluginFileURL($filePath, $URLMode=null) {
        return $this->get_baseURL($URLMode).($this->sharedPluginLink != '' ? $this->sharedPluginLink.'/' : '').$filePath;
    }

    /**
     * Return core plugin file URL.
     * @param string $filePath
     * @param string $URLMode
     * @return string
     */
    public function get_corePluginFileURL($filePath, $URLMode=null) {
        return $this->get_baseURL($URLMode).($this->corePluginLink != '' ? $this->corePluginLink.'/' : '').$filePath;
    }

    /* Assets related files */

    /**
     * Return asset file path.
     * @param string $asset
     * @return string
     */
    public function get_assetFilePath($asset) {
        /* Check file in website */
        $file = 'Assets/'.$asset;
        if ($this->mode < 3) {
            if (file_exists($file))
                return $file;
        }

        /* Check file in CaMykS */
        $file = 'Core/'.$file;
        if ($this->check_engineFileExists($file))
            return $this->get_engineFileFullPath($file);

        /* Default for file that doesn't exist */
        return '';
    }

    /**
     * Return asset file URL.
     * @param string $asset
     * @param string $URLMode
     * @return string
     */
    public function get_assetFileURL($asset, $URLMode=null) {
        /* Check file in website */
        $file = 'Assets/'.$asset;
        if ($this->mode < 3) {
            if (file_exists($file))
                return $this->get_baseURL($URLMode).$file;
        }

        /* Check file in CaMykS */
        $file = 'Core/'.$file;
        if ($this->check_engineFileExists($file))
            return $this->get_baseURL($URLMode).($this->coreAssetLink != '' ? $this->coreAssetLink.'/' : '').$file;

        /* Default for files that don't exists */
        return '';
    }

    /**
     * Load asset layout.
     * @param string $layout
     * @param mixed $context
     * @return string
     */
    public function load_assetLayout($layout, $context=null) {
        /* Save context */
        $this->layoutContext = $context;

        /* Check file in website */
        $file = 'Assets/Layouts/'.$layout.$this->layoutExtension;
        if ($this->mode < 3) {
            if (file_exists($file))
                include $file;
        }

        /* Check file in CaMykS */
        $file = 'Core/'.$file;
        if ($this->check_engineFileExists($file))
            include $file;

        /* Default for files that don't exists */
        return '';
    }

    /* Scripts assets related methods */

    /**
     * Load engine script.
     * @param string $file
     * @param string $folder
     * @param array $params
     * @param string $URLMode
     */
    public function load_engineJSScript($file, $folder, $params=array(), $URLMode=null) {
        /* Default params */
        $defaultParams = array('async'=>'');
        $params = array_merge($defaultParams, $params);

        /* Attach script to HTML Page */
        if (isset($this->HTMLPage))
            $this->HTMLPage->add_JSFile($this->get_assetFileURL('Scripts/'.$folder.'/'.$file, $URLMode), $params);
    }

    /**
     * Load CaMykS client-side object.
     * @return void
     */
    public function load_CaMykSJSScript() {
        /* Load object script */
        $this->load_engineJSScript('CaMykS.js', 'Objects');

        /* Add object initialisation */
        if (isset($this->HTMLPage))
            $this->HTMLPage->add_JSScript('CaMykS.initialise();', true, 'load');
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
        /* Do the check, with prefix */
        if ($prefix != '' and isset($this->locales[$prefix.'_'.$key]))
            return true;

        /* Do the check, without prefix */
        return isset($this->locales[$key]);
    }

    /**
     * Return locale string.
     * @param string $key
     * @param string $prefix
     * @param mixed $data
     * @param string $default
     * @return string
     */
    public function get_locale($key, $prefix='', $data=array(), $default=null) {
        /* Check data value */
        if (is_string($data))
            $data = array($data);
        elseif (is_null($data))
            $data = array();

        /* Search and return locale from key with prefix */
        if ($prefix != '' and isset($this->locales[$prefix.'_'.$key]))
            return vsprintf($this->locales[$prefix.'_'.$key], $data);

        /* Search and return locale from key without prefix */
        if (isset($this->locales[$key]))
            return vsprintf($this->locales[$key], $data);

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
     * @param string $language
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
     * Trigger an internal error.
     * @param string $origin
     * @param integer $code
     * @param string $message
     * @param string $details
     * @param string $file
     * @param integer $line
     * @param boolean $isFatal
     * @return boolean success
     */
    public function trigger_error($origin, $code, $message, $details, $file='', $line=0, $isFatal=false) {
        if ($this->errorHandler !== false)
            return $this->errorHandler->handle_CaMykSError($origin, $code, $message, $details, $file, $line, $isFatal);
        return false;
    }

    /**
     * Log a message.
     * @param string $object
     * @param string $action
     * @param string $message
     * @return boolean success
     */
    public function log_message($object, $action, $message='') {
        $logManager = $this->get_module('Admin_SystemLogManager', true);
        if ($logManager === false)
            return false;
        return $logManager->log_message($object, $action, $message);
    }

    /**
     * Log messages.
     * @param string $object
     * @param string $messages
     * @return boolean success
     */
    public function log_messages($object, $messages=array()) {
        $logManager = $this->get_module('Admin_SystemLogManager', true);
        if ($logManager === false)
            return false;
        return $logManager->log_messages($object, $messages);
    }

    /**
     * Log an error.
     * @param CaMykS2\Error $error
     * @return boolean success
     */
    public function log_error($error) {
        $logManager = $this->get_module('Admin_SystemLogManager', true);
        if ($logManager === false)
            return false;
        return $logManager->log_error($error);
    }

    /**
     * Display splash message.
     * @param string $layout
     * @param mixed $content
     * @param integer $HTTPCode
     * @return void
     */
    public function display_splashMessage($layout, $content='', $HTTPCode=200) {
        /* Send HTTP code */
        $this->HTMLPage->set_status($HTTPCode);

        /* Display page headers */
        $this->HTMLPage->add_styleIncludedFile($this->get_assetFilePath('Styles/Page.css'));
        $this->HTMLPage->write_header();

        /* Display page header */
        $this->load_assetLayout('Page/Header');

        /* Display error */
        $this->load_assetLayout($layout, $content);

        /* Display page footer */
        $this->load_assetLayout('Page/Footer');

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
