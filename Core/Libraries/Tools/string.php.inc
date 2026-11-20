<?php
/**
 * @brief String specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre9
 * @date Creation: Nov 2019
 * @date Modification: Nov 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Mark: Conversion methods */

/**
 * Decode string from HTML to text.
 * @param string $string
 * @param string $encoding
 * @return string
 */
function string_html2Text(string $string, ?string $encoding=null): string {
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
function string_text2Html(string $string, ?string $encoding=null): string {
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

/**
 * Return unaccentued words list from html.
 * @param string $string
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string contenttype - \b text / \b html <br />
 * \e string accents - \b replace / \b keep <br />
 * \e string changecase - \b void / \b lower / \b upper <br />
 * \e string keptchars - <br />
 * \e integer minlength <br />
 * @endparblock
 * @return array
 */
function string_text2Words(string $string, array $params=array()): array {
    /* default config */
    $config = array('contenttype'=>'html', 'accents'=>'replace', 'changecase'=>'', 'keptchars'=>'', 'minlength'=>0);

    /* merge config & params */
    $params = array_merge($config, $params);

    /* remove html tags */
    $string = string_stripTags($string, ' ');

    /* normalise html and text encoding */
    if ($params['accents'] == 'replace') {
        $string = string_html2Text($string);
        $string = string_text2Html($string);
    } elseif ($params['contenttype'] == 'text') {
        $string = string_text2Html($string);
    } elseif ($params['contenttype'] == 'html') {
        $string = string_html2Text($string);
    }

    /* parse accents */
    if ($params['accents'] == 'replace') {
        $string = string_replaceEntityChars($string, array('lastreplace'=>' '));
        $accentChars = '';
    } else {
        $accentChars = string_getAccentuedChars(array('listmode'=>'string', 'charmode'=>'text'));
    }

    /* update case */
    if ($params['changecase'] == 'lower')
        $string = strtolower($string);
    else if ($params['changecase'] == 'upper')
        $string = strtoupper($string);

    /* set min length word regexp modifier */
    if ($params['minlength'] > 1)
        $length_modifier = '{'.$params['minlength'].',}';
    else
        $length_modifier = '+';

    /* split on non-letter chars */
    $count = preg_match_all('/\b[\w'.$accentChars.string_escape4Regex($params['keptchars']).']'.$length_modifier.'\b/', $string, $array);
    $array = $array[0];

    /* check html mode */
    if ($params['contenttype'] == 'html' and $params['accents'] != 'replace') {
        foreach ($array as $idx => $value) {
            $array[$idx] = htmlentities($value);
        }
    } elseif ($params['contenttype'] == 'text') {
        foreach ($array as $idx => $value) {
            $array[$idx] = string_html2Text($value);
        }
    }
    return $array;
}

/**
 * Return replace entity char by unaccentued chars.
 * @param string $string
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string/array pattern - containing patterns to complete built-in list <br />
 * \e string/array replace - containing replacement values to complete built-in list<br />
 * \e string lastreplace - all last entities are replaced by this value (empty string for no replacement) <br />
 * @endparblock
 * @return string
 */
function string_replaceEntityChars(string $string, array $params=array()): string {
    /* default config */
    $config = array('pattern'=>null, 'replace'=>null, 'lastreplace'=>null);

    /* merge config & params */
    $params = array_merge($config, $params);

    /* init pattern array */
    $pattern = array(
        /* add html named entities */
        '/&([a-zA-Z])(?:acute|grave|cedil|tilde|circ|uml|ring|slash);/',
        /* add html number entities for capitalised letters */
        '/&#19[2-7];/', '/&#199;/', '/&#20[0-3];/', '/&#20[4-7];/', '/&#209;/', '/&#21[0-46];/', '/&#2(?:1[7-9]|20);/', '/&#221;/',
        /* add html number entities for small letters */
        '/&#22[4-9];/', '/&#231;/', '/&#23[2-5];/', '/&#23[6-9];/', '/&#241;/', '/&#24[2-68];/', '/&#2(?:49|5[0-2]);/', '/&#25[35];/',
        /* add html number entities for special letters */
        '/&(?:#198|AElig);/', '/&(?:#223|szlig);/', '/&(?:#230|aelig);/', '/&(?:#338|oelig);/', '/&(?:#339|OElig);/', '/&#352;/', '/&#353;/', '/&#376;/',
        /* add html number entities for special chars */
        '/&amp;/', '/&hellip;/',
    );

    /* init replacement array */
    $replace = array(
        /* add html named entities */
        '$1',
        /* add html number entities for capitalised letters */
        'A', 'C', 'E', 'I', 'N', 'O', 'U', 'Y',
        /* add html number entities for small letters */
        'a', 'c', 'e', 'i', 'n', 'o', 'u', 'y',
        /* add html number entities for special letters */
        'AE', 'ss', 'ae', 'OE', 'oe', 'S', 's', 'Y',
        /* add html number entities for special chars */
        '', '...',
    );

    /* add param patterns and replacements */
    if (isset($params['pattern']) and isset($params['replace'])) {
        if (is_string($params['pattern']) and is_string($params['replace'])) {
            $pattern[] = $params['pattern'];
            $replace[] = $params['replace'];
        } else if (is_array($params['pattern']) and is_array($params['replace']) and count($params['pattern']) == count($params['replace'])) {
            $pattern = array_merge($pattern, $params['pattern']);
            $replace = array_merge($replace, $params['replace']);
        }
    }

    /* all other entities */
    if (isset($params['lastreplace'])) {
        $pattern[] = '/&[#\w]+;/';
        $replace[] = $params['lastreplace'];
    }

    /* execute replacement */
    return preg_replace($pattern, $replace, $string);
}

/**
 * Return accentued chars list.
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string listmode - \b array / \b string <br />
 * \e string charmode - \b text / \b html <br />
 * @endparblock
 * @return mixed
 */
function string_getAccentuedChars(array $params=array()): mixed {
    /* default config */
    $config = array('listmode'=>'array', 'charmode'=>'text');

    /* merge config & params */
    $params = array_merge($config, $params);

    /* init list */
    $accentChars = array();

    /* special array/text optimisation */
    if ($params['listmode'] == 'array' and $params['charmode'] == 'text') {
        for ($i=192; $i<256; $i++)
            $accentChars[] = html_entity_decode('&#'.$i.';');
        foreach (array(338, 339, 352, 353) as $i)
            $accentChars[] = html_entity_decode('&#'.$i.';');
        return $accentChars;
    }

    /* generic  */
    for ($i=192; $i<256; $i++)
        $accentChars[] = '&#'.$i.';';
    foreach (array(338, 339, 352, 353) as $i)
        $accentChars[] = '&#'.$i.';';
    if ($params['listmode'] != 'array')
        $accentChars = implode('', $accentChars);
    if ($params['charmode'] == 'text')
        $accentChars = html_entity_decode($accentChars);
    return $accentChars;
}

/**
 * Return parsed string, accentued chars are replaced by their unaccentued equivalent.
 * @param string $string
 * @param array $params
 * @parblock - Available parameters are <br />
 * \e string/array pattern <br />
 * \e string/array replace <br />
 * @endparblock
 * @return string
 */
function string_parseSpecialChars(string $string, array $params=array()): string {
    /* default config */
    $config = array('pattern'=>'/[^a-zA-Z]/', 'replace'=>'');

    /* merge config & params */
    $params = array_merge($config, $params);

    /* execute replacement */
    return string_replaceEntityChars(string_text2Html($string), array('pattern'=>$params['pattern'], 'replace'=>$params['replace'], 'lastreplace'=>''));
}

/* Mark: Check methods */

/**
 * Check if given string is html.
 * @param string $string
 * @return boolean
 */
function string_isHTML(string $string): bool {
    return (strip_tags($string) != $string or preg_match('/&[^ ]+;/', $string));
}

/**
 * Check if given string is an URL.
 * @param string $string
 * @return boolean
 */
function string_isURL(string $string): bool {
    return preg_match('#^https?://#i', $string);
}

/**
 * Check if given string is UTF8.
 * @param string $string
 * @return boolean
 */
function string_isUTF8(string $string): bool {
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
function string_isBase64(string $string): bool {
    if (base64_decode($string, true) === false)
        return false;
    if (strlen($string) % 4 != 0)
        return false;
    return true;
}

/* Mark: Tag parsing methods */

/**
 * Evolved strip_tags method, may replace tags by another string
 * @param string $value
 * @param string $replacement
 * @return string
 */
function string_stripTags(string $value, string $replacement=''): string {
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
function string_stripPHPTags(string $value, string $replacement=''): string {
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
function string_stripScriptTags(string $value, string $replacement=''): string {
    return preg_replace('/<script.*?\/script>/s', $replacement, $value);
}

/* Mark: String transformation methods */

/**
 * Return limited version of given sentence.
 * @param string $string
 * @param integer $length
 * @return string
 */
function string_getShortenedSentence(string $string='', int $length=64): string {
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

/* Mark: String search methods */

/**
 * Check if given string starts with other string.
 * @param string $haystack
 * @param mixed $needle
 * @param boolean $caseSensitive
 * @return boolean result
 */
function string_startsWith(string $haystack, mixed $needle, bool $caseSensitive=false): bool {
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
function string_endsWith(string $haystack, mixed $needle, bool $caseSensitive=false): bool {
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

/* Mark: String escaping methods */

/**
 * Return string escaped for regular expression.
 * @param string $string
 * @return string
 */
function string_escape4Regex(string $string=''): string {
    return preg_replace('/(\/|\(|\)|\.|\?|\$|\^|\[|\]|\*|\+|\{|\}|\,|\-)/', '\\\$1', $string);
}

?>
