<?php
/**
 * @brief SystemSitePageManager admin module, Page\Meta library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemSitePageManager\Page;

use CaMykS2\Module;
use CaMykS2\ChildModuleLibrary;

/**
 * Meta class.
 */
final class Meta extends ChildModuleLibrary {
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
        $this->define_databaseAttribute('AttributeType', 'ListItem', 'name', array('Values'=>array('name', 'property', 'http-equiv')));
        $this->define_databaseAttribute('Name',          'Text',     '');
        $this->define_databaseAttribute('Content',       'Text',     '');
        $this->define_databaseAttribute('Language',      'Text',     '');
    }
}
?>
