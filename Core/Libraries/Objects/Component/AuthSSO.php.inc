<?php
/**
 * @brief AuthSSOComponent object.
 * <br />Sub component abstract library for SSO authentication components.
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
 * AuthSSOComponent class.
 */
abstract class AuthSSOComponent extends Component {
    /**
     * @var string $ssoProtocol
     * @brief SSO protocol name (e.g. OAuth, SAML).
     */
    protected string $ssoProtocol = '';

    /**
     * Class constructor.
     * @param string|null $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Override Component variables */
        $this->componentType = 'AuthSSO';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Data related methods */

    /**
     * Return SSO protocol value.
     * @return string
     */
    public function get_ssoProtocol(): string {
        return $this->ssoProtocol;
    }

    /* Mark: User SSO configuration methods */

    /**
     * Initialise SSO configuration for user.
     * @param array $params
     * @return boolean success
     */
    abstract public function initialise_userConfiguration(array $params=array()): bool;

    /**
     * Display SSO configuration for user.
     * @return boolean success
     */
    public function display_userConfiguration(): bool {
        $this->update_selectedLayout('UserConfiguration');
        return $this->display();
    }

    /**
     * Confirm SSO configuration for user.
     * @param array $params
     * @return string|false
     */
    abstract public function confirm_userConfiguration(array $params=array()): string|false;

    /**
     * Reset SSO configuration for user.
     * @param array $params
     * @return boolean success
     */
    abstract public function reset_userConfiguration(array $params=array()): bool;

    /* Mark: User SSO login methods */

    /**
     * Initialise SSO login for user.
     * @param array $params
     * @return boolean success
     */
    abstract public function initialise_userLogin(array $params=array()): bool;

    /**
     * Display SSO login for user.
     * @return boolean success
     */
    public function display_userLogin(): bool {
        $this->update_selectedLayout('UserLogin');
        return $this->display();
    }

    /**
     * Confirm SSO login for user.
     * @param array $params
     * @return string|false
     */
    abstract public function confirm_userLogin(array $params=array()): string|false;

    /* Mark: External user profile methods */

    /**
     * Return normalised user information retrieved from SSO provider.
     * @return array
     */
    abstract public function get_userInfo(): array;
}
?>
