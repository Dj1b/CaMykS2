<?php
/**
 * @brief Manager entry point for CaMykS2.
 * @details Engine / Index Page
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a2
 * @date Creation: Feb 2017
 * @date Modification: Mar 2020
 * @copyright 2017 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Start session */
session_start();

/* Load CaMykS */
if ((@include_once('CaMykS.php.inc')) === false)
    exit('Fatal Error : CaMykS engine not found.');

/* Create CaMykS engine object */
$CaMykS = new CaMykS2\CaMykS();

/* Execute CaMykS engine */
$CaMykS->execute(CAMYKS_MODE_MANAGER);

?>
