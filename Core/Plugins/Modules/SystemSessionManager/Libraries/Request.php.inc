<?php
/**
 * @brief SystemSessionManager admin module, request library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0.1
 * @date Creation: Jan 2022
 * @date Modification: Feb 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemSessionManagerSession class.
 */
final class SystemSessionManagerRequest extends ModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define link to parent session attributes */
        $this->define_databaseAttribute('SessionId',            'ObjectId',     0);
        $this->define_databaseAttribute('CreateDate',           'Date',         true);
        $this->define_databaseAttribute('IPAddress',            'IPAddress',    '');
        $this->define_databaseAttribute('UserAgent',            'Text',         '', array('Length'=>255));

        /* Define PHP server information attributes */
        $this->define_databaseAttribute('PHPGenerationTime',    'Integer',      0);
        $this->define_databaseAttribute('PHPMemoryPeak',        'Float',        0);

        /* Define request information attributes */
        $this->define_databaseAttribute('RequestMethod',        'ListItem',     '', array('Values'=>array('GET', 'HEAD', 'POST', 'PUT')));
        $this->define_databaseAttribute('RequestURL',           'Text',         '', array('Length'=>255));
        $this->define_databaseAttribute('RequestData',          'Text',         '', array('Length'=>255));

        /* Define response information attributes */
        $this->define_databaseAttribute('ResponseStatus',       'Integer',      0);
        $this->define_databaseAttribute('ResponseType',         'Text',         '');

        /* Define content related attributes */
        $this->define_databaseAttribute('ContentLanguage',      'Language',     '');
        $this->define_databaseAttribute('ContentPageId',        'Integer',      0);
        $this->define_databaseAttribute('ContentPageParams',    'Text',         '');
        $this->define_databaseAttribute('ContentAction',        'Text',         '');
        $this->define_databaseAttribute('ContentStatus',        'Boolean',      false);
        $this->define_databaseAttribute('ContentObject',        'Text',         '');
        $this->define_databaseAttribute('ContentObjectId',      'Integer',      '');
    }
}
?>
