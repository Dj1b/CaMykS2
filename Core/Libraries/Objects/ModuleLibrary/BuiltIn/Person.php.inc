<?php
/**
 * @brief Person built-in module library object.
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
 * Person class.
 */
class Person extends ManagedIdentifiableModuleLibrary {
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
        $this->define_databaseAttribute('Civility',     'ListItem',     '', array('Values'=>array('', 'Mr', 'Mrs', 'Ms', 'Dr', 'Prof')));
        $this->define_databaseAttribute('LastName',     'Text',         '', array('Length' => 64, 'AutoTrim' => true, 'StripHTMLTags' => true));
        $this->define_databaseAttribute('FirstName',    'Text',         '', array('Length' => 64, 'AutoTrim' => true, 'StripHTMLTags' => true));
        $this->define_databaseAttribute('Email',        'EmailAddress', '');
        $this->define_databaseAttribute('Phone',        'Phone',        '');
        $this->define_databaseAttribute('Mobile',       'Phone',        '');
    }

    /* Mark: Civility title methods */

    /**
     * Return civility title list translated into specified or current language.
     * @param mixed $language
     * @return array
     */
    public function get_civilityTitleList(mixed $language=null): array {
        global $CaMykS;

        /* Load PersonalData generic locales */
        $CaMykS->load_engineLocales('Generic/PersonalData', '', $language);

        $list = array();
        $params = $this->get_attributeParams('Civility');
        $values = isset($params['Values']) ? $params['Values'] : array('', 'Mr', 'Mrs', 'Ms', 'Dr', 'Prof');

        foreach ($values as $val) {
            if ($val === '') {
                $list[''] = '';
                continue;
            }
            $list[$val] = $CaMykS->get_locale('Civility_'.$val, '', array(), $val);
        }

        return $list;
    }

    /**
     * Return translated civility title value for current object.
     * @param mixed $language
     * @return string
     */
    public function get_civilityTitleValue(mixed $language=null): string {
        $list = $this->get_civilityTitleList($language);
        $val = $this->Civility;
        return isset($list[$val]) ? $list[$val] : (string)$val;
    }
}
?>
