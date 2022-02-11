<?php
/**
 * @brief SystemPluginManager admin module, plugin library.
 * @details Plugin / Module Library
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
 * SystemPluginManagerPlugin class.
 */
final class SystemPluginManagerPlugin extends ModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define content attributes */
        $this->define_databaseAttribute('Status',   'Integer',  0, array('Max'=>16)); /* 0: Disabled / 1: Enabled */
        $this->define_databaseAttribute('Name',     'Name',     '');
        $this->define_databaseAttribute('Type',     'ListItem', '', array('Values' => array('Component', 'Module', 'Theme', 'Template')));
        $this->define_databaseAttribute('Location', 'ListItem', '', array('Values' => array('System', 'Shared', 'Website')));
        $this->define_databaseAttribute('Package',  'Name',     '');
        $this->define_databaseAttribute('Version',  'Version',  '1.0');
        $this->define_databaseAttribute('Source',   'ObjectId', 0);
    }

    /* Specific tool methods */

    /**
     * Load plugin information from its name.
     * @param string $name
     * @return boolean success
     */
    public function load_fromName($name=null) {
        /* Check name */
        if (is_null($name)) {
            if (is_null($this->Name))
                return false;
            $name = $this->Name;
        }

        /* Build query */
        $query = new DbQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_whereCondition(new DbQueryWhereCondition('Name', $name));

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return false;

        if ($data = $this->get_queryDataLine()) {
            /* Plugin has been found */
            $this->objectId = $data['id'];
            $this->load_fromArray($data);
        } else
            /* Plugin has not been found */
            $this->initialise();

        /* Return success */
        return true;
    }
}
?>
