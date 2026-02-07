<?php
/**
 * @brief URLPluginLibraryAttribute object.
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
 * URLPluginLibraryAttribute class.
 */
class URLPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Length'=>255, 'Protocols'=>array('http', 'https'));

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

        /* Check URL validity */
        if (filter_var($this->value, FILTER_VALIDATE_URL) === false) {
            return false;
        }

        /* Check protocol */
        $protocol = parse_url($this->value, PHP_URL_SCHEME);
        if ($protocol === false || !in_array($protocol, $this->params['Protocols'])) {
            return false;
        }

        /* Check length */
        if (strlen($this->value) > $this->params['Length']) {
            return false;
        }

        /* Return value is conform */
        return true;
    }
}
?>
