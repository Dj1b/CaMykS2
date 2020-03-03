<?php
/**
 * @brief Template object.
 * <br />Template abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Dec 2019
 * @date Modification: Mar 2019
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Template class.
 */
abstract class Template extends Plugin {

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Template';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /**
     * Initialise template object.
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        parent::initialise($params);
    }

    /**
     * Display template object layout.
     * @return void
     */
    public function display() {
        $this->load_layout('Main');
    }

    /**
     * Display template object thumb.
     * @return void
     */
    public function display_thumb() {
        $this->load_layout('Thumb');
    }

    /**
     * Display in-template slot content.
     * @param integer $slotIndex
     * @return void
     */
    protected function display_slotContent($slotIndex) {
        global $CaMykS;
        $CaMykS->display_templateSlotContent($slotIndex);
    }
}
?>
