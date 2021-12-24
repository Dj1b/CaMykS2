<?php
/**
 * @brief Component object.
 * <br />Component abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Dec 2019
 * @date Modification: Dec 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Component class.
 */
abstract class Component extends Plugin {
    /**
     * @var string $componentType
     * @brief Component type.
     */
    protected $componentType = '';

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Component';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Action related methods */

    /**
     * Plug component to page, to be overrided.
     * @return void
     */
    public function plug($params=array()) {
        /* Check component is initialised */
        if ($this->pluginStatus !== 'loaded')
            $this->initialise($params);
        else
            $this->update_params($params);

        /* Add styles and scripts in components */
    }

    /* Data related methods */

    /**
     * Return component type value.
     * @return string
     */
    public function get_componentType() {
        return $this->componentType;
    }
}
?>
