<?php
/**
 * @brief SystemSessionManager admin module.
 * <br />Module to manage visitors (and admin users) sessions.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Jun 2021
 * @date Modification: Nov 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemSessionManager;

use CaMykS2\AdminModule;

/**
 * SystemSessionManager class.
 */
final class SystemSessionManager extends AdminModule {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected bool $hasConfiguration = false;

    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected array $libraries = array('Session', 'Session\\Request');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }
}
?>
