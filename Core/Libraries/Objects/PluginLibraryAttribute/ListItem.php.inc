<?php
/**
 * @brief ListItemPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: May 2021
 * @date Modification: Dec 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ListItemPluginLibraryAttribute class.
 */
final class ListItemPluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Format'=>'Text', 'Length'=>32, 'Values'=>array());

    /* Override PluginLibraryAttribute methods */

    /**
     * Return read value.
     * @param mixed $value
     * @return mixed
     */
    protected function read_value($value) {
        if ($this->params['Format'] == 'Integer')
            return (int)$value;
        if ($this->params['Format'] == 'Float')
            return (float)$value;
        return $value;
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    protected function check_value() {
        /* Check value is in authorised values */
        if (!in_array($this->value, $this->params['values']))
            return false;

        /* Return value is conform */
        return true;
    }
}
?>
