<?php
/**
 * @brief Theme object.
 * <br />Theme abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Dec 2019
 * @date Modification: Feb 2020
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Theme class.
 */
abstract class Theme extends Plugin {

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = get_class();

        /* Execute parent construct */
        parent::__construct($location);
    }
}
?>
