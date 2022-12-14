<?php
/**
 * @brief PHP related methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2021
 * @date Modification: Dec 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Return PHP's error reporting level as string.
 * @param integer $errorLevel
 * @param string $separator
 * @return string
 */
function php_displayErrorReportingLevelAsString($errorLevel=null, $separator=', ') {
    /* Check level value */
    if (is_null($errorLevel)) $errorLevel = error_reporting();

    /* Define error level list */
    $errorLevels = array(
        E_ALL               => 'E_ALL',
        E_USER_DEPRECATED   => 'E_USER_DEPRECATED',
        E_DEPRECATED        => 'E_DEPRECATED',
        E_RECOVERABLE_ERROR => 'E_RECOVERABLE_ERROR',
        E_STRICT            => 'E_STRICT',
        E_USER_NOTICE       => 'E_USER_NOTICE',
        E_USER_WARNING      => 'E_USER_WARNING',
        E_USER_ERROR        => 'E_USER_ERROR',
        E_COMPILE_WARNING   => 'E_COMPILE_WARNING',
        E_COMPILE_ERROR     => 'E_COMPILE_ERROR',
        E_CORE_WARNING      => 'E_CORE_WARNING',
        E_CORE_ERROR        => 'E_CORE_ERROR',
        E_NOTICE            => 'E_NOTICE',
        E_PARSE             => 'E_PARSE',
        E_WARNING           => 'E_WARNING',
        E_ERROR             => 'E_ERROR',
    );

    /* Initialise results */
    $result = array();

    /* Tests all levels */
    foreach ($errorLevels as $level => $name) {
        if (($errorLevel & $level) == $level)
            $result[] = $name;
    }

    /* Return result */
    return implode($separator, $result);
}

/**
 * Check if required PHP modules are available.
 * @return array
 */
function php_checkModuleRequirements() {
    /* Load available modules */
    $availableModules = get_loaded_extensions();

    /* Define CaMykS requirements */
    $requirements = array(
        'Required' => array(
            'mbstring',
            'json',
            'intl',
        ),
        'Dependences' => array(
            /* Cryptographer */
            'openssl',
            'mcrypt',
            'sodium',

            /* FTPClient */
            'ftp',

            /* Image */
            'gd',

            /* http */
            'curl',

            /* */
            'zip',
        ),
    );

    /* Initialise result */
    $result = array();

    /* Build result */
    foreach ($requirements as $level => $modules) {
        $result[$level] = array();
        foreach ($modules as $module) {
            $result[$level][$module] = in_array($module, $availableModules);
        }
    }

    /* Return result */
    return $result;
}
?>
