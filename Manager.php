<?php
/**
 * @brief Manager entry point for CaMykS2.
 * @details Engine / Index Page
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 2.0a
 * @date Creation: Feb 2017
 * @date Modification: Sep 2019
 * @copyright 2017 - 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Start session */
session_start();

/* Load CaMykS */
if ((@include_once('CaMykS.php.inc')) === false)
    exit('Fatal Error : CaMykS folder not found.');

/* Create CaMykS engine object */
$CaMykS = new CaMykS(null);

/* Execute CaMykS engine */
$CaMykS->execute(CAMYKS_MODE_MANAGER);

?>
