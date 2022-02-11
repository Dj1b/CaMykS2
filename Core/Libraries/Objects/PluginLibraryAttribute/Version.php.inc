<?php
/**
 * @brief VersionPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: May 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * VersionPluginLibraryAttribute class.
 */
final class VersionPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Length' => 16);

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Check value is valid version number */
        if (!version_compare($this->value, '0.0.0.1', '>='))
            return false;

        /* Return value is conform */
        return true;
    }
}
?>
