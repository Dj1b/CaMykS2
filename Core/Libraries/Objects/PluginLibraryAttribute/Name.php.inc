<?php
/**
 * @brief NamePluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: May 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * NamePluginLibraryAttribute class.
 */
abstract class NamePluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Length'=>32, 'ValidationRegex'=>'[^0-9A-Za-z]*', 'ReplacementChar'=>'');

    /* Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    protected function check_value() {
        /* Check validated characters */
        $this->value = preg_replace('/'.$this->params['ValidationRegex'].'/', $this->params['ReplacementChar'], $this->value);

        /* Check length */
        $this->value = substr($value, 0, $this->params['Length']);

        /* Return value is conform */
        return true;
    }
}
?>
