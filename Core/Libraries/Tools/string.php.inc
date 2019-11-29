<?php
/**
 * @brief String specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Check methods */

/**
 * Check if given string is html.
 * @param string $string
 * @return boolean
 */
function string_isHTML($string) {
    return (strip_tags($string) != $string or preg_match('/&[^ ]+;/', $string));
}

/**
 * Check if given string is an URL.
 * @param string $string
 * @return boolean
 */
function string_isURL($string) {
    return preg_match('#^https?://#i', $string);
}

/**
 * Check if given string is UTF8.
 * @param string $string
 * @return boolean
 */
function string_isUTF8($string) {
    return preg_match('%(?:
        [\xC2-\xDF][\x80-\xBF]              # non-overlong 2-byte
        |\xE0[\xA0-\xBF][\x80-\xBF]         # excluding overlongs
        |[\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
        |\xED[\x80-\x9F][\x80-\xBF]         # excluding surrogates
        |\xF0[\x90-\xBF][\x80-\xBF]{2}      # planes 1-3
        |[\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
        |\xF4[\x80-\x8F][\x80-\xBF]{2}      # plane 16
        )+%xs', $string
    );
}

/**
 * Check if given string is base64 encoded.
 * @param string $string
 * @return boolean
 */
function string_isBase64($string) {
    if (base64_decode($string, true) === false)
        return false;
    if (strlen($string) % 4 != 0)
        return false;
    return true;
}

/* Tag parsing methods */

/**
 * Evolved strip_tags method, may replace tags by another string
 * @param string $value
 * @param string $replacement
 * @return string
 */
function string_stripTags($value, $replacement='') {
    if ($replacement == '')
        return strip_tags($value);
    return preg_replace('/<([^>]+)>/i', $replacement, $value);
}

/**
 * Remove PHP tags from value.
 * @param string $value
 * @param string $replace
 * @return string
 */
function string_stripPHPTags($value, $replacement='') {
    $tags = array();
    $tags[] = '/<\?[^\?'.'>]*\?'.'>/';
    $tags[] = '/<\?/';
    return preg_replace($tags, $replacement, $value);
}

/**
 * Remove Script tags from value.
 * @param string $value
 * @param string $replace
 * @return string
 */
function string_stripScriptTags($value, $replacement='') {
    return preg_replace('/<script.*?\/script>/s', $replacement, $value);
}

?>
