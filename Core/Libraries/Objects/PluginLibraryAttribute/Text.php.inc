<?php
/**
 * @brief TextPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre7
 * @date Creation: Feb 2021
 * @date Modification: Oct 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * TextPluginLibraryAttribute class.
 */
class TextPluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Length'=>32, 'StripPHPTags'=>true, 'StripJSTags'=>true, 'StripHTMLTags'=>false, 'AutoTrim'=>false,);

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Return read value from save.
     * @param mixed $value
     * @return mixed
     */
    protected function read_valueFromSave(mixed $value): mixed {
        return stripSlashes($value);
    }

    /**
     * Return optimise value for save.
     * @param mixed $value
     * @return $mixed
     */
    protected function optimise_valueToSave(mixed $value): mixed {
        return addSlashes($value);
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value(): bool {
        if ($this->isLocalised && is_array($this->value)) {
            foreach ($this->value as $lang => $value) {
                $value = is_array($value) ? (string)reset($value) : (string)$value;
                if ($this->params['StripHTMLTags']) {
                    $value = strip_tags($value, '<br>');
                    $value = preg_replace('/\<br(\s*)?\/?\>/i', "\n", $value);
                } else {
                    if ($this->params['StripPHPTags'])
                        $value = preg_replace(array('/<\?[^\?>]*\?>/', '/<\?/', '/<qss[^>]*>/', '/<qss/'), '', $value);
                    if ($this->params['StripJSTags'])
                        $value = preg_replace(array('/<script[^>]*>[^<]<\/script>/', '/<script/'), '', $value);
                }
                if ($this->params['AutoTrim'])
                    $value = trim($value);
                $this->value[$lang] = substr($value, 0, $this->get_realMaxLength());
            }
        } else {
            $value = (string)$this->value;
            if ($this->params['StripHTMLTags']) {
                $value = strip_tags($value, '<br>');
                $value = preg_replace('/\<br(\s*)?\/?\>/i', "\n", $value);
            } else {
                if ($this->params['StripPHPTags'])
                    $value = preg_replace(array('/<\?[^\?>]*\?>/', '/<\?/', '/<qss[^>]*>/', '/<qss/'), '', $value);
                if ($this->params['StripJSTags'])
                    $value = preg_replace(array('/<script[^>]*>[^<]<\/script>/', '/<script/'), '', $value);
            }
            if ($this->params['AutoTrim'])
                $value = trim($value);
            $this->value = substr($value, 0, $this->get_realMaxLength());
        }

        return true;
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters(): array {
        return array(
            /* Generic values */
            'Type'      => 'text',  /* text, integer, float, date, datetime, time */
            'Default'   => $this->defaultValue,
            'IsUnique'  => $this->params['IsUnique'],

            /* Text settings */
            'Length'    => $this->params['Length'], /* 0-255|Tiny, 65535|Small, 16777215|Medium, 4294967295|Long  */

            /* Primary key and index settings */
            'IndexName'     => $this->params['IndexName'],
            'IndexLength'   => $this->params['IndexLength'],
        );
    }

    /**
     * Return real maximum string length from parameters.
     * @return integer
     */
    public function get_realMaxLength(): int {
        if (is_int($this->params['Length'])) {
            if ($this->params['Length'] <= 256)
                return $this->params['Length'];
            elseif ($this->params['Length'] <= 65535)
                return 65535;
            elseif ($this->params['Length'] <= 16777215)
                return 16777215;
            else
                return 4294967295;
        } elseif (is_string($this->params['Length'])) {
            if ($this->params['Length'] === 'Tiny')
                return 256;
            elseif ($this->params['Length'] === 'Small')
                return 65535;
            elseif ($this->params['Length'] === 'Medium')
                return 16777215;
            elseif ($this->params['Length'] === 'Long')
                return 4294967295;
        }
    }
}
?>
