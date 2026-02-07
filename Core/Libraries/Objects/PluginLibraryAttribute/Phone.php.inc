<?php
/**
 * @brief PhonePluginLibraryAttribute object.
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
 * PhonePluginLibraryAttribute class.
 */
class PhonePluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Length'=>20);

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        global $CaMykS;

        /* Load phone tool library */
        $CaMykS->load_library('phone', 'Tools');

        /* Check empty value (if not mandatory) */
        if ($this->value === '') {
            return true;
        }

        /* Check value validity */
        if (phone_validate($this->value) === false) {
            return false;
        }

        /* Format value */
        $this->value = phone_format($this->value);

        /* Check length */
        if (strlen($this->value) > $this->params['Length']) {
            return false;
        }

        /* Return value is conform */
        return true;
    }
}
?>
