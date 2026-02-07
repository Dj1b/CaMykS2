<?php
/**
 * @brief ColorPluginLibraryAttribute object.
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
 * ColorPluginLibraryAttribute class.
 */
class ColorPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Length'=>7, 'UpperCase'=>true);

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Trim value */
        $this->value = trim($this->value);

        /* Check empty value (if not mandatory) */
        if ($this->value === '') {
            return true;
        }

        /* Check value format */
        if (!preg_match('/^#[a-fA-F0-9]{6}$/', $this->value)) {
            /* Try simple fallback for 3-digit hex */
            if (preg_match('/^#[a-fA-F0-9]{3}$/', $this->value)) {
                $this->value = preg_replace('/^#([a-fA-F0-9])([a-fA-F0-9])([a-fA-F0-9])$/', '#$1$1$2$2$3$3', $this->value);
            } else {
                return false;
            }
        }

        /* Case management */
        if ($this->params['UpperCase'])
            $this->value = strtoupper($this->value);

        /* Check length */
        if (strlen($this->value) > $this->params['Length']) {
            return false;
        }

        /* Return value is conform */
        return true;
    }
}
?>
