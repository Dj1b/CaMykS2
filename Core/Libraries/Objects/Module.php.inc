<?php
/**
 * @brief Module object.
 * <br />Module abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Dec 2019
 * @date Modification: Jan 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Module class.
 */
abstract class Module extends Plugin {
    /**
     * @var string $moduleType
     * @brief Module type, values are "Admin", "Content", "Tool".
     */
    protected $moduleType;

    /**
     * @var boolean $moduleLoaded
     * @brief Module status.
     */
    protected $moduleLoaded = false;

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Module';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /**
     * Load module.
     * @return void
     */
    protected function load() {
        $this->moduleLoaded = true;
    }
}
?>
