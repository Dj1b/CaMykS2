<?php
/**
 * @brief Plugin object.
 * <br />Parent object for Module, Template, Theme, Component, extends CLibrary.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
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
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected $hasConfiguration = false;

    /**
     * @var string $pluginPath
     * @brief Plugin root path.
     */
    protected $pluginPath;

    /**
     * Class constructor.
     * @return void
     */
    public function __construct() {
        parent::__construct();

        /* Set up plugin pathes */
        $this->pluginPath = '';
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
        global $camyks;

        /* build file path */
        $filePath = $this->plugin_path.'/lib/'.$library.$camyks->file_extension;

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
