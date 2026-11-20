<?php
/**
 * @brief SystemLogManager admin module, Configuration library.
 * @details Plugin / Module Library / Configuration
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemLogManager;

use CaMykS2\PluginConfiguration;

/**
 * Configuration class.
 */
final class Configuration extends PluginConfiguration {
    /**
     * Class constructor.
     * @param mixed $parent
     * @param string $name
     * @return void
     */
    public function __construct(&$parent, string $name='config') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* Define file log attributes */
        $this->define_attribute('File_MaxRotationSize', 'Integer', 5, array('Min' => 1));
        $this->define_attribute('File_MaxKeptFiles',     'Integer', 10, array('Min' => 1));
    }
}
?>
