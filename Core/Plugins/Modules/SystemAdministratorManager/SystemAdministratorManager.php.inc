<?php
/**
 * @brief SystemAdministratorManager admin module.
 * <br />Module to manage administration users and groups.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Jun 2021
 * @date Modification: Nov 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemAdministratorManager;

use CaMykS2\AdminModule;

/**
 * SystemAdministratorManager class.
 */
final class SystemAdministratorManager extends AdminModule {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected bool $hasConfiguration = true;

    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected array $libraries = array('User', 'UserPermission', 'UserPreference', 'Group', 'GroupPermission', 'UserGroup');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Installation specific methods */

    /**
     * Check initial administrator account data before website installation finalisation.
     * @param array $data
     * @return boolean success
     */
    public function check_initialAdministratorData(array $data=array()): bool {
        $user = $this->get_libraryInstance('User');
        if ($user === false)
            return false;

        $user->initialise();
        $user->FirstName = $data['FirstName'] ?? '';
        $user->LastName  = $data['LastName'] ?? '';
        $user->Email     = $data['Email'] ?? '';
        $user->Login     = !empty($data['Login']) ? $data['Login'] : ($data['Email'] ?? '');
        $user->Password  = $data['Password'] ?? '';
        $user->Status    = 2;
        $user->Type      = 'Owner';

        return $user->check_data();
    }

    /**
     * Create initial administrator account during website installation.
     * @param array $data
     * @return boolean success
     */
    public function create_initialAdministrator(array $data=array()): bool {
        /* Check if a user already exists */
        $user = $this->get_libraryInstance('User');
        if ($user === false or $user->get_count() > 0)
            return false;

        /* Define attributes */
        $user->FirstName = $data['FirstName'] ?? '';
        $user->LastName  = $data['LastName'] ?? '';
        $user->Email     = $data['Email'] ?? '';
        $user->Login     = !empty($data['Login']) ? $data['Login'] : ($data['Email'] ?? '');
        $user->Password  = $data['Password'] ?? '';
        $user->Status    = 2; // Enabled
        $user->Type      = 'Owner';

        /* Save user */
        return $user->save();
    }
}
?>
