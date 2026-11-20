<?php
/**
 * @brief AuthSAMLComponent object.
 * <br />Sub component abstract library for SAML SSO authentication components.
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
 * AuthSAMLComponent class.
 */
abstract class AuthSAMLComponent extends AuthSSOComponent {
    /**
     * Class constructor.
     * @param string|null $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Override AuthSSOComponent variables */
        $this->ssoProtocol = 'SAML';

        /* Execute parent construct */
        parent::__construct($location);
    }
}
?>
