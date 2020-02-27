<?php
/**
 * @brief Array related methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Oct 2019
 * @date Modification: Feb 2020
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Drop given value from array.
 * @param array $array
 * @param mixed $value
 * @param boolean $keepKeys
 * @return string
 */
function array_dropValue($array=array(), $value=null, $keepKeys=true) {
    /* Check array */
    if (!is_array($array))
        return array();

    /* Check value */
    if ($value == null)
        return $array;

    /* get parsed array */
    $parsed = preg_grep("/{$value}/i", $array, PREG_GREP_INVERT);

    /* Return result */
    if ($keepKeys)
        return $parsed;
    return array_values($parsed);
}

/**
 * Apply the same value on every key then return it.
 * @param array $array
 * @param mixed $value
 * @return array
 */
function array_applySameValue($array, $value=null) {
    return array_fill_keys(array_keys($array), $value);
}

/**
 * Add a prefix to all values of an array.
 * @param array $array
 * @param string $prefix
 * @return array
 */
function array_prefixValues($array, $prefix='') {
    if ($prefix == '')
        return $array;
    array_walk($array, function(&$value, $key, $prefix) {$value = $prefix.$value;}, $prefix);
    return $array;
}

/**
 * Add a prefix to all keys of an array.
 * @param array $array
 * @param string $prefix
 * @return array
 */
function array_prefixKeys($array, $prefix='') {
    if ($prefix == '')
        return $array;
    $result = array();
    foreach ($array as $key => $value)
        $result[$prefix.$key] = $value;
    return $result;
}

/* Check functions */

/**
 * Check if all elements are the same and given type.
 * @param array $array
 * @param string $type
 * @return boolean result
 */
function array_checkDataType($array=array(), $type='') {
    if (!function_exists('is_'.$type))
        return false;
    return (count($array) === count(array_filter($array, 'is_'.$type)));
}

/**
 * Check if array is associative.
 * @param array $array
 * @return boolean result
 */
function array_isAssociative($array) {
    /* Quick tests */
    if (count($array) == 0)
        return true;

    /* Harder test */
    return array_keys($array) !== range(0, count($array) - 1);
}

/**
 * Check if array is sequential.
 * @param array $array
 * @return boolean result
 */
function array_isSequential($array) {
    /* Quick tests */
    if (count($array) == 0)
        return true;

    /* Harder test */
    return array_keys($array) === range(0, count($array) - 1);
}

/* Recursive functions */

/**
 * Merge arrays recursively.
 * @param array $array1
 * @param array $array2
 * @return array
 */
function array_mergeRecursive($array1=array(), $array2=array()) {
    /* Check arrays */
    if (!is_array($array1) and !is_array($array2))
        return array();
    if (!is_array($array1))
        return $array2;
    if (!is_array($array2))
        return $array1;

    /* Merge arrays */
    foreach ($array2 as $key => $value) {
        if (!isset($array1[$key]))
            /* add value */
            $array1[$key] = $value;
        elseif (is_array($value) and is_array($array1[$key]))
            /* Merge array values */
            $array1[$key] = array_recursiveMerge($array1[$key], $value);
        else
            /* Merge other values */
            $array1[$key] = $value;
    }

    /* Return merged array */
    return $array1;
}

/**
 * Filter array recursively.
 * @param array $array
 * @param string $function
 * @return array
 */
function array_filterRecursive($array, $function) {
    /* Check array */
    if (!is_array($array))
        return $array;

    /* Check function */
    if (!function_exists($function))
        return $array;

    /* Read array */
    foreach ($array as $key => &$value) {
        if (is_array($value))
            $value = array_filterRecursive($value, $function);
        else
            $value = call_user_func($function, $value);
    }

    /* Return updated array */
    return $array;
}

/**
 * Count items recursively in an array.
 * @note Parent arrays or empty arrays are not items.
 * @param array $array
 * @return integer
 */
function array_countRecursive($array) {
    /* Check array */
    if (!is_array($array))
        return $array;

    /* Initialise count */
    $count = 0;

    /* Read array */
    foreach ($array as $index => $value) {
        if (is_array($value))
            $count += array_countRecursive($value);
        else
            $count ++;
    }

    /* Return count */
    return $count;
}
?>
