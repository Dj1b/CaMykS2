<?php
/**
 * @brief Plugin object.
 * <br />Parent object for Component, Module, Template, Theme.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre10
 * @date Creation: Nov 2019
 * @date Modification: Jul 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Plugin class.
 */
abstract class Plugin extends CaMykSLibrary {
    /**
     * var string $pluginStatus
     * @brief Plugin status.
     */
    protected $pluginStatus = 'initiated';

    /**
     * @var string $pluginVersion
     * @brief Plugin version.
     */
    protected $pluginVersion = '1.0';

    /**
     * @var string $pluginType
     * @brief Plugin type, values are "Component", "Module", "Special", "Template", "Theme".
     */
    protected $pluginType;

    /**
     * @var string $pluginLocation
     * @brief Plugin location, values are "Website", "Shared", "System".
     */
    protected $pluginLocation;

    /**
     * @var string $pluginPackage
     * @brief Plugin package.
     */
    protected $pluginPackage;

    /**
     * @var string $pluginFolder
     * @brief Plugin folder path.
     */
    protected $pluginFolder;

    /**
     * @var string $pluginPath
     * @brief Plugin root path.
     */
    protected $pluginPath;

    /**
     * @var array $pluginDependencies
     * @brief Plugin list of dependencies, including version and mandatoriness.
     */
    protected $pluginDependencies = array();

    /**
     * @var array $pluginExtensions
     * @brief List of plugin which it provides an extension.
     */
    protected $pluginExtensions = array();

    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected $hasConfiguration = false;

    /**
     * @var string $selectedLayout
     * @brief Currently selected layout to be displayed in display method.
     */
    protected $selectedLayout = '';

    /**
     * @var array $styleFiles.
     * @brief List of styles to be attached in site mode.
     * @note Style files are indicated like "file.js" => "media and condition"
     * @note Optionaly, version can be added to file name like "file.js?v=1.0.0"
     */
    protected $styleFiles = array();

