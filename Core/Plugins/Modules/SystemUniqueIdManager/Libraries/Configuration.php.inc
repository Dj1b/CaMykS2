<?php
/**
 * @brief SystemUniqueIdManager admin module, configuration library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2022
 * @date Modification: Feb 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemUniqueIdManagerConfiguration class.
 */
final class SystemUniqueIdManagerConfiguration extends PluginConfiguration {
    /**
     * Class constructor.
     * @return void
     */
    public function __construct(&$parent, $name='config') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* Generating attributes */
        $this->define_attribute('Generating_Format',        'ListItem', 'Text', array('Values'=> array('Text', 'UUID')));
        $this->define_attribute('Generating_TextLength',    'Integer',  16);
        $this->define_attribute('Generating_MaxAttempts',   'Integer',  10);
    }
}
?>
