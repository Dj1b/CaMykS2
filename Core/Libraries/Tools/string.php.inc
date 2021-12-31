<?php
/**
 * @brief String specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Nov 2019
 * @date Modification: Dec 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Conversion methods */

/**
 * Decode string from HTML to text.
 * @param string $string
 * @param string $encoding
 * @return string
 */
function string_html2Text($string, $encoding=null) {
    global $CaMykS;

    /* check encoding */
    if (!isset($encoding))
        $encoding = $CaMykS->get_configValue('Website_Encoding');

    /* decode HTML */
    $string = html_entity_decode($string);

    /* encode text */
    switch ($encoding) {
        case 'UTF-8':
            return utf8_encode($string);
            break;
        case 'ISO-8859-1':
            if (string_isUTF8($string))
                return utf8_decode($string);
            break;
        default:
        case '':
            return $string;
    }
    return $string;
}

/**
 * Encode string from text to HTML.
 * @param string $string
 * @param string $encoding
 * @return string
 */
function string_text2Html($string, $encoding=null) {
    global $CaMykS;

    /* check encoding */
    if (!isset($encoding))
        $encoding = $CaMykS->get_configValue('Website_Encoding');

    /* decode text */
    switch ($encoding) {
        case 'UTF-8':
            return htmlentities($string, null, 'UTF-8');
        case 'ISO-8859-1':
            return htmlentities($string, null, 'ISO-8859-1');
        default:
        case '':
    }
    return htmlentities($string);
}

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
 * @param string $replacement
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
 * @param string $replacement
 * @return string
 */
function string_stripScriptTags($value, $replacement='') {
    return preg_replace('/<script.*?\/script>/s', $replacement, $value);
}

/* String transformation methods */

/**
 * Return limited version of given sentence.
 * @param string $string
 * @param integer $length
 * @return string
 */
function string_getShortenedSentence($string='', $length=64) {
    $length = max($length, 4);
    if (strlen($string) > $length) {
        if (preg_match('/^(.){1,'.($length-3).'} /u', $string, $matches))
            return $matches[0].'...';
        if (function_exists('mb_substr'))
            return mb_substr($string, 0, $length).'...';
        return substr($string, 0, $length).'...';
    }
    return $string;
}

/* String search methods */

/**
 * Check if given string starts with other string.
 * @param string $haystack
 * @param mixed $needle
 * @param boolean $caseSensitive
 * @return boolean result
 */
function string_startsWith($haystack, $needle, $caseSensitive=false) {
    if (is_array($needle)) {
        foreach ($needle as $id => $value)
            $needle[$id] = preg_quote($value, '/');
        return preg_match('/^('.implode('|', $needle).')/'.($caseSensitive?'i':''), $haystack) > 0;
    } else {
        if ($caseSensitive) {
            $needle = strtolower ($needle);
            $haystack = strtolower ($haystack);
        }
        return $needle === "" || strpos($haystack, $needle) === 0;
    }
}

/**
 * Check if given string ends with other string.
 * @param string $haystack
 * @param mixed $needle
 * @param boolean $caseSensitive
 * @return boolean result
 */
function string_endsWith($haystack, $needle, $caseSensitive=false) {
    if (is_array($needle)) {
        foreach ($needle as $id => $value)
            $needle[$id] = preg_quote($value, '/');
        return preg_match('/('.implode('|', $needle).')$/'.($caseSensitive?'i':''), $haystack) > 0;
    } else {
        if ($caseSensitive) {
            $needle = strtolower ($needle);
            $haystack = strtolower ($haystack);
        }
        return $needle === "" || substr($haystack, -strlen($needle)) === $needle;
    }
}

/* String escaping methods */

/**
 * Return string escaped for regular expression.
 * @param string $string
 * @return string
 */
function string_escape4Regex($string='') {
  return preg_replace('/(\/|\(|\)|\.|\?|\$|\^|\[|\]|\*|\+|\{|\}|\,)/', '\\\$1', $string);
}

?>
