<?php
/**
 * @brief File specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Nov 2019
 * @date Modification: Nov 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Check if element is in path.
 * @param string $element
 * @param string $path
 * @return boolean result
 */
function file_checkIsInPath(string $element='', string $path=''): bool {
    if ($path === '')
        return false;

    /* Get path real or virtual path */
    if (file_exists($path) && is_dir($path))
        $pathFolder = realpath($path);
    else
        $pathFolder = file_getVirtualRealPath($path);

    if ($pathFolder === '')
        return false;

    /* Get element real or virtual path */
    if (file_exists($element))
        $element = realpath($element);
    else
        $element = file_getVirtualRealPath($element);

    /* Check element */
    if ($element == '')
        return false;

    /* Check folder is in path */
    return (bool)preg_match('#^' . preg_quote($pathFolder, '#') . '#', $element);
}

/**
 * Return virtual real path for element.
 * @param string $element
 * @return string
 */
function file_getVirtualRealPath(string $element=''): string {
    $isAbsolute = str_starts_with($element, '/');

    /* Explode element path in array */
    $path = explode('/', $element);

    /* Remove leading empty folders */
    while (isset($path[0]) && $path[0] == '')
        array_shift($path);

    if (empty($path))
        return '';

    if ($isAbsolute)
        $path[0] = '/' . $path[0];

    /* Check root real path */
    $real = realpath($path[0]);
    if ($real === false || $real === '')
        return '';
    $path[0] = $real;

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
function file_search(string $pattern='*', string $rootPath='./', int $globFlags=0): array {
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

/**
 * Return image type for given file.
 * @param string $file
 * @return string
 */
function file_getImageType(string $file): string {
    /* Return image type from EXIF */
    if (function_exists('exif_imagetype') and file_exists($file) and ($type = @exif_imagetype($file)) !== false) {
        switch ($type) {
            case 1: return 'gif';
            case 2: return 'jpg';
            case 3: return 'png';
            case 5: return 'psd';
            case 6: return 'bmp';
            case 18: return 'webp';
        }
    }

    /* By default return type from its extension */
    if (preg_match('/\.([^.]{1,4})$/', $file, $ext))
        return strtolower($ext[1]);
    return '';
}

?>
