<?php
/**
 * @brief Auth2FAComponent object.
 * <br />Sub component abstract library for 2FA authentication components.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Nov 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Auth2FAComponent class.
 */
abstract class Auth2FAComponent extends Component {
    /**
     * Class constructor.
     * @param string|null $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Override Component variables */
        $this->componentType = 'Auth2FA';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: User 2FA configuration methods */

    /**
     * Initialise 2FA configuration for user.
     * @param array $params
     * @return boolean success
     */
    abstract public function initialise_userConfiguration(array $params=array()): bool;

    /**
     * Display 2FA configuration for user.
     * @return boolean success
     */
    public function display_userConfiguration(): bool {
        $this->update_selectedLayout('UserConfiguration');
        return $this->display();
    }

    /**
     * Confirm 2FA configuration for user.
     * @param array $params
     * @return string|false
     */
    abstract public function confirm_userConfiguration(array $params=array()): string|false;

    /**
     * Reset 2FA configuration for user.
     * @param array $params
     * @return boolean success
     */
    abstract public function reset_userConfiguration(array $params=array()): bool;

    /* Mark: User 2FA login methods */

    /**
     * Initialise 2FA login for user.
     * @param array $params
     * @return boolean success
     */
    abstract public function initialise_userLogin(array $params=array()): bool;

    /**
     * Display 2FA login for user.
     * @return boolean success
     */
    public function display_userLogin(): bool {
        $this->update_selectedLayout('UserLogin');
        return $this->display();
    }

    /**
     * Confirm 2FA login for user.
     * @param array $params
     * @return boolean result
     */
    abstract public function confirm_userLogin(array $params=array()): bool;
}
?>
