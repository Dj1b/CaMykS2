<?php
/**
 * @brief SystemPluginManager admin module.
 * <br />Module to manage installed plugins.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: May 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemPluginManager class.
 */
final class SystemPluginManager extends AdminModule {
    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected $libraries = array('Plugin');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }
}
?>
