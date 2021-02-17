<?php
/**
 * @brief FloatPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2021
 * @date Modification: Feb 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * FloatPluginLibraryAttribute class.
 */
abstract class FloatPluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Min'=>null, 'Max'=>null, 'Unsigned'=>'Auto', 'Decimals'=>null);

    /* Override PluginLibraryAttribute methods */

    /**
     * Return read value.
     * @param mixed $value
     * @return mixed
     */
    protected function read_value($value) {
        if (is_string($value) and substr_count(',', $value) == 1)
            $value = preg_replace('/,/', '.', $value);

        return (float)$value;
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

        /* Check decimals */
        if (!is_null($this->params['Decimals']))
            $value = round($value, $this->params['Decimals']);

        /* Return value */
        return $value;
    }
}
?>
