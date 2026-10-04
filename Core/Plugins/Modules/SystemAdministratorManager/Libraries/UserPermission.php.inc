<?php
/**
 * @brief SystemAdministratorManager admin module, user permission library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2022
 * @date Modification: Oct 2026
 * @copyright 2022 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemAdministratorManager;
/**
 * UserPermission class.
 */
final class UserPermission extends \CaMykS2\ModuleLibrary {
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
        $this->define_databaseAttribute('Plugin',   'Text',         '', array('Length'=>48));
        $this->define_databaseAttribute('Name',     'Text',         '', array('Length'=>12));
        $this->define_databaseAttribute('Value',    'Integer',      0);
    }
}
?>