    /**
     * @var array $adminStyleFiles.
     * @brief List of styles to be attached in admin mode.
     * @note Style files are indicated like "file.js" => "media and condition"
     * @note Optionaly, version can be added to file name like "file.js?v=1.0.0"
     */
    protected $adminStyleFiles = array();

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location=null) {
        parent::__construct();

        /* Save plugin location */
        $this->pluginLocation = $location;
    }

    /* System methods */

    /**
     * Load plugin.
     * @return void
     */
    protected function load() {
        /* Check is already loaded */
        if ($this->pluginStatus === 'loaded')
            return;

        /* Set location and path */
        $this->define_locationAndPath();

        /* Check plugin dependencies */
        $this->check_dependencies();

        /* Load plugin data */
        $this->load_data();

        /* Set module as loaded */
        $this->pluginStatus = 'loaded';
    }

    /**
     * Load plugin data.
     * @return void
     */
    protected function load_data() {
        /* To be override by PluginType libraries or plugin themselves */
    }

    /* Main methods */

    /**
     * Initialise plugin.
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        /* Load plugin */
        $this->load();

        /* Merge params */
        $this->update_params($params);
    }

    /**
     * Display plugin.
     * @return boolean success
     */
    public function display() {
        return $this->load_layout($this->selectedLayout);
    }

    /* Location related methods */

    /**
     * Set up plugin location value and path.
     * @return void
     */
    private function define_locationAndPath() {
        global $CaMykS;

        /* Load type folder */
        $typeFolder = $this->get_typeFolder();

        /* Define plugin folder */
        $this->pluginFolder = 'Plugins/'.$typeFolder.'/'.$this->libraryName.'/';

        /* Search plugin if location is not defined */
        if (is_null($this->pluginLocation)) {
            if (__FILE__ == $CaMykS->get_engineFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.CAMYKS_LIBRARYEXTENSION))
                $this->pluginLocation = 'Shared';
            elseif (__FILE__ == $CaMykS->get_engineFileFullPath('Core/Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.CAMYKS_LIBRARYEXTENSION))
                $this->pluginLocation = 'System';
            elseif ($CaMykS->get_websiteFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.CAMYKS_LIBRARYEXTENSION))
                $this->pluginLocation = 'Website';
            else {
                /* Plugin not found throw fatal error */
                $camyks->trigger_error('Core', 0, 'Plugin '.$this->libraryName.' not found.', '', __FILE__, __LINE__, true);
            }
        }

        /* Define plugin path */
        switch ($this->pluginLocation) {
            case 'Shared'   : $this->pluginPath = $CaMykS->get_engineFileFullPath($this->pluginFolder); break;
            case 'System'   : $this->pluginPath = $CaMykS->get_engineFileFullPath('Core/'.$this->pluginFolder); break;
            case 'Website'  : $this->pluginPath = $CaMykS->get_websiteFileFullPath($this->pluginFolder); break;
        }
    }

    /**
     * Return plugin type folder.
     * @return string
     */
    protected function get_typeFolder() {
        return ucwords(strtolower($this->pluginType.'s'));
    }

    /* Data related methods */

    /**
     * Return plugin version.
     * @return string
     */
    public function get_version() {
        return $this->pluginVersion;
    }

    /**
     * Return plugin package.
     * @return string
     */
    public function get_package() {
        return $this->pluginPackage;
    }

    /* Libraries related methods */

    /**
     * Load library file.
     * @param string $library
     * @return boolean success
     */
    public function load_library($library='') {
        global $CaMykS;

        /* build file path */
        $filePath = $this->pluginPath.'/Libraries/'.$library.CAMYKS_LIBRARYEXTENSION;

        /* load file if exists */
        if (file_exists($filePath))
            return ((require_once($filePath)) == true);
        return false;
    }

    /* Configuration related methods */

    /**
     * Check plugin has a configuration.
     * @return boolean result
     */
    public function has_configuration() {
        return $this->hasConfiguration;
    }

    /**
     * Return configuration value.
     * @param string $name
     * @param mixed $default
     * @return mixed
     */
    public function get_configValue($name, $default=null) {
        global $CaMykS;

        /* Check for plugin configuration */
        if ($this->has_configuration()) {
            /* Load configuration if required */
            if ($this->configuration === null)
                $this->configuration->load();

            /* Load configuration */
            $value = $this->configuration->get_value($name, $default);

            /* Return value if defined */
            if ($value !== null)
                return $value;
        }

        /* Return configuration value from CaMykS */
        return $CaMykS->get_configValue($name, $default);
    }

    /* Management related methods */

    /* Extensions related methods */

    /* Dependencies related methods */

    /**
     * Check Plugin dependencies.
     * @return @todo
     */
    protected function check_dependencies() {

    }

    /* Request mode methods */

    /* Locales related methods */

    /**
     * Check if locale exists.
     * @param string $locale
     * @return boolean result
     */
    public function check_locale($locale='') {
        global $CaMykS;

        /* Check locale in CaMykS object */
        return $CaMykS->check_locale($locale, $this->libraryName);
    }

    /**
     * Return locale.
     * @param string $locale
     * @param mixed $data
     * @return string
     */
    public function get_locale($locale='', $data=null) {
        global $CaMykS;

        /* Return locale from CaMykS object */
        return $CaMykS->get_locale($locale, $this->libraryName, $data);
    }

    /**
     * Load library locales.
     * @param string $folder
     * @param string $language
     * @return boolean success
     */
    public function load_locales($folder='', $language=null) {
        global $CaMykS;

        /* Load locales in plugin folder */
        return $CaMykS->load_locales($this->pluginPath.'Locales'.($folder != ''? '/'.$folder : ''), $this->libraryName, $language);
    }

    /**
     * Return available locale languages for plugin.
     * @param string $folder
     * @return array
     */
    public function get_availableLocaleLanguages($folder='') {
        /* Load file list */
        $files = glob($this->pluginPath.'Locales'.($folder != ''? '/'.$folder : '').'/*');

        /* Initialise result list */
        $result = array();

        /* Add languages to list */
        foreach ($files as $file) {
            $file = basename($file);
            if (preg_match('/Locales.(.*).php.inc/', $file, $match))
                $result[] = $match[1];
        }

        /* Return result */
        return $result;
    }

    /* Files related methods */

    /**
     * Return file path, allowing file overriding.
     * @param string $filePath
     * @return string
     */
    public function get_filePath($filePath) {
        global $CaMykS;

        /* Complete file path */
        $filePath = $this->pluginFolder.$filePath;

        /* Remove QueryString for versioned files */
        $f = explode('?', $filePath);
        $filePath = $f[0];

        /* Check in website folder */
        if ($CaMykS->check_websiteFileExists($filePath))
            return $CaMykS->get_websiteFileFullPath($filePath);

        /* Check plugin is located in website folder */
        if ($this->pluginLocation == 'Website')
            return false;

        /* Check in shared folder */
        if ($CaMykS->check_engineFileExists($filePath))
            return $CaMykS->get_engineFileFullPath($filePath);

        /* Check plugin is located in shared folder */
        if ($this->pluginLocation == 'Shared')
            return false;

        /* Check in system folder */
        if ($CaMykS->check_engineFileExists('Core/'.$filePath))
            return $CaMykS->get_engineFileFullPath('Core/'.$filePath);

        /* File not found, return default value */
        return '';
    }

    /**
     * Return file URL, allowing file overriding.
     * @param string $filePath
     * @param string $URLMode
     * @return string
     */
    public function get_fileURL($filePath, $URLMode=null) {
        global $CaMykS;

        /* Complete file path */
        $filePath = $this->pluginFolder.$filePath;

        /* Keep back QueryString for versioned files */
        $f = explode('?', $filePath);
        if (count($f) == 2) {
            $filePath = $f[0];
            $queryString = '?'.$f[1];
        } else {
            $queryString = '';
        }

        /* Check in website folder */
        if ($CaMykS->check_websiteFileExists($filePath))
            return $CaMykS->get_websiteFileURL($filePath.$queryString, $URLMode);

        /* Check plugin is located in website folder */
        if ($this->pluginLocation == 'Website')
            return false;

        /* Check in shared folder */
        if ($CaMykS->check_engineFileExists($filePath))
            return $CaMykS->get_sharedPluginFileURL($filePath.$queryString, $URLMode);

        /* Check plugin is located in shared folder */
        if ($this->pluginLocation == 'Shared')
            return false;

        /* Check in system folder */
        if ($CaMykS->check_engineFileExists('Core/'.$filePath))
            return $CaMykS->get_corePluginFileURL($filePath.$queryString, $URLMode);

        /* File not found, return default value */
        return '';
    }

    /**
     * Load file.
     * @param string $filePath
     * @return boolean result
     */
    public function load_file($filePath) {
        $filePath = $this->get_filePath($filePath);
        if ($filePath == '')
            return false;
        require $filePath;
        return true;
    }

    /* Layout related methods */

    /**
     * Update selected layout.
     * @param string $layout
     * @return void
     */
    protected function update_selectedLayout($layout='') {
        $this->selectedLayout = $layout;
    }

    /**
     * Load layout file.
     * @param string $file
     * @return boolean result
     */
    protected function load_layout($file) {
        return $this->load_file('Layouts/'.$file.CAMYKS_LAYOUTEXTENSION);
    }

    /* Javascript related methods */

    /**
     * Attach a javascript file to HTML page.
     * @param string $file
     * @param string $params
     * @param string $URLMode
     * @return void
     */
    protected function add_JSFile($file, $params=array(), $URLMode=null) {
        global $CaMykS;
        $CaMykS->HTMLPage->add_JSFile($this->get_fileURL('Scripts/'.$file, $URLMode), $params);
    }

    /**
     * Add a script to HTML page.
     * @param string $script
     * @param boolean $unique
     * @param string $event
     * @return void
     */
    public function add_JSScript($script='', $unique=false, $event='Main') {
        global $CaMykS;
        $CaMykS->HTMLPage->add_JSScript($script, $unique, $event);
    }

    /* Styles related methods */

    /**
     * Load generic styles files.
     * @return void
     */
    public function load_styleFiles() {
        /* Attach style files to HTML page */
        foreach ($this->styleFiles as $file => $condition)
            $this->add_styleFile($file, $condition);
    }

    /**
     * Load admin specific styles files.
     * @return void
     */
    public function load_adminStyleFiles($mode = 'Attached') {
        if ($mode === 'Attached') {
            /* Attach admin style files to HTML page */
            foreach ($this->adminStyleFiles as $file => $condition)
                $this->add_styleFile($file, $condition);
        } elseif ($mode === 'Inserted') {
            /* Insert admin style files to HTML page */
            foreach ($this->adminStyleFiles as $file => $condition)
                $this->add_styleIncludedFile($file, $condition);
        }
    }

    /**
     * Attach a style file to HTML page.
     * @param string $file
     * @param string $condition
     * @param string $URLMode
     * @return void
     */
    protected function add_styleFile($file, $condition='', $URLMode=null) {
        global $CaMykS;
        $CaMykS->HTMLPage->add_styleFile($this->get_fileURL('Styles/'.$file, $URLMode), $condition);
    }

    /**
     * Insert a style file to HTML page.
     * @param string $file
     * @param string $condition
     * @param string $URLMode
     * @return void
     */
    protected function add_styleIncludedFile($file, $condition='', $URLMode=null) {
        global $CaMykS;
        $CaMykS->HTMLPage->add_styleIncludedFile($this->get_filePath('Styles/'.$file, $URLMode), $condition);
    }

    /**
     * Add a style script to HTML page.
     * @param string $script
     * @param string $condition
     * @return void
     */
    public function add_styleScript($script='', $condition='') {
        global $CaMykS;
        $CaMykS->HTMLPage->add_styleScript($script, $condition);
    }

    /* CaMykS events related methods */

    /* CaMykS page related methods */
}
?>
