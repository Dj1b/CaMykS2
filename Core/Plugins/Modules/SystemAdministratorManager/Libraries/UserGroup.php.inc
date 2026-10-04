<?php
/**
 * @brief SystemAdministratorManager admin module, user group link library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Feb 2022
 * @date Modification: Oct 2026
 * @copyright 2022 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemAdministratorManager;
/**
 * UserGroup class.
 */
final class UserGroup extends \CaMykS2\ObjectAssociationModuleLibrary {
    /**
     * @var string $object1
     * @brief Object 1 name.
     */
    protected string $object1 = 'UserId';

    /**
     * @var string $object2
     * @brief Object 2 name.
     */
    protected string $object2 = 'GroupId';
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
