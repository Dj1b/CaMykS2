<?php
/**
 * @brief TextPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Feb 2021
 * @date Modification: Dec 2021
 * @copyright 2021 CaMykS Team
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

    /* Override PluginLibraryAttribute methods */

    /**
     * Return read value from save.
     * @param mixed $value
     * @return mixed
     */
    protected function read_valueFromSave($value) {
        return stripSlashes($value);
    }

    /**
     * Return optimise value for save.
     * @param mixed $value
     * @return $mixed
     */
    protected function optimise_valueToSave($value) {
        return addSlashes($value);
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    protected function check_value() {
        /* Check tags stripping */
        if ($this->params['StripHTMLTags']) {
            $this->value = strip_tags($value, '<br>');
            $this->value = preg_replace('/\<br(\s*)?\/?\>/i', "\n", $this->value);
        } else {
            if ($this->params['StripPHPTags'])
                $this->value = preg_replace(array('/<\?[^\?>]*\?>/', '/<\?/', '/<qss[^>]*>/', '/<qss/'), '', $this->value);
            if ($this->params['StripJSTags'])
                $this->value = preg_replace(array('/<script[^>]*>[^<]<\/script>/', '/<script/'), '', $this->value);
        }

        /* Check autotrim */
        if ($this->params['AutoTrim'])
            $this->value = trim($this->value);

        /* Check length */
        $this->value = substr($value, 0, $this->params['Length']);

        /* Return value is conform */
        return true;
    }
}
?>
