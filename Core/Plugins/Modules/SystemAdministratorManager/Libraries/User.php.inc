<?php
/**
 * @brief SystemAdministratorManager admin module, user library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Feb 2022
 * @date Modification: Nov 2026
 * @copyright 2022 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemAdministratorManager;

use CaMykS2\ManagedModuleLibrary;

/**
 * User class.
 */
final class User extends ManagedModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Define management attributes */
        $this->define_databaseAttribute('Type',                     'ListItem',     '', array('Values'=>array('Owner', 'Webmaster', 'Editor')));
        $this->define_databaseAttribute('Email',                    'EmailAddress', '');
        $this->define_databaseAttribute('Login',                    'Text',         '');
        $this->define_databaseAttribute('Password',                 'Password',     '');

        /* Define personal information attributes */
        $this->define_databaseAttribute('FirstName',                'Text',         '');
        $this->define_databaseAttribute('LastName',                 'Text',         '');

        $this->define_databaseAttribute('Company',                  'Text',         '');
        $this->define_databaseAttribute('Phone',                    'Phone',        '');
        $this->define_databaseAttribute('Mobile',                   'Phone',        '');
        $this->define_databaseAttribute('Address',                  'Text',         '', array('Length'=>255));

        /* Define security attributes */
        $this->define_databaseAttribute('2FAPlugin',                'Text',         '', array('Length'=>48));
        $this->define_databaseAttribute('2FAKey',                   'Text',         '', array('Length'=>64));

        /* Define history attributes */
        $this->define_databaseAttribute('LastConnectionDate',       'Date',         false);
        $this->define_databaseAttribute('LastPasswordUpdateDate',   'Date',         true);
        $this->define_databaseAttribute('LastPasswords',            'Text',         '', array('Length'=>32767));
        $this->define_databaseAttribute('Last2FACheckDate',         'Date',         false);
    }

    /* Mark: Override ManagedModuleLibrary methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        /* Apply Password_MinLength from module configuration */
        $minLength = (int)$this->get_configValue('Password_MinLength', null, 6);
        if ($minLength > 0)
            $this->set_attributeParam('Password', 'MinLength', $minLength);

        /* Perform parent check_data */
        return parent::check_data($loadPreviousVersion);
    }
}
?>
