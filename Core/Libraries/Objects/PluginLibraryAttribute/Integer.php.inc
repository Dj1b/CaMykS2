<?php
/**
 * @brief IntegerPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Feb 2021
 * @date Modification: Dec 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * IntegerPluginLibraryAttribute class.
 */
class IntegerPluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Min'=>null, 'Max'=>null, 'Unsigned'=>'Auto');

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
     * Check and optimise stored value.
     * @return boolean result
     */
    protected function check_value() {
        /* Force value as an integer */
        $this->value = (int)$this->value;

        /* Check minimum value */
        if (!is_null($this->params['Min']))
            $this->value = max($this->params['Min'], $this->value);

        /* Check maximum value */
        if (!is_null($this->params['Max']))
            $this->value = min($this->params['Max'], $this->value);

        /* Return value is conform */
        return true;
    }
}
?>
