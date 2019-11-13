<?php
/**
 * @brief URL specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */

/**
 * Check if provided urls are in the same domain.
 * @param array $urls
 * @param boolean $strict check also subdomain if true
 * @return boolean result
 */
function url_areInSameDomain($urls=array(), $strict=false) {
    if (!is_array($urls))
      return false;

    $domain = '';
    foreach ($urls as $url) {
        /* Load hostname from url */
        $v = strToLower(parse_url($url, PHP_URL_HOST));

        if ($domain == '') {
            /* Initialise first value. */
            if ($strict)
                $domain = $v;
            else {
                $v = explode('.', $v);
                if (count($v) < 2)
                    return false;
                $domain = $v[count($v)-2].'.'.$v[count($v)-1];
            }
        } elseif ($strict) {
            /* Check domain in strict mode. */
            if ($domain != $v)
                return false;
        } else {
            /* Check domain in non-strict mode. */
            $v = explode('.', $v);
            if (count($v) < 2)
                return false;
            if ($domain != $v[count($v)-2].'.'.$v[count($v)-1])
                return false;
        }
    }

    /* Return success */
    return true;
}

/**
 * Return completed URL with scheme when needed.
 * @param string $url
 * @param string $scheme
 * @return string
 */
function url_getAsComplete($url, $scheme='http') {
    /* Trim URL */
    $url = trim($url);

    /* Check URL */
    if ($url == '')
        return '';

    /* Check if URL already have scheme */
    if (!preg_match('#:\/\/#', $url))
        $url = $scheme.'://'.$url;

    /* Return URL */
    return $url;
}

/**
 * Return smart display for URL.
 * @param string $url
 * @return string
 */
function url_getAsSmart($url) {
    /* Split URL scheme and host/path */
    $url = explode('://', trim($url));

    /* Return host/path value */
    if (count($url) == 1)
        return $url[0];
    return $url[1];
}
?>
