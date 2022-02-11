<?php
/**
 * @brief DatePluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2022
 * @date Modification: Feb 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @details Available formats
 * <br /> DateTime
 * <br /> Date
 * <br /> Time
 */

namespace CaMykS2;

/**
 * DatePluginLibraryAttribute class.
 */
class DatePluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Format'=>'DateTime');

    /* Override PluginLibraryAttribute methods */

    /**
     * Return read value from save.
     * @param mixed $value
     * @return mixed
     */
    protected function read_valueFromSave($value) {
        return strtotime($value);
    }

    /**
     * Return optimise value for save.
     * @param mixed $value
     * @return $mixed
     */
    protected function optimise_valueToSave($value) {
        return $this->format_date($value);
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {


        /* Return value is conform */
        return true;
    }

    /**
     * Return default value.
     * @return mixed
     */
    protected function get_defaultValue() {
        if ($this->defaultValue === true)
            return time();
        return $this->defaultValue;
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters() {
        $params = array(
            /* Generic values */
            'Type'      => strtolower($this->params['Format']),
            'Default'   => $this->defaultValue,
            'IsUnique'  => $this->params['IsUnique'],

            /* Primary key and index settings */
            'IndexName'     => $this->params['IndexName'],
            'IndexLength'   => $this->params['IndexLength'],
        );

        /* Return params */
        return $params;
    }

    /* Specific methods */

    /**
     * Return formated date.
     * @param mixed $value
     * @return string
     */
    private function format_date($value) {
        if ($this->params['Format'] == 'DateTime')
            return date('Y-m-d H:i:s', $value);

        if ($this->params['Format'] == 'Date')
            return date('Y-m-d', $value);

        if ($this->params['Format'] == 'Time')
            return date('H:i:s', $value);

        return '';
    }
}
?>
