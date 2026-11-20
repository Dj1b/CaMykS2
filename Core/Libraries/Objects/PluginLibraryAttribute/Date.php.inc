<?php
/**
 * @brief DatePluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Jan 2022
 * @date Modification: Nov 2026
 * @copyright 2022 - 2026 CaMykS Team
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

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Return read value from save.
     * @param mixed $value
     * @return mixed
     */
    protected function read_valueFromSave(mixed $value): mixed {
        if (is_null($value) || $value === '' || $value === false || $value === 0 || $value === '0' || str_starts_with((string)$value, '0000-00-00'))
            return 0;
        if (is_numeric($value))
            return (int)$value;
        $ts = strtotime((string)$value);
        return ($ts !== false) ? $ts : 0;
    }

    /**
     * Return optimise value for save.
     * @param mixed $value
     * @return mixed
     */
    protected function optimise_valueToSave(mixed $value): mixed {
        return $this->format_date($value);
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value(): bool {
        if ($this->value === false || is_null($this->value) || $this->value === '')
            $this->value = 0;

        /* Return value is conform */
        return true;
    }

    /**
     * Return default value.
     * @return mixed
     */
    protected function get_defaultValue(): mixed {
        if ($this->defaultValue === true)
            return time();
        if ($this->defaultValue === false || is_null($this->defaultValue))
            return 0;
        return $this->defaultValue;
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters(): array {
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

    /* Mark: Specific methods */

    /**
     * Return formatted date or null when unset.
     * @param mixed $value
     * @return string|null
     */
    private function format_date(mixed $value): ?string {
        if (is_null($value) || $value === false || $value === '' || $value === 0 || $value === '0')
            return null;

        if (!is_numeric($value)) {
            if (str_starts_with((string)$value, '0000-00-00'))
                return null;
            $ts = strtotime((string)$value);
            if ($ts === false || $ts <= 0)
                return null;
            $value = $ts;
        } else {
            $value = (int)$value;
            if ($value <= 0)
                return null;
        }

        if ($this->params['Format'] == 'DateTime')
            return date('Y-m-d H:i:s', $value);

        if ($this->params['Format'] == 'Date')
            return date('Y-m-d', $value);

        if ($this->params['Format'] == 'Time')
            return date('H:i:s', $value);

        return null;
    }
}
?>
