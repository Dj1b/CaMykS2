<?php
/**
 * @brief UniqueIdPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Oct 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * UniqueIdPluginLibraryAttribute class.
 */
final class UniqueIdPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Length'=>32, 'ValidationRegex'=>'[^0-9a-f]*', 'ReplacementChar'=>'');

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value(): bool {
        /* Check validated characters */
        $this->value = preg_replace('/'.$this->params['ValidationRegex'].'/', $this->params['ReplacementChar'], $this->value);

        /* Check length */
        $this->value = substr($this->value, 0, $this->params['Length']);

        /* Return value is conform */
        return true;
    }
}
?>
