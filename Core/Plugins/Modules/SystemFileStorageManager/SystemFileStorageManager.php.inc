<?php
/**
 * @brief SystemFileStorageManager tool module.
 * <br />Module to manage in-database local storage.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2021
 * @date Modification: Jan 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemFileStorageManager class.
 */
final class SystemFileStorageManager extends ToolModule {

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Module methods */

    /**
     * Load module.
     * @return void
     */
    protected function load() {
        /* Check is already loaded */
        if ($this->moduleLoaded)
            return;

        /* Load libraries */

        /* Create internal object */

        /* Mark module as loaded */
        $this->moduleLoaded = true;
    }

    /* Documents related methods */

    /**
     * Return documents URL.
     * @param string $plugin
     * @param string $path
     * @param array $files
     * @param string $URLMode
     * @return mixed
     */
    public function get_documentsURL($plugin, $path, $files=array(), $URLMode=null) {

    }

    /**
     * Save documents.
     * @param string $plugin
     * @param string $path
     * @param array $files
     * @return mixed
     */
    public function save_uploadedDocuments($plugin, $path, $files=array()) {

    }

    /* Configuration related methods */

    /**
     * Return configuration data.
     * @param string $plugin
     * @param string $type
     * @param string $name
     * @return array
     */
    public function get_configuration($plugin, $type, $name) {

    }

    /**
     * Save configuration data.
     * @param string $plugin
     * @param string $type
     * @param string $name
     * @param array $data
     * @return boolean success
     */
    public function save_configuration($plugin, $type, $name, $data) {

    }

    /* Event related methods */

    /**
     * Enable database storage
     * @return void
     */
    public function enable_databaseStorage() {

    }

    /**
     * Disable database storage
     * @return void
     */
    public function disable_databaseStorage() {

    }
}
?>
