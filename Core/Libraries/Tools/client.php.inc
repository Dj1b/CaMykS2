<?php
/**
 * @brief Client specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Dec 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Return client IP address.
 * @return string
 */
function client_getIp(): string {
    if (!empty($_SERVER['HTTP_CLIENT_IP']))
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR']))
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    elseif (!empty($_SERVER['HTTP_X_FORWARDED']))
        $ip = $_SERVER['HTTP_X_FORWARDED'];
    elseif (!empty($_SERVER['HTTP_FORWARDED_FOR']))
        $ip = $_SERVER['HTTP_FORWARDED_FOR'];
    elseif (!empty($_SERVER['HTTP_FORWARDED']))
        $ip = $_SERVER['HTTP_FORWARDED'];
    elseif (!empty($_SERVER['REMOTE_ADDR']))
        $ip = $_SERVER['REMOTE_ADDR'];
    else
        return '';

    if (network_isIPValid($ip))
        return $ip;
    return '';
}

/**
 * Check if page is loaded in HTTPS(SSL).
 * @return boolean result
 */
function client_isHTTPS(): bool {
    return (!empty($_SERVER['HTTPS']) or (isset($_SERVER['HTTP_X_FORWARDED_PROTO']) and $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https'));
}

/**
 * Return current request URL.
 * @return string
 */
function client_getCurrentURL(): string {
    return 'http'.(client_isHTTPS() ? 's' : '').'://'.$_SERVER['HTTP_HOST'].strtok($_SERVER["REQUEST_URI"], '?');
}

/**
 * Return client referer site.
 * @return string
 */
function client_getRefererSite(): string {
    return (isset($_SERVER['HTTP_REFERER'])?$_SERVER['HTTP_REFERER']:'');
}

/**
 * Check if the server is also the client.
 * @return boolean result
 */
function client_isServerClient(): bool {
    return ($_SERVER['SERVER_ADDR'] == client_getIp());
}

/**
 * Check if client is marked as Do Not Track.
 * @return boolean result
 */
function client_getDoNotTrack(): bool {
    return (isset($_SERVER['HTTP_DNT']) and $_SERVER['HTTP_DNT'] == 1);
}

/**
 * Return client user agent.
 * @return string
 */
function client_getUserAgent(): string {
    return (isset($_SERVER['HTTP_USER_AGENT'])?$_SERVER['HTTP_USER_AGENT']:'');
}

/**
 * Return client user agent details.
 * @return string
 * @todo
 */
function client_getUserAgentDetails(): string {
    return array();
}
?>
