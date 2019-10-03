<?php
/**
 * @brief CaMykS version 2 CMS master class.
 * @details Engine / Master Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a
 * @date Creation: Feb 2017
 * @date Modification: Oct 2019
 * @copyright 2017 - 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * @def CAMYKS_MODE_SITE
 */
define('CAMYKS_MODE_SITE',      0);

/**
 * @def CAMYKS_MODE_ADMIN
 */
define('CAMYKS_MODE_ADMIN',     1);

/**
 * @def CAMYKS_MODE_REQUEST
 */
define('CAMYKS_MODE_REQUEST',   2);

/**
 * @def CAMYKS_MODE_MANAGER
 */
define('CAMYKS_MODE_MANAGER',   3);

/**
 * @def CAMYKS_MODE_INSTALL
 */
define('CAMYKS_MODE_INSTALL',   4);

/**
 * CaMykS class.
 */
final class CaMykS {


    /**
     * Class constructor.
     * @return void
     */
    public function __construct() {

    }

    /**
     * Execute CaMykS engine
     * @param integer $mode to run
     * @return void
     */
    public function execute($mode) {

    }
}
?>
