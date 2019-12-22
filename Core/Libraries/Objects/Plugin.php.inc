<?php
/**
 * @brief Plugin object.
 * <br />Parent object for Module, Template, Theme, Component, extends CaMykSLibrary.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
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
     * @brief Plugin type, values are "component", "module", "special", "template", "theme".
     */
    protected $pluginType;

    /**
     * @var string $pluginLocation
     * @brief Plugin location, values are "website", "shared", 'system".
     */
    protected $pluginLocation;

    /**
     * @var string $pluginPath
     * @brief Plugin root path.
     */
    protected $pluginPath;

    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected $hasConfiguration = false;

    /**
     * Class constructor.
     * @return void
     */
    public function __construct($type, $location=false) {
        parent::__construct();

        /* Set type */
        $this->pluginType = $type;

        /* Set location and path */
        $this->define_locationAndPath($location);
    }

    /* Location related methods */

    /**
     * Set up plugin location value and path.
     * @param string $location
     * @return void
     */
    private function define_locationAndPath($location=false) {
        global $CaMykS;

        /* Load type folder */
        $typeFolder = $this->get_typeFolder();

        /* Check is in shared plugins */
        if ($location == 'Shared' or ($location === false and __FILE__ == $CaMykS->get_engineFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'Shared';
            $this->pluginPath = $CaMykS->get_engineFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/');
        }

        /* Check is in system plugins */
        elseif ($location == 'System' or ($location === false and __FILE__ == $CaMykS->get_engineFileFullPath('Core/Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'System';
            $this->pluginPath = $CaMykS->get_engineFileFullPath('Core/Plugins/'.$typeFolder.'/'.$this->libraryName.'/');
        }

        /* Check is in website plugins */
        elseif ($location == 'Website' or ($location === false and __FILE__ == $CaMykS->get_websiteFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'Website';
            $this->pluginPath = $CaMykS->get_websiteFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/');
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
        $filePath = $this->plugin_path.'/Libraries/'.$library.$CaMykS->file_extension;

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
     * @param $string $folder
     * @param string $language
     * @return boolean success
     */
    protected function load_locales($folder='', $language=null) {
        global $CaMykS;

        /* Load locales in plugin folder */
        return $CaMykS->load_locales($this->plugin_path.'/Locales'.($folder != ''? '/'.$folder : ''), $this->libraryName, $language);
    }

    /* Files related methods */

    /* CaMykS events related methods */

    /* CaMykS page related methods */

}
?>
