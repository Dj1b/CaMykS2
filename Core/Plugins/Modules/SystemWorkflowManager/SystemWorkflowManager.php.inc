<?php
/**
 * @brief SystemWorkflowManager admin module.
 * <br />Module to manage workflow-controlled objects across plugins.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Nov 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemWorkflowManager;

use CaMykS2\AdminModule;

/**
 * SystemWorkflowManager class.
 */
final class SystemWorkflowManager extends AdminModule {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected bool $hasConfiguration = true;

    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected array $libraries = array('Configuration');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Workflow status & configuration helper methods */

    /**
     * Check if workflow management is enabled.
     * @return boolean result
     */
    public function is_workflowEnabled(): bool {
        if (!$this->is_available())
            return false;
        return ((int)$this->get_configValue('Status', null, 0) === 1);
    }

    /**
     * Return configured number of validation levels (1 to 3).
     * @return integer
     */
    public function get_validationLevels(): int {
        return max(1, min(3, (int)$this->get_configValue('ValidationLevels', null, 1)));
    }
}
?>
