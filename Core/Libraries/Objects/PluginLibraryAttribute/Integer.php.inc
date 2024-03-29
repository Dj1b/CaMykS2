<?php
/**
 * @brief IntegerPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Feb 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
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
    public function check_value() {
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

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters() {
        $params = array(
            /* Generic values */
            'Type'      => 'integer',
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
