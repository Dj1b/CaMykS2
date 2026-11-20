<?php
/**
 * @brief SystemPrivacyPolicyManager admin module, Configuration library.
 * @details Plugin / Module Library / Configuration
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemPrivacyPolicyManager;

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

        /* Define data pruning attributes */
        $this->define_attribute('PruneOldData_Method', 'ListItem', 'None', array('Values' => array('None', 'Anonymise', 'Delete')));
        $this->define_attribute('PruneOldData_Delay',  'Integer',  12,     array('Min' => 0));
    }

    /* Mark: PruneOldData method helpers */

    /**
     * Return prune old data method list.
     * @return array
     */
    public function get_pruneOldDataMethodList(): array {
        global $CaMykS;

        return array(
            'None'      => $CaMykS->get_locale('PruneOldDataMethod_None', '', array(), 'None'),
            'Anonymise' => $CaMykS->get_locale('PruneOldDataMethod_Anonymise', '', array(), 'Anonymise'),
            'Delete'    => $CaMykS->get_locale('PruneOldDataMethod_Delete', '', array(), 'Delete'),
        );
    }

    /**
     * Return prune old data method title.
     * @param mixed $method
     * @return string
     */
    public function get_pruneOldDataMethodTitle(mixed $method=null): string {
        if (is_null($method))
            $method = $this->PruneOldData_Method;

        $list = $this->get_pruneOldDataMethodList();
        if (isset($list[$method]))
            return $list[$method];
        return (string)$method;
    }
}
?>
