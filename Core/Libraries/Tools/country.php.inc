<?php
/**
 * @brief CaMykS country(ies) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre7.1
 * @date Creation: Oct 2019
 * @date Modification: Mar 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Country data related methods */

/**
 * Return countries data.
 * @return array
 */
function country_getCountriesData() {
    global $CaMykS;

    return $CaMykS->get_assetData('countries');
}

/* Country locale related methods */

/**
 * Load countries locale.
 * @param string $language
 * @return void
 */
function country_loadCountriesLocale($language=null) {
    global $CaMykS;

    $CaMykS->load_engineLocales('Libraries/country', 'country', $language);
}

/**
 * Return countries localised title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function country_getLocalisedTitles($params=array(), $language=null) {
    global $CaMykS;

    /* Define default params and merge with params */
    $params = array_merge(array('Territories'=>1, 'ShortList'=>array()), $params);

    /* Load data file */
    $countriesData = country_getCountriesData();

    /* Load locales */
    country_loadCountriesLocale($language);

    /* Initialise result */
    $result = array();

    if (is_array($params['ShortList']) and count($params['ShortList'])) {
        /* Add countries from the short list */
        foreach ($params['ShortList'] as $c)
            if (isset($countriesData[$c]) and ($countriesData[$c]['Type'] == 'country' or $params['Territories'] == 1))
                $result[$c] = $CaMykS->get_locale($c, 'country');
    } else {
        /* Add all countries */
        foreach ($countriesData as $c => $data)
            if ($data['Type'] == 'country' or $params['Territories'] == 1)
                $result[$c] = $CaMykS->get_locale($c, 'country');
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
    global $CaMykS;

    /* Optimise country */
    $country = strtolower($country);

    /* Load data file */
    $countriesData = country_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return '';

    /* Load locales */
    country_loadCountriesLocale($language);

    /* Return result */
    return $CaMykS->get_locale($country, 'country');
}

/**
 * Return country code from localised title.
 * @param string $title
 * @param string $language
 * @return string
 */
function countries_getCodeFromLocalisedTitle($title, $language=null) {
    global $CaMykS;

    /* Load data file */
    $countriesData = language_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return '';

    /* Load locales */
    country_loadCountriesLocale($language);

    /* Search values */
    $keys = $CaMykS->get_localeKeysByValue($title, 'country');

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
    /* Load data file */
    $countriesData = language_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return array();

    /* Return result */
    return $countriesData[$country]['OfficialLanguages'];
}

/**
 * Return country unofficial languages.
 * @param string $country
 * @return array
 */
function country_getUnofficialLanguages($country) {
    /* Load data file */
    $countriesData = language_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return array();

    /* Return result */
    return $countriesData[$country]['UnofficialLanguages'];
}

/**
 * Return country default language.
 * @param string $country
 * @return string
 */
function country_getDefaultLanguage($country) {
    /* Load data file */
    $countriesData = language_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]) or count($countriesData[$country]['OfficialLanguages']) == 0)
        return '';

    /* Return result */
    return $countriesData[$country]['OfficialLanguages'][0];
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
    /* Load data file */
    $countriesData = language_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return false;

    /* Return zipcode check result */
    return preg_match($countriesData[$country]['ZipCodeCheck'], $zipcode);
}

/* Country currency related methods */

/**
 * Return currency code for given country.
 * @param string $country
 * @return boolean string
 */
function country_getCurrencyCode($country) {
    /* Load data file */
    $countriesData = language_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return '';

    /* Return currency code */
    return $countriesData[$country]['Currency'];
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

/* Country flag related methods */

/**
 * Return country flag URL
 * @param string $country
 * @param string $collection
 * @param string $size
 * @param mixed $URLMode
 * @return string
 */
function country_getFlagURL($country, $collection='Flat', $size=16, $URLMode=false) {
    global $CaMykS;

    /* Load data file */
    $countriesData = country_getCountriesData();

    /* Check country */
    if (!isset($countriesData[$country]))
        return '';

    /* Define collections parameters */
    $collections = array(
        'Flat' => array(
            'FileExtension' => 'png',
            'Sizes'         => array(16, 32, 64, 128, 256, '256U'),
        ),
        'Wave' => array(
            'FileExtension' => 'png',
            'Sizes'         => array(16, 32, 64, 128, 256),
        ),
    );

    /* Check collection */
    if (!isset($collections[$collection]))
        return '';

    /* Check size */
    if (!in_array($size, $collections[$collection]['Sizes']))
        return '';

    /* Load file URL from CaMykS */
    $url = $CaMykS->get_assetFileURL('Images/Flags/'.$collection.'/'.$size.'/'.strtolower($country).'.'.$collections[$collection]['FileExtension'], $URLMode);

    /* Check URL */
    if ($url != '')
        return $url;

    /* Check for parent flag */
    if (!is_null($countriesData[$country]['Parent']))
        $url = country_getFlagURL($countriesData[$country]['Parent'], $collection, $size);

    /* Return url */
    return $url;
}

/**
 * Return country flag tag
 * @param string $country
 * @param string $collection
 * @param string $size
 * @param mixed $URLMode
 * @param string $imgParams
 * @param string $language
 * @return string
 */
function country_getFlagTag($country, $collection=null, $size=null, $URLMode=false, $imgParams=array(), $language=null) {
    /* Add source to image parameters */
    $imgParams['src'] = country_getFlagURL($country, $collection, $size, $URLMode);

    /* Add country name to image parameters */
    if (!isset($imgParams['alt']) or $imgParams['alt'] == '') {
        $imgParams['alt'] = country_getLocalisedTitle($country, $language);
        if (!isset($imgParams['title']) or $imgParams['title'] == '')
            $imgParams['title'] = $imgParams['alt'];
    }

    /* Return built tag */
    return html_getImg($imgParams);
}
?>
