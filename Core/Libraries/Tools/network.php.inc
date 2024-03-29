<?php
/**
 * @brief Network methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2020
 * @date Modification: Jan 2022
 * @copyright 2020 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Generic IP methods */

/**
 * Check IP format.
 * @param string $ip
 * @return string
 */
function network_checkIPFormat($ip) {
    if (network_isIPv4Valid($ip)) return 'IPv4';
    if (network_isIPv6Valid($ip)) return 'IPv6';
    if (network_isIPCIDRValid($ip)) return 'CIDR';
    return '';
}

/**
 * Check IP is valid.
 * @param string $ip
 * @return boolean result
 */
function network_isIPValid($ip) {
    if (network_isIPv4Valid($ip)) return true;
    return network_isIPv6Valid($ip);
}

/* IPv4 methods */

/**
 * Check IP is a valid IPv4.
 * @param string $ip
 * @return boolean result
 */
function network_isIPv4Valid($ip) {
    return filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4);
}

/**
 * Check IP is a valid CIDR.
 * @param string $cidr
 * @return boolean result
 */
function network_isIPCIDRValid($cidr) {
    list ($ip, $mask) = explode ('/', $cidr);
    return (!is_null($mask) and $mask > 0 and $mask < 24 and filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4));
}

/**
 * Check IP is in given CIDR.
 * @param string $ip
 * @param string $cidr
 * @return boolean result
 */
function network_isIPv4InCIDR($ip, $cidr) {
    list ($net, $mask) = explode ('/', $cidr);

    $ip_net = ip2long($net);
    $ip_mask = ~((1 << (32 - $mask)) - 1);

    $ip_ip = ip2long($ip);

    return (($ip_ip & $ip_mask) == ($ip_net & $ip_mask));
}

/* IPv6 methods */

/**
 * Check IP is a valid IPv6.
 * @param string $ip
 * @return boolean result
 */
function network_isIPv6Valid($ip) {
    return filter_var($ip, FILTER_VALIDATE_IP, FILTER_FLAG_IPV6);
}
?>
