<?php
/**
 * @brief SystemWorkflowManager admin module, Configuration library.
 * @details Plugin / Module Library / Configuration
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Nov 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemWorkflowManager;

use CaMykS2\Plugin;
use CaMykS2\PluginConfiguration;

/**
 * Configuration class.
 */
final class Configuration extends PluginConfiguration {
    /**
     * Class constructor.
     * @param Plugin $parent
     * @param string $name
     * @return void
     */
    public function __construct(Plugin &$parent, string $name='config') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* General workflow settings */
        $this->define_attribute('Status',            'Integer',  0, array('Min' => 0, 'Max' => 1, 'Unsigned' => true));
        $this->define_attribute('ValidationLevels',  'Integer',  1, array('Min' => 1, 'Max' => 3, 'Unsigned' => true));
        $this->define_attribute('PublicationMode',   'Integer',  0, array('Min' => 0, 'Max' => 1, 'Unsigned' => true));
        $this->define_attribute('Notifications',     'Integer',  0, array('Min' => 0, 'Max' => 1, 'Unsigned' => true));

        /* Administrator groups settings */
        $this->define_attribute('EditionGroup',      'ObjectId', 0);
        $this->define_attribute('Validation1Group',  'ObjectId', 0);
        $this->define_attribute('Validation2Group',  'ObjectId', 0);
        $this->define_attribute('Validation3Group',  'ObjectId', 0);
        $this->define_attribute('PublicationGroup',  'ObjectId', 0);
    }
}
?>
