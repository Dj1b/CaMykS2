<?php
/**
 * @brief ContentModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Nov 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ContentModuleLibrary class.
 */
class ContentModuleLibrary extends ManagedModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Content module library attributes */
        $this->define_databaseAttribute('Name', 'Text', '', array('Length' => 128, 'AutoTrim' => true, 'StripHTMLTags' => true));
    }

    /* Mark: Content list methods */

    /**
     * Return associative list of content names indexed by object ID.
     * @param array $whereParams
     * @param array $orderBy
     * @return array
     */
    public function get_nameList(array $whereParams=array(), array $orderBy=array('Name' => 'asc')): array {
        return $this->get_objectAttributeList('Name', false, $whereParams, -1, -1, $orderBy);
    }
}
?>
