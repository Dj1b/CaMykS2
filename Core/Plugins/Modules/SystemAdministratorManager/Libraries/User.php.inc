<?php
/**
 * @brief SystemAdministratorManager admin module, user library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2022
 * @date Modification: Mar 2023
 * @copyright 2022 - 2023 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemAdministratorManagerUser class.
 */
final class SystemAdministratorManagerUser extends AdvancedModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define management attributes */
        $this->define_databaseAttribute('Type',                     'ListItem',     '', array('Values'=>array('Owner', 'Webmaster', 'Editor')));
        $this->define_databaseAttribute('Email',                    'Email',        '');
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
}
?>
