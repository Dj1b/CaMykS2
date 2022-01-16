<?php
/**
 * @brief EmailAddressPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * EmailAddressPluginLibraryAttribute class.
 */
class EmailAddressPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Length'=>255, 'StripHTMLTags'=>true, 'AutoTrim'=>true);

    /* Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    protected function check_value() {
        /* Check generic text checks */
        if (!parent::check_value())
            return false;

        /* Check email address validity */
        if (!email_isValid($this->value))
            return false;

        /* Return value is conform */
        return true;
    }
}
?>
