<?php
/**
 * @brief Component object.
 * <br />Component abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Dec 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
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
    protected string $componentType = '';

    /**
     * Class constructor.
     * @param string|null $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Override Plugin variables */
        $this->pluginType = 'Component';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Action related methods */

    /**
     * Plug component to page, to be overrided.
     * @param array $params
     * @return void
     */
    public function plug(array $params=array()): void {
        /* Check component is initialised */
        if ($this->pluginStatus !== 'loaded')
            $this->initialise($params);
        else
            $this->update_params($params);

        /* Add styles and scripts in components */
    }

    /* Mark: Data related methods */

    /**
     * Return component type value.
     * @return string
     */
    public function get_componentType(): string {
        return $this->componentType;
    }
}
?>
