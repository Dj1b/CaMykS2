<?php
/**
 * @brief SystemUniqueIdManager admin module, configuration library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2022
 * @date Modification: Oct 2026
 * @copyright 2022 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemUniqueIdManager;
/**
 * Configuration class.
 */
final class Configuration extends \CaMykS2\PluginConfiguration {
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
