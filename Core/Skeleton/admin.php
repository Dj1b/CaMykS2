<?php
/**
 * @brief Admin site entry point
 * @details Skeleton site / Site index
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: May 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
/* Load site configuration */
if (!(@include_once('./Configuration/Website.php.inc')))
    exit('Fatal Error: Website configuration not found.');

/* Load CaMykS */
if (!(@include_once($configuration['Website_CaMykSPath'].'/CaMykS.php.inc')))
    exit('Fatal Error: CaMykS engine not found.');

/* Create CaMykS engine object */
$CaMykS = new CaMykS2\CaMykS($configuration);

/* Execute CaMykS engine */
$CaMykS->execute(CAMYKS_MODE_ADMIN);
?>