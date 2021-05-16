<?php
/**
 * @brief URL specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.3
 * @date Creation: Nov 2019
 * @date Modification: May 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Return current URL.
 * @return string
 */
function url_getCurrentURL() {
     return ((isset($_SERVER['HTTPS']) and $_SERVER['HTTPS'] == 'on') ? 'https://' : 'http://').$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
}

/**
 * Return base URL from mode.
 * @param string $mode
 * @param string $URL
 * @return string
 */
function url_getBaseURLFromMode($mode='Relative', $URL=null) {
    switch ($mode) {
        case 'Absolute':
        case 'AbsoluteLong':
            if (is_null($URL)) {
                $URL = parse_url($_SERVER['REQUEST_URI']);
                $URL = $URL['scheme'].'://'.$URL['host'];
            }
            return $URL.'/';
        case 'AbsoluteShort':
            return '/';
        case 'Relative':
        default:
            return '';
    }
    return '';
}

/**
 * Return available base URL modes.
 * @return array
 */
function url_getBaseURLModes() {
    global $CaMykS;

    /* Load locales file */
    $CaMykS->load_engineLocales('Libraries/url', 'url');

    /* Return list */
    return array(
        'AbsoluteLong'  => $CaMykS->get_locale('BaseURLMode_Absolute_Long', 'url'),
        'AbsoluteShort' => $CaMykS->get_locale('BaseURLMode_Absolute_Short', 'url'),
        'Relative'      => $CaMykS->get_locale('BaseURLMode_Relative', 'url'),
    );
}

/**
 * Return URL with given SSL mode.
 * @param string $mode
 * @param string $URL
 * @return string
 */
function url_getURLWithSSLMode($mode='Default', $URL=null) {
    /* Check URL */
    $tempURL = url_getCurrentURL();
    if (is_null($URL))
        $URL = $tempURL;

    /* Parse URL */
    $parsedURL = parse_url($URL);
    $parseTempURL = parse_url($tempURL);

    /* Check scheme from SSLMode */
    switch ($mode) {
        case 'HTTPS' : // Forced to HTTPS
            $parsedURL['scheme'] = 'https';
            break;
        case 'HTTP' : // Forced to HTTP
            $parsedURL['scheme'] = 'http';
            break;
        case 'Auto' : // Autocheck
            /* @todo */
            break;
        case 'Default' : // default / client's choice
        default :
            /* Do not change scheme */
    }

    /* Finalise URL */
    return $parsedURL['scheme'].'://'.
        (!empty($parsedURL['host']) ? $parsedURL['host'] : $parseTempURL['host']).'/'.
        (!empty($parsedURL['path']) ? $parsedURL['path'].'/' : '').
        (!empty($parsedURL['query']) ? $parsedURL['query'].'/' : '');
}

/**
 * Return available SSL modes.
 * @return array
 */
function url_getSSLModes() {
    global $CaMykS;

    /* Load locales file */
    $CaMykS->load_engineLocales('Libraries/url', 'url');

    /* Return list */
    return array(
        'HTTPS'     => $CaMykS->get_locale('SSLMode_HTTPS', 'url'),
        'HTTP'      => $CaMykS->get_locale('SSLMode_HTTP', 'url'),
        'Auto'      => $CaMykS->get_locale('SSLMode_Auto', 'url'),
        'Default'   => $CaMykS->get_locale('SSLMode_Default', 'url'),
    );
}

/**
 * Check if provided urls have same host.
 * @param array $urls
 * @param boolean $strict check also subdomain if true
 * @return boolean result
 */
function url_haveSameHost($urls=array(), $strict=false) {
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
    /* Remove tailing / char */
    $url = preg_replace('#/$#', '', $url);

    /* Split URL scheme and host/path */
    $url = explode('://', trim($url));

    /* Return host/path value */
    if (count($url) == 1)
        return $url[0];
    return $url[1];
}
?>
