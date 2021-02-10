<?php
/**
 * @brief IntegerPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2021
 * @date Modification: Feb 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * IntegerPluginLibraryAttribute class.
 */
abstract class IntegerPluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @const array DEFAULT_PARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULT_PARAMS = array('Min'=>null, 'Max'=>null, 'Unsigned'=>'Auto');

    /* Override PluginLibraryAttribute methods */

    /**
     * Return read value.
     * @param mixed $value
     * @return mixed
     */
    protected function read_value($value) {
        return (int)$value;
    }

    /**
     * Read and return value.
     * @param mixed $value
     * @return sting
     */
    protected function check_value($value) {
        /* Check minimum value */
        if (!is_null($this->params['Min']))
            $value = max($this->params['Min'], $value);

        /* Check maximum value */
        if (!is_null($this->params['Max']))
            $value = min($this->params['Max'], $value);

        /* Return value */
        return $value;
    }
}
?>
