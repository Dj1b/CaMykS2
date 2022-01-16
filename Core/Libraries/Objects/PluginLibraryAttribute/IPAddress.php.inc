<?php
/**
 * @brief IPAddressPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @details Available formats
 * <br /> '' is not specified, can be any of the following
 * <br /> IPv4 : 000.000.000.000
 * <br /> IPv6 : 0000:0000:0000:0000:0000:0000:0000:0000
 * <br /> or   : 0000:0000:0000:0000:0000:0000:000.000.000.000
 * <br /> CIDR : 000.000.000.000/00
 */

namespace CaMykS2;

/**
 * IPAddressPluginLibraryAttribute class.
 */
final class IPAddressPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Length'=>46, 'StripPHPTags'=>true, 'StripJSTags'=>true, 'StripHTMLTags'=>true, 'AutoTrim'=>true, 'Format'=>'');

    /* Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    protected function check_value() {
        $format = network_checkIPFormat($this->value);
        if ($format === '')
            return false;

        if ($this->params['Format'] === '')
            return true;

        if ($this->params['Format'] !== $format)
            return false;

        /* Return value is conform */
        return true;
    }
}
?>
