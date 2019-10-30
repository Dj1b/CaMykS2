<?php
/**
 * @brief CaMykS country(ies) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Oct 2019
 * @date Modification: Oct 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Country data related methods */

/**
 * Load countries data.
 * @return void
 */
function country_loadCountriesData() {
    global $CaMykS;

    $CaMykS->load_library('Data/countries');
}

/**
 * Return countries data.
 * @return array
 */
function country_getCountriesData() {
    global $cCountries;

    /* Load data file */
    country_loadCountriesData();

    /* Return languages data */
    return $cCountries;
}

/* Country locale related methods */

/**
 * Load countries locale.
 * @param string $language
 * @return void
 */
function country_loadCountriesLocale($language=null) {
    global $CaMykS;

    $CaMykS->load_locales('Libraries/countries', $language);
}

/**
 * Return countries localised title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function country_getLocalisedTitles($params=array(), $language=null) {
    global $cCountries, $CaMykS;

    /* Define default params and merge with params */
    $params = array_merge(array('Territories'=>1, 'ShortList'=>array()), $params);

    /* Load data file */
    language_getCountriesData();

    /* Load locales */
    country_loadCountriesLocale($language);

    /* Initialise result */
    $result = array();

    if (is_array($params['ShortList']) and count($params['ShortList'])) {
        /* Add countries from the short list */
        foreach ($params['ShortList'] as $c)
            if (isset($cCountries[$c]) and ($cCountries[$c]['Type'] == 'country' or $params['Territories'] == 1))
                $result[$c] = $CaMykS->get_locale('country_'.$c);
    } else {
        /* Add all countries */
        foreach ($cCountries as $c => $data)
            if ($data['Type'] == 'country' or $params['Territories'] == 1)
                $result[$c] = $CaMykS->get_locale('country_'.$c);
    }

    /* Return result */
    return $result;
}

/**
 * Return country localised title.
 * @param string $country
 * @param string $language
 * @return array
 */
function country_getLocalisedTitle($country, $language=null) {
    global $cCountries, $CaMykS;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]))
        return '';

    /* Load locales */
    country_loadCountriesLocale($language);

    /* Return result */
    return $CaMykS->get_locale('country_'.$country);
}

/**
 * Return country code from localised title.
 * @param string $title
 * @param string $language
 * @return string
 */
function countries_getCodeFromLocalisedTitle($title, $language=null) {
    global $cCountries, $CaMykS;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]))
        return '';

    /* Load locales */
    country_loadCountriesLocale($language);

    /* Search values */
    $keys = $CaMykS->get_localeKeysByValue($title, 'country_');

    /* Check keys */
    if (count($keys) === 0)
        return false;

    /* Return country code for first result (it should be only one) */
    foreach ($keys as $key)
        return substr($key, strlen('country_'));

    /* Default return */
    return false;
}

/* Country languages related methods */

/**
 * Return country official languages.
 * @param string $country
 * @return array
 */
function country_getOfficialLanguages($country) {
    global $cCountries;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]))
        return array();

    /* Return result */
    return $cCountries[$country]['OfficialLanguages'];
}

/**
 * Return country unofficial languages.
 * @param string $country
 * @return array
 */
function country_getUnofficialLanguages($country) {
    global $cCountries;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]))
        return array();

    /* Return result */
    return $cCountries[$country]['UnofficialLanguages'];
}

/**
 * Return country default language.
 * @param string $country
 * @return string
 */
function country_getDefaultLanguage($country) {
    global $cCountries;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]) or count($cCountries[$country]['OfficialLanguages']) == 0)
        return '';

    /* Return result */
    return $cCountries[$country]['OfficialLanguages'][0];
}

/* Country phone index related methods */



/* Country zipcode related methods */

/**
 * Check if provided zipcode is valid for given country.
 * @param string $country
 * @param string $zipcode
 * @return boolean result
 */
function country_checkZipcode($country, $zipcode) {
    global $cCountries;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]))
        return false;

    /* Return zipcode check result */
    return preg_match($cCountries[$country]['ZipCodeCheck'], $zipcode);
}

/* Country currency related methods */

/**
 * Return currency code for given country.
 * @param string $country
 * @return boolean string
 */
function country_getCurrencyCode($country) {
    global $cCountries;

    /* Load data file */
    language_getCountriesData();

    /* Check country */
    if (!isset($cCountries[$country]))
        return '';

    /* Return currency code */
    return $cCountries[$country]['Currency'];
}

/**
 * Return currency title for given country.
 * @param string $country
 * @return boolean string
 */
function country_getCurrencyTitle($country) {
    /* Return currency title */
    return currency_getLocalisedTitle(country_getCurrencyCode($country));
}





/**
 * Return countries code list.
 * @param array $params
 * @return array
 */
