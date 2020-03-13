<?php
/**
 * @brief Server specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Dec 2019
 * @date Modification: Mar 2020
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Server software related methods */

/**
 * Return server software description.
 * @return array
 */
function server_getSoftware() {
    if (preg_match('#^([A-Za-z]*)/([0-9\.])#', $_SERVER['SERVER_SOFTWARE'], $matches))
        return array('Name'=>$matches[1], 'Version'=>$matches[2]);
    return array('Name'=>'', 'Version'=>'');
}

/**
 * Return server software name.
 * @return string
 */
function server_getSoftwareName() {
    $data = server_getSoftware();
    return $data['Name'];
}

/**
 * Return server software version.
 * @return string
 */
function server_getSoftwareVersion() {
    $data = server_getSoftware();
    return $data['Version'];
}

/**
 * Check if module is available on server software.
 * @param string $module
 * @return boolean result
 */
function server_checkModuleAvailability($module='') {
    /* Do the check for Apache httpd */
    if (server_getSoftwareName() == 'Apache') {
        return in_array($module, apache_get_modules());
    }

    /* For other software, trust installation */
    return true;
}

/**
 * Return server document root.
 * @return string
 */
function server_getDocumentRoot() {
    /* Do the check for Apache httpd */
    switch (server_getSoftwareName()) {
        case 'Apache': return getEnv('DOCUMENT_ROOT');
    }

    /* No software found, return defaut value */
    return '';
}

/* Server operating system related methods */

/**
 * Return server operating system description.
 * @return array
 */
function server_getOperatingSystem() {
    return server_getOperatingSystemName().' '.server_getOperatingSystemVersion();
}

/**
 * Return server operating system name.
 * @return string
 */
function server_getOperatingSystemName() {
    return php_uname('s');
}

/**
 * Return server operating system version.
 * @return string
 */
function server_getOperatingSystemVersion() {
    return php_uname('r').' ('.php_uname('m').')';
}
?>
