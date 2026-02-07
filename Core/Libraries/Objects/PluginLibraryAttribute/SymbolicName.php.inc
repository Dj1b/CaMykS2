<?php
/**
 * @brief SymbolicNamePluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2026
 * @date Modification: Feb 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SymbolicNamePluginLibraryAttribute class.
 */
class SymbolicNamePluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Length'=>64, 'AutoFormat'=>true, 'Separator'=>'_');

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Check empty value (if not mandatory) */
        if ($this->value === '') {
            return true;
        }

        /* Auto format value */
        if ($this->params['AutoFormat']) {
            /* Trim value */
            $this->value = trim($this->value);

            /* Lower case value */
            $this->value = strtolower($this->value);

            /* Replace separators */
            $this->value = str_replace(array(' ', '-'), $this->params['Separator'], $this->value);

            /* Remove unauthorized characters */
            $this->value = preg_replace('/[^a-z0-9'.preg_quote($this->params['Separator'], '/').']/', '', $this->value);

            /* Remove double separators */
            while (strpos($this->value, $this->params['Separator'].$this->params['Separator']) !== false)
                $this->value = str_replace($this->params['Separator'].$this->params['Separator'], $this->params['Separator'], $this->value);
        }

        /* Check validation regex */
        if (!preg_match('/^[a-z0-9]+(['.preg_quote($this->params['Separator'], '/').'][a-z0-9]+)*$/', $this->value))
            return false;

        /* Check length */
        if (strlen($this->value) > $this->params['Length'])
            return false;

        /* Return value is conform */
        return true;
    }
}
?>
