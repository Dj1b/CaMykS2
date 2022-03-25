<?php
/**
 * @brief SystemConfigurationManager admin module, configuration library.
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
 * SystemConfigurationManagerConfiguration class.
 */
final class SystemConfigurationManagerConfiguration extends PluginConfiguration {
    /**
     * Class constructor.
     * @return void
     */
    public function __construct(&$parent, $name='config') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* Login attributes */
        $this->define_attribute('Login_Identifier',         'Integer',  0);
        $this->define_attribute('Login_Captcha',            'Text',     'None');

        /* Security attributes */
        $this->define_attribute('Security_2FAStatus',       'Integer',  0);
        $this->define_attribute('Security_2FADelay',        'Integer',  0);

        /* Password attributes */
        $this->define_attribute('Password_MinLength',       'Integer', 6);
        $this->define_attribute('Password_CheckComplexity', 'Integer', 0);
        $this->define_attribute('Password_CheckMostUsed',   'Integer', 0);
        $this->define_attribute('Password_UpdateDelay',     'Integer', 0);
        $this->define_attribute('Password_AllowReuse',      'Integer', 1);
        $this->define_attribute('Password_LostMode',        'Integer', 0);
    }

    /* Data methods */

    /**
     * Return login identifier values.
     * @return array
     */
    public function get_loginIdentifierList() {
        return array(
            0 => $this->get_locale('LoginIdentifier_LoginEmail'),
            1 => $this->get_locale('LoginIdentifier_EmailOnly'),
            2 => $this->get_locale('LoginIdentifier_LoginOnly'),
        );
    }

    /**
     * Return security 2FA status values.
     * @return array
     */
    public function get_security2FAStatusList() {
        return array(
            0 => $this->get_locale('Security2FAStatus_Disabled'),
            1 => $this->get_locale('Security2FAStatus_Optional'),
            2 => $this->get_locale('Security2FAStatus_Mandatory'),
        );
    }

    /**
     * Return lost password mode values.
     * @return array
     */
    public function get_passwordLostModeList() {
        return array(
            0 => $this->get_locale('PasswordLostMode_Email'),
            1 => $this->get_locale('PasswordLostMode_Admin'),
        );
    }
}
?>
