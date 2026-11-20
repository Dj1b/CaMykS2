<?php
/**
 * @brief SystemLogManager admin module, Report library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemLogManager;

use CaMykS2\Module;
use CaMykS2\SimpleModuleLibrary;

/**
 * Report class.
 */
final class Report extends SimpleModuleLibrary {

    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Define error attributes */
        $this->define_databaseAttribute('Origin',       'Text',     '');
        $this->define_databaseAttribute('Type',         'Text',     '');
        $this->define_databaseAttribute('Code',         'Integer',  0);
        $this->define_databaseAttribute('File',         'Text',     '');
        $this->define_databaseAttribute('Line',         'Integer',  0);
        $this->define_databaseAttribute('Message',      'Text',     '');
        $this->define_databaseAttribute('Details',      'Text',     '');
        $this->define_databaseAttribute('Level',        'Integer',  3);

        /* Define occurrence counter and date attributes */
        $this->define_databaseAttribute('Counter',      'Integer',  1, array('Min'=>1));
        $this->define_databaseAttribute('CreationDate',  'Date',     true);
        $this->define_databaseAttribute('LastDate',      'Date',     true);

        /* Define admin read attributes */
        $this->define_databaseAttribute('ReadComment',   'Text',     '');
        $this->define_databaseAttribute('ReadDate',      'Date',     0);
        $this->define_databaseAttribute('ReadUser',      'ObjectId', 0);
    }
}
?>
