<?php
/**
 * @brief Password specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Encode password.
 * @param string $password
 * @param mixed $algorythm
 * @param string $salt
 * @return string
 */
function password_encode($password, $algorythm=null, $salt='') {
    switch ($algorythm) {
        case 128:
        case 'sha512':
            return hash('sha512', $password.$salt);
        case 96:
        case 'sha384':
            return hash('sha384', $password.$salt);
        case 64:
        case 'sha256':
            return hash('sha256', $password.$salt);
        case 40:
        case 'sha1':
            return hash('sha1', $password.$salt);
        case 32:
        case 'md5':
        default :
            return hash('md5', $password.$salt);
    }
}

/**
 * Check password.
 * @param string $password
 * @param string $hash
 * @param string $algorythm
 * @param string $salt
 * @return boolean result
 */
function password_check($password, $hash, $algorythm=null, $salt='') {
    /* Check for integrated salt in hash */
    if ($salt == '' and preg_match('#\:[0-9a-z]*$#', $hash)) {
        $hash = explode(':', $hash);
        $salt = $hash[0];
        $hash = $hash[1];
    }

    /* Load algorythm from its length */
    if (is_null($algorythm)) $algorythm = strlen($hash);

    /* Compare password and hash */
    return ($hash == password_encode($password, $algorythm, $salt));
}
?>
