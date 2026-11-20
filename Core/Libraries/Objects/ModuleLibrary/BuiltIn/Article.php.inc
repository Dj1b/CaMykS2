<?php
/**
 * @brief Article built-in module library object.
 * @details Engine / Object Library / BuiltIn
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\BuiltIn;

use CaMykS2\Module;
use CaMykS2\ManagedIdentifiableModuleLibrary;

/**
 * Article class.
 */
class Article extends ManagedIdentifiableModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Define attributes */
        $this->define_databaseAttribute('PublishedDate', 'Date', 0);

        /* Define localized attributes */
        $this->define_databaseAttribute('Title',    'Text', '', array('Length' => 64, 'AutoTrim' => true, 'StripHTMLTags' => true), true);
        $this->define_databaseAttribute('Summary',  'Text', '', array('Length' => 256, 'AutoTrim' => true, 'StripHTMLTags' => true), true);
        $this->define_databaseAttribute('Content',  'Text', '', array('Length' => 'Medium', 'AutoTrim' => true), true);
    }
}
?>
