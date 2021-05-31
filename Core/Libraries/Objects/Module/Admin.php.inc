<?php
/**
 * @brief AdminModule object.
 * <br />Sub module abstract library for admin modules.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Dec 2020
 * @date Modification: May 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * AdminModule class.
 */
abstract class AdminModule extends Module {
    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);

        /* Override Module variables */
        $this->moduleType = 'Admin';
    }

    /**
     * Return button for admin control panel.
     * @return false|array
     */
    public function get_adminControlPanelButton() {
        return false;

        return array(
            /*
            array(
                'title'         => '',
                'description'   => '',
                'icon'          => '',
                'category'      => '',
                'categoryTitle' => '', // Optional : to be defined only for non-generic categories
                'defaultURL'    => '', // Optional : to be defined only for special default URL
            ),
            */
        );
    }
}
?>
