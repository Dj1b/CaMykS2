<?php
/**
 * @brief SystemAdministratorManager admin module, user preference library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2022
 * @date Modification: Feb 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemAdministratorManagerUserPreference class.
 */
final class SystemAdministratorManagerUserPreference extends ModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define management attributes */
        $this->define_databaseAttribute('UserId',   'ObjectId',     0);
        $this->define_databaseAttribute('Name',     'Text',         '', array('Length'=>24));
        $this->define_databaseAttribute('Value',    'Text',         '');
    }
}
?>
