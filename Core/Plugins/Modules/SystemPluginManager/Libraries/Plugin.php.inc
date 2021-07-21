<?php
/**
 * @brief SystemPluginManager admin module, plugin library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: May 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
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
        $this->define_databaseAttribute('Status',   'Integer',  0);
        $this->define_databaseAttribute('Name',     'Name',     '');
        $this->define_databaseAttribute('Type',     'ListItem', '', array('Values'=>array('Component', 'Module/Admin', 'Module/Content', 'Theme', 'Template')));
        $this->define_databaseAttribute('Location', 'ListItem', '', array('Values'=>array('System', 'Shared', 'Website')));
        $this->define_databaseAttribute('Package',  'Name',     '');
        $this->define_databaseAttribute('Version',  'Version',  '1.0');
    }
}
?>
