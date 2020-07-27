<?php
/**
 * @brief Plugin object.
 * <br />Parent object for Module, Template, Theme, Component, extends CaMykSLibrary.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.5.1
 * @date Creation: Nov 2019
 * @date Modification: Jul 2020
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Plugin class.
 */
abstract class Plugin extends CaMykSLibrary {
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
     * @brief Plugin location, values are "Website", "Shared", 'System".
     */
    protected $pluginLocation;

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
     * @brief List of styles to be attached.
     * @note Style files are indicated like "file.js" => "media and condition"
     */
    protected $styleFiles = array();

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location=null) {
        parent::__construct();

        /* Set location and path */
        $this->define_locationAndPath($location);

        /* Check dependencies */

        /* Declare extensions */

    }

    /* Main methods */

    /**
     * Prepare plugin.
     * @return boolean success
     */
    public function prepare() {
        return true;
    }

    /**
     * Initialise plugin.
     * @param array $params
     * @return boolean success
     */
    public function initialise($params=array()) {
        /* Merge params */
        $this->update_params($params);

        /* Attach style files to HTML page */
        foreach ($this->styleFiles as $file => $condition)
            $this->add_styleFile($file, $condition);

        /* Return success */
        return true;
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
     * @param string $location
     * @return void
     */
    private function define_locationAndPath($location=null) {
        global $CaMykS;

        /* Load type folder */
        $typeFolder = $this->get_typeFolder();

        /* Define plugin folder */
        $this->pluginFolder = 'Plugins/'.$typeFolder.'/'.$this->libraryName.'/';

        /* Check is in shared plugins */
        if ($location == 'Shared' or ($location === null and __FILE__ == $CaMykS->get_engineFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'Shared';
            $this->pluginPath = $CaMykS->get_engineFileFullPath($this->pluginFolder);
        }

        /* Check is in system plugins */
        elseif ($location == 'System' or ($location === null and __FILE__ == $CaMykS->get_engineFileFullPath('Core/Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'System';
            $this->pluginPath = $CaMykS->get_engineFileFullPath('Core/'.$this->pluginFolder);
        }

        /* Check is in website plugins */
        elseif ($location == 'Website' or ($location === null and __FILE__ == $CaMykS->get_websiteFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'Website';
            $this->pluginPath = $CaMykS->get_websiteFileFullPath($this->pluginFolder);
        }

        /* Plugin not found */
        else {
            /* @todo */
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
        return $this->version;
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
        $filePath = $this->pluginPath.'/Libraries/'.$library.$CaMykS->libraryExtension;

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
    protected function load_locales($folder='', $language=null) {
        global $CaMykS;

        /* Load locales in plugin folder */
        return $CaMykS->load_locales($this->pluginPath.'/Locales'.($folder != ''? '/'.$folder : ''), $this->libraryName, $language);
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

        /* Check in website folder */
        if ($CaMykS->check_websiteFileExists($filePath))
            return $CaMykS->get_websiteFileURL($filePath, $URLMode);

        /* Check plugin is located in website folder */
        if ($this->pluginLocation == 'Website')
            return false;

        /* Check in shared folder */
        if ($CaMykS->check_engineFileExists($filePath))
            return $CaMykS->get_sharedPluginFileURL($filePath, $URLMode);

        /* Check plugin is located in shared folder */
        if ($this->pluginLocation == 'Shared')
            return false;

        /* Check in system folder */
        if ($CaMykS->check_engineFileExists('Core/'.$filePath))
            return $CaMykS->get_corePluginFileURL($filePath, $URLMode);

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
        global $CaMykS;

        return $this->load_file('Layouts/'.$file.$CaMykS->layoutExtension);
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
