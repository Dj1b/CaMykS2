<?php
/**
 * @brief Plugin object.
 * <br />Parent object for Module, Template, Theme, Component, extends CLibrary.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Plugin class.
 */
abstract class Plugin extends CLibrary {
    /**
     * @var string $version
     * @brief Plugin version.
     */
    protected $version = '1.0';

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
        if ($location == 'shared' or ($location === false and __FILE__ == $CaMykS->get_engineFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'shared';
            $this->pluginPath = $CaMykS->get_engineFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/');
        }

        /* Check is in system plugins */
        elseif ($location == 'system' or ($location === false and __FILE__ == $CaMykS->get_engineFileFullPath('Core/Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'system';
            $this->pluginPath = $CaMykS->get_engineFileFullPath('Core/Plugins/'.$typeFolder.'/'.$this->libraryName.'/');
        }

        /* Check is in website plugins */
        elseif ($location == 'website' or ($location === false and __FILE__ == $CaMykS->get_websiteFileFullPath('Plugins/'.$typeFolder.'/'.$this->libraryName.'/'.$this->libraryName.$CaMykS->libraryExtension))) {
            $this->pluginLocation = 'website';
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

    /**
     * Check plugin has a configuration.
     * @return boolean result
     */
    public function has_configuration() {
        return $this->hasConfiguration;
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

    /* Management related methods */

    /* Extensions related methods */

    /* Dependencies related methods */

    /* Request mode methods */

    /* Languages related methods */

    /* Files related methods */


    /* CaMykS events related methods */

    /* CaMykS page related methods */

}
?>
