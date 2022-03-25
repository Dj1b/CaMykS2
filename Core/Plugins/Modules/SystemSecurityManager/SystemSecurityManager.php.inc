<?php
/**
 * @brief SystemSecurityManager admin module.
 * <br />Module to manage security in the website.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jun 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemSecurityManager class.
 */
final class SystemSecurityManager extends AdminModule {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected $hasConfiguration = true;

    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected $libraries = array('FilterRule');

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
