<?php
/**
 * @brief CaMykS language(s) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Oct 2019
 * @date Modification: Jan 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @note CaMykS use the following formats for languages
 * Long format : en_GB - language_COUNTRY
 * Short format : en - language
 */

/* Language data related methods */

/**
 * Return languages data.
 * @return array
 */
function language_getLanguagesData() {
    global $CaMykS;

    return $CaMykS->get_assetData('languages');
}

/* Language structure related method */

/**
 * Return CaMykS optimised language from given value.
 * @param mixed $language
 * @return array
 */
function language_read($language) {
    $default = array(
        'Language'  => null,
        'Country'   => null,
    );

    /* Check if language is already optimised */
    if (is_array($language)) {
        if (array_keys($language) == array('Language', 'Country'))
            return $language;
        else
            return $default;
    }

    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check for full format language */
    if (preg_match('/^([a-z]{2})_([a-z]{2})$/i', $language, $matches)) {
        if (!isset($languagesData[strtolower($matches[1])]))
            return $default;
        return array(
            'Language'  => strtolower($matches[1]),
            'Country'   => strtoupper($matches[2]),
        );
    }

    /* Check for short format language */
    elseif (preg_match('/^([a-z]{2})$/i', $language, $matches)) {
        if (!isset($languagesData[strtolower($matches[1])]))
            return $default;
        return array(
            'Language'  => strtolower($matches[1]),
            'Country'   => $languagesData[strtolower($matches[1])]['DefaultCountry'],
        );
    }

    /* No format found */
    return $default;
}

/**
 * Return language as string.
 * @param mixed $language
 * @param boolean $noCountry
 * @return string
 */
function language_export($language, $noCountry=false) {
    /* Optimise language */
    $language = language_read($language);

    /* Return language without country */
    if ($noCountry === true or $language['Country'] == null)
        return strtolower($language['Language']);

    /* Return language with country */
    return strtolower($language['Language']).'_'.strtoupper($language['Country']);
}

/* Language native title related methods */

/**
 * Return languages localised title list.
 * @param array $params
 * @return array
 */
function language_getTitles($params=array()) {
    global $CaMykS;

    /* Define default params and merge with params */
    $params = array_merge(array('ShortList'=>array()), $params);

    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Initialise result */
    $result = array();

    if (is_array($params['ShortList']) and count($params['ShortList'])) {
        /* Add languages from the short list */
        foreach ($params['ShortList'] as $c) {
            $l = language_read($c);
            if (isset($languagesData[$l['Language']]))
                $result[$c] = $languagesData[$l['Language']]['Title'];
        }
    } else {
        /* Add all languages */
        foreach ($languagesData as $c => $data)
            $result[$c] = $data['Title'];
    }

    /* Return result */
    return $result;
}


/**
 * Return language native title.
 * @param string $language
 * @return string
 */
function language_getTitle($language) {
    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check language */
    if (isset($languagesData[$language]))
        return $languagesData[$language]['Title'];

    /* Not found */
    return '';
}

/* Language locale related methods */

/**
 * Load languages locale.
 * @param string $language
 * @return void
 */
function language_loadLanguagesLocale($language=null) {
    global $CaMykS;

    $CaMykS->load_locales('Libraries/languages', 'language', $language);
}

/**
 * Return languages localised title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function language_getLocalisedTitles($params=array(), $language=null) {
    global $CaMykS;

    /* Define default params and merge with params */
    $params = array_merge(array('ShortList'=>array()), $params);

    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Load locales */
    language_loadLanguagesLocale($language);

    /* Initialise result */
    $result = array();

    if (is_array($params['ShortList']) and count($params['ShortList'])) {
        /* Add languages from the short list */
        foreach ($params['ShortList'] as $c) {
            $l = language_read($c);
            if (isset($languagesData[$l['Language']]))
                $result[$c] = $CaMykS->get_locale($c, 'language');
        }
    } else {
        /* Add all languages */
        foreach ($languagesData as $c => $data)
            $result[$c] = $CaMykS->get_locale($c, 'language');
    }

    /* Return result */
    return $result;
}


/**
 * Return language localised title.
 * @param string $requestLanguage
 * @param string $resultLanguage
 * @return string
 */
function language_getLocalisedTitle($requestLanguage, $resultLanguage=null) {
    global $CaMykS;

    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check request language */
    if (!isset($languagesData[$requestLanguage]))
        return '';

    /* Load locales */
    language_loadLanguagesLocale($resultLanguage);

    /* Return result */
    return $CaMykS->get_locale($requestLanguage, 'language');
}

/* Language default country related methods */

/**
 * Return language default country code.
 * @param string $language
 * @return string
 */
function language_getDefaultCountryCode($language) {
    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check language */
    if (isset($languagesData[$language]))
        return $languagesData[$language]['DefaultCountry'];

    /* Not found */
    return null;
}

/* Language writting direction related methods */

/**
 * Return language writting direction related method.
 * @param string $language
 * @return string
 */
function language_getWrittingDirection($language) {
    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check language */
    if (isset($languagesData[$language]))
        return $languagesData[$language]['Direction'];

    /* Not found */
    return 'ltr';
}

/* Language tool methods */

/**
 * Return file size unit from $language.
 * @param string $language
 * @return string
 */
function language_getFileUnit($language=null) {
    global $CaMykS;

    /* Check language */
    if ($language == null) $language = $CaMykS->get_currentLanguage();

    /* Optimise language */
    $language = language_read($language);

    /* Return result */
    switch ($language['Language']) {
        case 'fr':
            return 'o';
        case 'en':
        default:
            return 'b';
    }
}

/**
 * Return number formatting case of current language.
 * @param float $number
 * @param integer $floats
 * @param string $language
 * @return string
 */
function language_numberFormat($number, $floats=2, $language=null) {
    global $CaMykS;

    /* Check language */
    if ($language == null) $language = $CaMykS->get_currentLanguage();

    /* Optimise language */
    $language = language_read($language);

    /* Return result */
    return string_numberFormat($number, $floats, $language);
}

/* CaMykS specific sequence */

/**
 * Return CaMykS locale loading sequence.
 * @param mixed $language
 * @return array
 */
function language_getLocaleLoadingSequence($language) {
    /* Initialise sequence */
    $result = array();

    /* Optimise language */
    $language = language_read($language);

    /* Check for given language */
    if (!is_null($language['Country']))
        $result[] = language_export($language);

    /* Check for default language */
    if (!is_null($language['Language'])) {
        $default = language_read($language['Language']);
        if (!is_null($default['Country']) and $default['Country'] != $language['Country'])
            $result[] = language_export($default);
    }

    /* Add no country language *
    $result[] = language_export($language, true);

    /* Return sequence */
    return $result;
}

/**
 * Return system languages list.
 * @param mixed $language
 * @return array
 */
function language_getSystemLanguages($language) {
    /* Get string language */
    $language = language_export($language);

    /* Return built values */
    return array($language.'.utf8', $language.'.UTF-8', $language.'.UTF8', $language.'.utf-8', $language);
}
?>
