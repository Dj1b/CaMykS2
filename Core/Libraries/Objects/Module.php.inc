<?php
/**
 * @brief Module object.
 * Module abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre6
 * @date Creation: Dec 2019
 * @date Modification: Jan 2022
 * @copyright 2019 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Module class.
 */
abstract class Module extends Plugin {
    /**
     * @var string $moduleType
     * @brief Module type, values are "Admin", "Content", "Tool".
     */
    protected $moduleType;

    /**
     * @var array $libraries
     * @brief List of module internal libraries names.
     */
    protected $libraries = array();

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Module';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Plugin methods */

    /**
     * Load module data.
     * @return void
     */
    protected function load_data() {
        /* Load module libraries */
        $this->load_internalLibraries();
    }

    /**
     * Install plugin.
     * @return boolean success
     */
    public function install() {
        /* Check Plugin installation */
        if (!parent::install())
            return false;

        /* Do libraries installation */
        foreach ($this->libraries as  $library) {

            if (!$this->$library->install()) {
                /* Add error */

                /* Return false */
                return false;
            }
        }

        /* Return success */
        return true;
    }

    /**
     * Uninstall plugin.
     * @return boolean result.
     */
    public function uninstall() {
         /* Check Plugin installation */
        if (!parent::uninstall())
            return false;

        /* Do libraries installation */
        foreach ($this->libraries as $library) {
            if (!$library->uninstall()) {
                /* Add error */

                /* Return false */
                return false;
            }
        }

        /* Return success */
        return true;
    }

    /**
     * Check plugin installation.
     * @return boolean result.
     */
    public function is_installed() {
        global $CaMykS;

        /* Load plugin manager */
        $pluginManager = $CaMykS->get_module('SystemPluginManager');

        /* Return plugin manager answer */
        foreach ($this->libraries as $library)
            if (!$pluginManager->is_pluginLibraryInstalled($library->libraryName))
                return false;

        /* Return check success */
        return true;
    }

    /* Admin mode methods */

    /**
     * Initialise object in admin mode.
     * @param array $params
     * return void
     */
    public function initialise_admin($params=array()) {
        /* Do generic initialisation */
        $this->initialise($params);

        /* Check user access permission */
        /* @TODO */

        /* Load action to execute
        $action = ;
        */

        /* Execute action */
        $method = 'initialise_admin_'.$action;
        if (method_exists($this, $method))
            call_user_method($method, $this);
        else
            $this->initialise_admin_Main();
    }

    /**
     * Display object in admin mode.
     * @return void
     */
    public function display_admin() {
        /* Do generic display */
        parent::display();
    }

    /**
     * Initialise object admin/main mode.
     * @return void
     */
    protected function initialise_admin_Main() {

    }

    /* Configuration mode methods */

    /**
     * Return button for configuration panel.
     * @return false|array
     */
    public function get_adminConfigurationPanelButton() {
        return false;

        return array(
            /*
            array(
                'title'         => '',
                'icon'          => '',
                'category'      => '',
                'categoryTitle' => '', // Optional : to be defined only for non-generic categories
            ),
            */
        );
    }

    /**
     * Initialise object in view configuration mode.
     * @return void
     */
    protected function initialise_configuration_view() {

    }

    /**
     * Display object in view configuration mode.
     * @return void
     */
    protected function display_configuration_view() {

    }

    /**
     * Initialise object in edit configuration mode.
     * @return void
     */
    protected function initialise_configuration_edit() {

    }

    /**
     * Display object in edit configuration mode.
     * @return void
     */
    protected function display_configuration_edit() {

    }

    /* Private methods */

    /**
     * Load libraries.
     * @return boolean success
     */
    private function load_internalLibraries() {
        foreach ($this->libraries as $library) {
            /* Load library */
            if (!$this->load_library($library)) {
                /* Log error */
                /* @TODO */

                /* Return failure */
                return false;
            }

            /* Compute library full name */
            $libraryFullName = $this->libraryNamespace.'\\'.$this->libraryName.$library;

            /* Create and check full library name */
            if (!class_exists($libraryFullName)) {
                /* Log error */
                /* @TODO */

                /* Return failure */
                return false;
            }

            /* Create new object from library */
            $this->$library = new $libraryFullName($this, 0);
        }
        return true;
    }
}
?>
