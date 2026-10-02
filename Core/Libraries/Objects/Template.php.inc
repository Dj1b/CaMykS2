<?php
/**
 * @brief Template object.
 * <br />Template abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Dec 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
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
     * @param string|null $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Override Plugin variables */
        $this->pluginType = 'Template';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Override Plugin methods */

    /**
     * Initialise template object.
     * @param array $params
     * @return void
     */
    public function initialise(array $params=array()): void {
        parent::initialise($params);
    }

    /**
     * Display template object layout.
     * @return boolean success
     */
    public function display(): bool {
        return $this->load_layout('Main');
    }

    /* Mark: Specific display methods */

    /**
     * Display template object thumb.
     * @return boolean success
     */
    public function display_thumb(): bool {
        return $this->load_layout('Thumb');
    }

    /**
     * Display in-template slot content.
     * @param integer $slotIndex
     * @return void
     */
    protected function display_slotContent(int $slotIndex): void {
        global $CaMykS;
        $CaMykS->display_templateSlotContent($slotIndex);
    }
}
?>
