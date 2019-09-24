<?php
/**
 * @brief CaMykS version 2 CMS master class.
 * @details Engine / Main Object
 * @file CaMykS.php
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a
 * @date Creation: Feb 2017
 * @date Modification: Sep 2019
 * @copyright 2017 - 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * AdminoTemplateMaster class.
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

/* check for manager execution */
if ($_REQUEST['SCRIPT_FILENAME'] === 'CaMykS.php') {
    $CaMykS = new CaMykS();
    /* execute CaMykS engine */
    $CaMykS->execute('manager');
}
?>
