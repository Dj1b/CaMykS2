<?php
/**
 * @brief SimpleModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SimpleModuleLibrary class.
 */
class SimpleModuleLibrary extends ModuleLibrary {
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
        $this->define_databaseAttribute('Status', 'Integer', 0);
    }

    /* Status related methods */

    /**
     * Return status values.
     * @return array
     */
    public function get_statusValueList() {
        return array(
            1 => $this->get_locale('ObjectStatus_Disabled'),
            2 => $this->get_locale('ObjectStatus_Enabled'),
        );
    }

    /**
     * Return multi object status values.
     * @param array $params
     * @return array
     */
    public function get_multiStatusValueList($params=array()) {
        /* Check params */
        $params = array_merge(array('AddAll'=>false), $params);

        /* Initialise list */
        $list = array();

        /* Check for none value */
        if ($params['AddAll'])
            $list[0] = $this->get_locale('ObjectsStatus_All');

        /* Insert values */
        $list[1] = $this->get_locale('ObjectsStatus_Disabled');
        $list[2] = $this->get_locale('ObjectsStatus_Enabled');

        /* Return list */
        return $list;
    }

    /**
     * Return status title.
     * @param integer $status
     * @return string
     */
    public function get_statusTitle($status=null) {
        if (is_null($status)) $status = $this->Status;

        $list = $this->get_statusValueList();

        if (isset($list[$status]))
            return $list[$status];
        return '<span class="undefined">'.$this->get_locale('ObjectStatus_Undefined').'</span>';
    }
}
?>
