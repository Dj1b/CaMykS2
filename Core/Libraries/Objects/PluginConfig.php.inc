<?php
/**
 * @brief PluginConfig object.
 * <br />Abstract library to create plugin configuration objects.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2021
 * @date Modification: Jan 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * PluginConfig class.
 */
abstract class PluginConfig extends PluginLibrary {
    /**
     * @var string $objectName
     * @brief Object name.
     */
    protected $objectName;

    /**
     * Class constructor.
     * @param Object $parent
     * @param string $name
     * @return void
     */
    public function __construct(&$parent, $name='config') {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define library variables */
        $this->objectName = $name;
    }

    /* Data management methods */

    /**
     * Load object.
     * @return boolean success
     */
    public function load() {

    }
}
?>
