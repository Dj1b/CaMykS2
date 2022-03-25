<?php
/**
 * @brief ObjectAssociationModuleLibrary object.
 * @details Engine / Object Library
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
 * ObjectAssociationModuleLibrary class.
 */
class ObjectAssociationModuleLibrary extends ModuleLibrary {
    /**
     * @var string $object1
     * @brief Object 1 name.
     */
    protected $object1 = 'Object1Id';

    /**
     * @var string $object2
     * @brief Object 2 name.
     */
    protected $object2 = 'Object2Id';

    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Simple module library attributes */
        $this->define_databaseAttribute($this->object1,   'ObjectId',  0);
        $this->define_databaseAttribute($this->object2,   'ObjectId',  0);
    }
}
?>
