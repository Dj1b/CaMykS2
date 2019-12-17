<?php
/**
 * @brief Util methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Dec 2019
 * @date Modification: Dec 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */

/**
 * Return human readable size.
 * @param integer $size
 * @param string $format
 * @return string
 */
function util_getHumanReadableSize($size, $format='Binary') {
    /* Check size */
    $size = (int)$size;

   /* Check format */
    if ($format == 'Decimal')
        $divider = 1000.00;
    else
        $divider = 1024.00;

    /* Load unit to display */
    $t = language_getFileUnit();

    /* List of multiples */
    $l = array('', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y');

    /* Reduce to correct multiple */
    $s = 0;
    while ($size > $divider) {
        $s++;
        $size /= $divider;
    }
    $size = ((int)($size*10)/10);

    if ($s == 0)
        return $size.' '.($t == 'o' ? 'octet' : 'byte');
    return $size.' '.$l[$s].$t;
}
?>
