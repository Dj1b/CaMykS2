<?php
/**
 * @brief BooleanPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2022
 * @date Modification: Apr 2025
 * @copyright 2022 - 2025 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * BooleanPluginLibraryAttribute class.
 */
final class BooleanPluginLibraryAttribute extends IntegerPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Min'=>0, 'Max'=>1, 'Unsigned'=>true);

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Return optimise value for save.
     * @param mixed $value
     * @return $mixed
     */
    protected function optimise_valueToSave(mixed $value) {
        return (bool)$value ? 1 : 0;
    }
}
?>
