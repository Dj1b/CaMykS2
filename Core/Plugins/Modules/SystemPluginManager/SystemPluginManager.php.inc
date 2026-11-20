<?php
/**
 * @brief SystemPluginManager admin module.
 * <br />Module to manage installed plugins.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre7
 * @date Creation: May 2021
 * @date Modification: Nov 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemPluginManager;

use CaMykS2\AdminModule;

/**
 * SystemPluginManager class.
 */
final class SystemPluginManager extends AdminModule {
    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected array $libraries = array('Plugin');

    /**
     * @var array|null $installedTables
     * @brief List of installed tables.
     */
    private ?array $installedTables = null;

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
    public function install(): bool {
        /* Perform plugin installation */
        if (!parent::install())
            return false;

        /* Add its own information in plugin list data */
        $pluginLibrary = $this->get_libraryInstance('Plugin');
        $pluginLibrary->load_fromArray(array(
            'Status'    => 1,
            'Name'      => $this->libraryName,
            'Type'      => $this->pluginType,
            'Location'  => 'System',
            'Version'   => $this->pluginVersion,
        ));
        if (!$pluginLibrary->save())
            return false;

        /* Reset installed tables list so it will be reloaded on next check */
        $this->installedTables = null;

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
    public function enable_plugin($name, $type, $location=null): bool {
        global $CaMykS;

        /* Load plugin information from database */
        $pluginLibrary = $this->get_libraryInstance('Plugin');
        if ($pluginLibrary === false)
            return false;
        $pluginInfo = $pluginLibrary->get_newInstance();
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
    public function is_pluginEnabled($name): bool {
        /* Load plugin information from database */
        $pluginLibrary = $this->get_libraryInstance('Plugin');
        if ($pluginLibrary === false)
            return false;
        $pluginInfo = $pluginLibrary->get_newInstance();
        $pluginInfo->load_byName($name);

        /* Check plugin already exists */
        if ($pluginInfo->objectId == 0)
            return false;

        /* Return result from database */
        return ((int)$pluginInfo->Status === 1);
    }

    /**
     * Return enabled plugin list.
     * @param string|null $type
     * @return array
     */
    public function get_enabledPluginList(?string $type=null): array {
        /* Load Plugin library instance */
        $pluginLibrary = $this->get_libraryInstance('Plugin');
        if ($pluginLibrary === false)
            return array();

        /* Query enabled plugins from database */
        $rows = $pluginLibrary->get_list(array(
            'Status'    => 1,
            'Type'      => $type ?? '',
        ));

        /* Build Plugin object list */
        $plugins = array();
        foreach ($rows as $id => $data) {
            $plugin = $pluginLibrary->get_newInstance((int)$id);
            $plugin->load_fromArray($data);
            $plugins[$plugin->Name] = $plugin;
        }

        /* Return enabled plugins */
        return $plugins;
    }

    /* Table/Library related tool methods */

    /**
     * Check if table is installed.
     * @param string $tableName
     * @return boolean result
     */
    public function check_tableExists($tableName=''): bool {
        if ($this->installedTables === null)
            $this->load_installedTables();
        return (in_array($tableName, $this->installedTables));
    }

    /**
     * Register a newly created table in the installed tables list.
     * @param string $tableName
     * @return void
     */
    public function register_installedTable(string $tableName): void {
        if ($this->installedTables === null)
            $this->load_installedTables();
        if (!in_array($tableName, $this->installedTables))
            $this->installedTables[] = $tableName;
    }

    /* Private methods */

    /**
     * Load installed tables.
     * @return boolean success
     */
    private function load_installedTables(): bool {
        global $CaMykS;

        /* Initialise installed tables list */
        $this->installedTables = array();

        /* Get main database connection */
        $dbConnection = &$CaMykS->get_databaseConnection();
        if ($dbConnection === false)
            return false;

        /* Load all tables in databases */
        $list = $dbConnection->get_tableList();
        if (!is_array($list))
            return false;

        /* Check for tables in website */
        $tablePrefix = $dbConnection->get_tablePrefix();
        foreach ($list as $tableName) {
            if (string_startsWith($tableName, $tablePrefix))
                $this->installedTables[] = $tableName;
        }

        /* Return success */
        return true;
    }
}
?>
