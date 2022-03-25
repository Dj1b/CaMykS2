<?php
/**
 * @brief SystemSecurityManager admin module, filter rule library.
 * @details Plugin / Module Library
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
 * SystemSecurityManagerFilterRule class.
 */
final class SystemSecurityManagerFilterRule extends AdvancedModuleLibrary {
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
        $this->define_databaseAttribute('Permission',   'Boolean',     1);
        $this->define_databaseAttribute('AccessMode',   'Integer',     0, array('Min'=>0, 'Max'=>2));
        $this->define_databaseAttribute('ValidityDate', 'Date',        false);
        $this->define_databaseAttribute('Reason',       'Integer',     0);

        /* Define filter attributes */
        $this->define_databaseAttribute('IPAddress',    'IPAddress',    '');
        $this->define_databaseAttribute('URLPattern',   'Text',         '', array('Length'=>64));
        $this->define_databaseAttribute('UserAgent',    'Text',         '', array('Length'=>64));
        $this->define_databaseAttribute('Referer',      'Text',         '', array('Length'=>64));

        /* Define comment attribute */
        $this->define_databaseAttribute('Comment',      'Text',         '');

        /* Define optional attributes */
        $this->define_databaseAttribute('Country',      'Country',      '');
    }

    /* Data methods */

    /**
     * Return login identifier values.
     * @return array
     */
    public function get_loginIdentifierList() {
        return array(
            0 => $this->get_locale('FilterRulePermission_Deny'),
            1 => $this->get_locale('FilterRulePermission_Allow'),
        );
    }
}
?>