function countries_getCodeList($params=array()) {
    $dParams = array('countries'=>1, 'territories'=>1, 'ex-countries'=>0, );
    $params = array_merge($dParams, $params);
    $list = array();

    /* add countries to the list */
    if ($params['countries']==1)
        $list = array_merge($list, array(
            'ad', 'ae', 'af', 'ag', 'al', 'am', 'ao', 'ar', 'at', 'au',
            'az', 'ba', 'bb', 'bd', 'be', 'bf', 'bg', 'bh', 'bi', 'bj',
            'bn', 'bo', 'br', 'bs', 'bt', 'bw', 'by', 'bz', 'ca', 'cd',
            'cf', 'cg', 'ch', 'ci', 'ck', 'cl', 'cm', 'cn', 'co', 'cr',
            'cu', 'cv', 'cw', 'cy', 'cz', 'de', 'dj', 'dk', 'dm', 'do',
            'dz', 'ec', 'ee', 'eg', 'er', 'es', 'et', 'fi', 'fj', 'fr',
            'ga', 'gb', 'gd', 'ge', 'gg', 'gh', 'gm', 'gn', 'gq', 'gr',
            'gt', 'gw', 'gy', 'hn', 'hr', 'ht', 'hu', 'id', 'ie', 'il',
            'im', 'in', 'iq', 'ir', 'is', 'it', 'je', 'jm', 'jo', 'jp',
            'ke', 'kg', 'kh', 'ki', 'km', 'kn', 'kp', 'kr', 'kw', 'kz',
            'la', 'lb', 'lc', 'li', 'lk', 'lr', 'ls', 'lt', 'lu', 'lv',
            'ly', 'ma', 'mc', 'md', 'me', 'mg', 'mh', 'mk', 'ml', 'mn',
            'mm', 'mr', 'mt', 'mu', 'mv', 'mw', 'mx', 'my', 'mz', 'na',
            'ne', 'nf', 'ng', 'ni', 'nl', 'no', 'np', 'nr', 'nz', 'om',
            'pa', 'pe', 'pg', 'ph', 'pk', 'pl', 'pt', 'pw', 'py', 'qa',
            'ro', 'rs', 'ru', 'rw', 'sa', 'sb', 'sc', 'sd', 'se', 'sg',
            'si', 'sk', 'sl', 'sm', 'sn', 'so', 'sr', 'ss', 'st', 'sv',
            'sy', 'sz', 'td', 'tg', 'th', 'tj', 'tl', 'tm', 'tn', 'to',
            'tr', 'tt', 'tv', 'tz', 'ua', 'ug', 'um', 'us', 'uy', 'uz',
            'va', 'vc', 've', 'vn', 'vu', 'ws', 'xk', 'ye', 'za', 'zm',
            'zw',
        ));

    /* add territories to the list */
    if ($params['territories']==1)
        $list = array_merge($list, array(
            'ai', 'an', 'aq', 'as', 'aw', 'ax', 'bl', 'bm', 'bv', 'bq',
            'cc', 'cx', 'eh', 'fk', 'fo', 'gf', 'gi', 'gl', 'gp', 'gs',
            'gu', 'hk', 'hm', 'io', 'ky', 'mf', 'mo', 'mp', 'mq', 'ms',
            'nc', 'nu', 'pf', 'pm', 'pn', 'pr', 'ps', 'qc', 're', 'sh',
            'sj', 'sx', 'td', 'tf', 'tk', 'tw', 'um', 'vg', 'vi', 'wf',
            'yt',
        ));

    /* add old countries to the list */
    if ($params['ex-countries']==1)
        $list = array_merge($list, array('cs', 'su', 'yu'));

    /* check for a short list of countries */
    if (count($params['shortlist']) > 0) {
        $t = array();
        foreach ($params['shortlist'] as $c)
            if (in_array($c, $list))
                $t[] = $c;
        $list = $t;
    }

    /* sort list */
    sort($list);

    /* return final code list */
    return $list;
}

/**
 * Return generic country list.
 * @return array
 */
function countries_getList() {
    static $list;
    if (isset($list))
        return $list;
    $list = countries_getTitleList();
    return $list;
}

/**
 * Return custom country list.
 * @param array $params
 * @return array
 */
function countries_getTitleList($params=array()) {
    /* load countries translation */
    countries_loadLocales();

    /* build complete list */
    $list = array();
    foreach ( countries_getCodeList($params) as $v )
        $list[$v] = countries_getTranslation($v);

    /* sort countries */
    asort($list);

    /* return list */
    return $list;
}

/**
 * Return country phone prefixes list.
 * @return array
 */
function countries_getPhonePrefixes() {
    global $__phonePrefixes;
    require_once ('countries_phonePrefixes.php.inc');
    return $__phonePrefixes;
}

/**
 * Return phone prefix value for given country.
 * @param string $country
 * @return string
 */
function countries_getPhonePrefix($country) {
    global $__phonePrefixes;
    require_once ('countries_phonePrefixes.php.inc');
    if (isset($__phonePrefixes[$country]))
        return $__phonePrefixes[$country];
    return '';
}
?>
