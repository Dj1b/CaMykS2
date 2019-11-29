<?php
/**
 * @brief File specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Check if element is in path.
 * @param string $element
 * @param string $path
 * @return boolean result
 */
function file_checkIsInPath($element='', $path='') {
    /* Check path exists, is not empty, and is a directory */
    if (!file_exists($path) or !is_dir($path) or $path == '')
        return false;
    $path = realpath($path).'/';

    /* Get element real path */
    if (file_exists($element))
        $element = realpath($element);
    else
        $element = file_getVirtualRealPath($element);

    /* Check element */
    if ($element == '')
        return false;

    /* Check folder is in path */
    return preg_match('#^'.$path.'#', $element);
}

/**
 * Return virtual real path for element.
 * @param string $element
 * @return string
 */
function file_getVirtualRealPath($element='') {
    /* Explode element path in array */
    $path = explode('/', $element);

    /* Remove leading empty folders */
    while ($path[0] == '')
        array_shift($path);

    /* Check root real path */
    $path[0] = realpath($path[0]);
    if ($path[0] === '')
        return '';

    /* Implode back path */
    return implode('/', $path);
}

/**
 * Search a file in a folder.
 * @param string $pattern
 * @param string $rootPath
 * @param integer $globFlags
 * @return array
 */
function file_search($pattern='*', $rootPath='./', $globFlags=0) {
    /* Check source is accessible */
    if (folder_checkPathAccessibility($rootPath, 'Read') === false)
        return array();

    /* Do the search */
    $paths = glob($rootPath.'*', GLOB_MARK|GLOB_ONLYDIR|GLOB_NOSORT);
    $files = glob($rootPath.$pattern, $globFlags);
    foreach ($paths as $path) {
        $files = array_merge($files, file_search($pattern, $path, $globFlags));
    }
    return $files;
}

?>
