<?php
/**
 * @brief TextPluginLibraryAttribute object.
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
 * TextPluginLibraryAttribute class.
 */
abstract class TextPluginLibraryAttribute extends PluginLibraryAttribute {

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
     * Read and return value.
     * @param mixed $value
     * @return sting
     */
    protected function check_value($value) {
        /* Check tags stripping */
        if ($this->params['StripHTMLTags'])
            $value = strip_tags($value, '<br>');
            $value = preg_replace('/\<br(\s*)?\/?\>/i', "\n", $value);
        else {
            if ($this->params['StripPHPTags'])
                $value = preg_replace(array('/<\?[^\?>]*\?>/', '/<\?/', '/<qss[^>]*>/', '/<qss/'), '', $value);
            if ($this->params['StripJSTags'])
                $value = preg_replace(array('/<script[^>]*>[^<]<\/script>/', '/<script/'), '', $value);
        }

        /* Check autotrim */
        if ($this->params['AutoTrim'])
            $value = trim($value);

        /* Return value */
        return $value;
    }
}
?>
