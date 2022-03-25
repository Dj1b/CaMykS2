<?php
/**
 * @brief MediaPopup component, configuration library.
 * @details Plugin / Component Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Nov 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * MediaPopupConfiguration class.
 */
final class MediaPopupConfiguration extends PluginConfiguration {
    /**
     * Class constructor.
     * @return void
     */
    public function __construct(&$parent, $name='config') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* Integration parameters */
        $this->define_attribute('Integration_Mode', 'Integer', 0); // 0 : manual / 1 : automatic
        $this->define_attribute('Integration_StyleSource', 'Text', 'plugin'); // plugin / theme
        $this->define_attribute('Integration_ButtonSource', 'Text', 'default'); // default / theme .... custom ?

        /* Control parameters */
        $this->define_attribute('Control_ClickBkgdToClose', 'Integer', 1);
        $this->define_attribute('Control_DisplayCloseButton', 'Integer', 1);
        $this->define_attribute('Control_EnableKeyboard', 'Integer', 1);

        /* Diaporama parameters */
        $this->define_attribute('Diaporama_Enable', 'Integer', 0);
    }
}
?>
