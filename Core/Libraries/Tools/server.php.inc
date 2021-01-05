<?php
/**
 * @brief Server specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Dec 2019
 * @date Modification: Jan 2021
 * @copyright 2019 - 2021 CaMykS Team
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
 * Return server software module list.
 * @return array
 */
function server_getSoftwareModules() {
    /* Return Apache httpd modules */
    if (server_getSoftwareName() == 'Apache') {
        return apache_get_modules();
    }

    /* For other software, return empty list */
    return array();
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
 * Check if required modules are available on server software.
 * @return array
 */
function server_checkModuleRequirements() {
    /* Do the check for Apache httpd */
    if (server_getSoftwareName() == 'Apache') {
        /* Load available modules */
        $availableModules = apache_get_modules();

        /* Define CaMykS requirements */
        $requirements = array(
            'Required' => array(
                array('name' => 'mod_php', 'alternatives'=>array('mod_php7')),
                array('name' => 'mod_rewrite', 'alternatives'=>array()),
            ),
            'Recommended' => array(
                array('name' => 'mod_headers', 'alternatives'=>array()),
            ),
        );

        /* Initialise result */
        $result = array();

        /* Build result */
        foreach ($requirements as $level => $modules) {
            $result[$level] = array();
            foreach ($modules as $module) {
                if (in_array($module['name'], $availableModules))
                    $result[$level][$module['name']] = true;
                else {
                    foreach ($module['alternatives'] as $alternative) {
                        if (in_array($alternative, $availableModules)) {
                            $result[$level][$alternative] = true;
                            continue 2;
                        }
                    }
                    $result[$level][$module['name']] = false;
                }
            }
        }

        /* Return result */
        return $result;
    }

    /* For other software, return empty list */
    return array();
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
