<?php
/**
 * @brief SystemPluginManager admin module.
 * <br />Module to manage installed plugins.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: May 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemPluginManager class.
 */
final class SystemPluginManager extends AdminModule {
    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected $libraries = array('Plugin');

    /**
     * @var array $installedTables
     * @brief List of installed tables.
     */
    private $installedTables;

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Plugin methods */

     /**
     * Install plugin.
     * @return boolean success
     */
    public function install() {
        /* Perform plugin installation */
        if (!parent::install())
            return false;

        /* Add its own information in plugin list data */
        $this->Plugin->load_fromArray(array(
            'Status'    => 1,
            'Name'      => $this->libraryName,
            'Type'      => $this->pluginType,
            'Location'  => 'System',
            'Version'   => $this->pluginVersion,
        ));
        if (!$this->Plugin->save())
            return false;

        /* Initialise installed tables list */
        $this->installedTables = array();

        /* Return success */
        return true;
    }

    /* Admin mode methods */

    /* Plugin related tool methods */

    /**
     * Enable plugin.
     * @param string $name
     * @param string $type
     * @param string $location
     * @return boolean success
     */
    public function enable_plugin($name, $type, $location=null) {
        global $CaMykS;

        /* Load plugin information from database */
        $pluginInfo = $this->Plugin->get_newInstance();
        if (!$pluginInfo->load_byName($name))
            return false;

        /* Install plugin if required */
        $plugin = $CaMykS->get_plugin($name, $type, true, $location);
        $plugin->initialise();
        if (!$plugin->is_installed()) {
            if (!$plugin->install()) {
                return false;
            }
        }

        /* Check plugin already exists */
        if ($pluginInfo->objectId == 0) {
            $pluginInfo->Name = $name;
            $pluginInfo->Type = $type;
            $pluginInfo->Location = $plugin->get_location();
        }

        /* Update other values */
        $pluginInfo->Status = 1;
        $pluginInfo->Version = $plugin->get_version();

        /* Save plugin */
        return $pluginInfo->save();
    }

    /**
     * Check plugin is enabled.
     * @param string $name
     * @return boolean result
     */
    public function is_pluginEnabled($name) {
        /* Load plugin information from database */
        $pluginInfo = $this->Plugin->get_newInstance();
        $pluginInfo->load_byName($name);

        /* Check plugin already exists */
        if ($pluginInfo->objectId == 0)
            return false;

        /* Return result from database */
        return ($pluginInfo->Status === 1);
    }

    /* Table/Library related tool methods */

    /**
     * Check if table is installed.
     * @param string $tableName
     * @return boolean result
     */
    public function check_tableExists($tableName='') {
        if ($this->installedTables === null)
            $this->load_installedTables();
        return (in_array($tableName, $this->installedTables));
    }

    /* Private methods */

    /**
     * Load installed tables.
     * @return boolean success
     */
    private function load_installedTables() {
        global $CaMykS;

        /* Load all tables in databases */
        $list = $CaMykS->databaseConnection->get_tableList();

        /* Initialise installed tables list */
        $this->installedTables = array();

        /* Check for tables in website */
        $tablePrefix = $CaMykS->databaseConnection->get_tablePrefix();
        foreach ($list as $tableName) {
            if (string_startsWith($tableName, $tablePrefix))
                $this->installedTables[] = $tableName;
        }
    }
}
?>
