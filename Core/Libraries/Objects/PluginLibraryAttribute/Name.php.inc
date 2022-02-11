<?php
/**
 * @brief NamePluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: May 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * NamePluginLibraryAttribute class.
 */
final class NamePluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Length'=>32, 'ValidationRegex'=>'[^0-9A-Za-z]*', 'ReplacementChar'=>'');

    /* Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Check validated characters */
        $this->value = preg_replace('/'.$this->params['ValidationRegex'].'/', $this->params['ReplacementChar'], $this->value);

        /* Check length */
        $this->value = substr($this->value, 0, $this->params['Length']);

        /* Return value is conform */
        return true;
    }
}
?>
