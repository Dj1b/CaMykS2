<?php
/**
 * @brief SystemAdministratorManager admin module, user group link library.
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
 * SystemAdministratorManagerUserGroup class.
 */
final class SystemAdministratorManagerUserGroup extends ObjectAssociationModuleLibrary {
    /**
     * @var string $object1
     * @brief Object 1 name.
     */
    protected $object1 = 'UserId';

    /**
     * @var string $object2
     * @brief Object 2 name.
     */
    protected $object2 = 'GroupId';
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);
    }
}
?>
