<?php
/**
 * @brief FloatPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Feb 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * FloatPluginLibraryAttribute class.
 */
class FloatPluginLibraryAttribute extends PluginLibraryAttribute {

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
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Force value as a float */
        $this->value = (float)$this->value;

        /* Check minimum value */
        if (!is_null($this->params['Min']))
            $this->value = max($this->params['Min'], $this->value);

        /* Check maximum value */
        if (!is_null($this->params['Max']))
            $this->value = min($this->params['Max'], $this->value);

        /* Check decimals */
        if (!is_null($this->params['Decimals']))
            $this->value = round($this->value, $this->params['Decimals']);

        /* Return value is correct */
        return true;
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters() {
        $params = array(
            /* Generic values */
            'Type'      => 'float',
            'Default'   => $this->defaultValue,
            'IsUnique'  => $this->params['IsUnique'],

            /* Primary key and index settings */
            'IndexName'     => $this->params['IndexName'],
            'IndexLength'   => $this->params['IndexLength'],
        );

        /* Check minimum value */
        if (!is_null($this->params['Min']))
            $params['Min'] = $this->params['Min'];

        /* Check maximum value */
        if (!is_null($this->params['Max']))
            $params['Max'] = $this->params['Max'];

        /* Check unsigned param */
        if (is_bool($this->params['Unsigned']))
            $params['Unsigned'] = $this->params['Unsigned'];

        /* Return params */
        return $params;
    }
}
?>
