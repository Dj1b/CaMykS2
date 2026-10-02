<?php
/**
 * @brief CaMykS language(s) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre9
 * @date Creation: Oct 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @note CaMykS use the following formats for languages
 * Long format : en_GB - language_COUNTRY
 * Short format : en - language
 */

/* Mark: Language data related methods */

/**
 * Return languages data.
 * @return array
 */
function language_getLanguagesData(): array {
    global $CaMykS;

    return $CaMykS->get_assetData('Languages');
}

/* Mark: Language native title related methods */

/**
 * Return languages localised title list.
 * @param array $params
 * @return array
 */
function language_getTitles(array $params=array()): array {
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
            $l = new \CaMykS2\Language($c);
            if (isset($languagesData[$l->get_Language()]))
                $result[$c] = $languagesData[$l->get_Language()]['Title'];
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
function language_getTitle(array $language): string {
    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check language */
    if (isset($languagesData[$language]))
        return $languagesData[$language]['Title'];

    /* Not found */
    return '';
}

/* Mark: Language locale related methods */

/**
 * Load languages locale.
 * @param string $language
 * @return void
 */
function language_loadLanguagesLocale(?string $language=null): void {
    global $CaMykS;

    $CaMykS->load_engineLocales('Libraries/language', 'language', $language);
}

/**
 * Return languages localised title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function language_getLocalisedTitles(array $params=array(), ?string $language=null): array {
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
            $l = new \CaMykS2\Language($c);
            if (isset($languagesData[$l->get_Language()]))
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
function language_getLocalisedTitle(string $requestLanguage, ?string $resultLanguage=null): string {
    global $CaMykS;

    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Read language */
    $requestLanguage = new \CaMykS2\Language($requestLanguage);

    /* Check request language */
    if (!isset($languagesData[$requestLanguage->get_Language()]))
        return '';

    /* Load locales */
    language_loadLanguagesLocale($resultLanguage);

    /* Return result */
    return $CaMykS->get_locale($requestLanguage->get_Language(), 'language');
}

/**
 * Return languages localised long title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function language_getLocalisedLongTitles(array $params=array(), ?string $language=null): array {
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
            if (($l = language_getLocalisedLongTitleValue(new \CaMykS2\Language($c))) != '')
                $result[$c] = $l;
        }
    } else {
        /* Load languages by countries */
        $countries = country_getOfficialLanguages();

        foreach ($countries as $country => $languages) {
            foreach ($languages as $language) {
                $c = new \CaMykS2\Language(array('Language' => $language, 'Script' => null, 'Country' => $country));
                if (($l = language_getLocalisedLongTitleValue($c)) != '')
                $result[(new \CaMykS2\Language($c))->get_value()] = $l;
            }
        }
    }

    /* Return result */
    return $result;
}

/**
 * Return language localised long title.
 * @param string $requestLanguage
 * @param string $resultLanguage
 * @return string
 */
function language_getLocalisedLongTitle(string $requestLanguage, ?string $resultLanguage=null): string {
    global $CaMykS;

    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Read language */
    $requestLanguage = new \CaMykS2\Language($requestLanguage);

    /* Check request language */
    if (!isset($languagesData[$requestLanguage->get_Language()]))
        return '';

    /* Load locales */
    language_loadLanguagesLocale($resultLanguage);

    /* Return result */
    return language_getLocalisedLongTitleValue($requestLanguage);
}

/**
 * Return language localised long title assuming all is loaded.
 * @param array $requestLanguage
 * @param string $resultLanguage
 * @return string
 */
function language_getLocalisedLongTitleValue(\CaMykS2\Language $requestLanguage, ?string $resultLanguage=null): string {
    global $CaMykS;

    /* Add language title */
    $title = $CaMykS->get_locale($requestLanguage->get_Language(), 'language');

    /* Add details */
    $details = array();
    if (!is_null($requestLanguage->get_Script()))
        $details[] = $CaMykS->get_locale($requestLanguage->get_Script(), 'language');
    if (!is_null($requestLanguage->get_Country()))
        $details[] = country_getLocalisedTitle($requestLanguage->get_Country(), $resultLanguage);
    if (count($details) > 0)
        $title .= ' ('.implode(', ', $details).')';

    /* Return result */
    return $title;
}

/* Mark: Language default country related methods */

/**
 * Return language default country code.
 * @param string $language
 * @return string
 */
function language_getDefaultCountryCode(string $language): string {
    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check language */
    if (isset($languagesData[$language]))
        return $languagesData[$language]['DefaultCountry'];

    /* Not found */
    return null;
}

/* Mark: Language writting direction related methods */

/**
 * Return language writting direction related method.
 * @param string $language
 * @return string
 */
function language_getWrittingDirection(string $language): string {
    /* Load data file */
    $languagesData = language_getLanguagesData();

    /* Check language */
    if (isset($languagesData[$language]))
        return $languagesData[$language]['Direction'];

    /* Not found */
    return 'ltr';
}

/* Mark: Language tool methods */

/**
 * Return file size unit from $language.
 * @param string $language
 * @return string
 */
function language_getFileUnit(?string $language=null): string {
    global $CaMykS;

    /* Check language */
    if ($language == null) $language = $CaMykS->get_currentLanguage(true);

    /* Return result */
    switch ($language) {
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
function language_numberFormat(float $number, int $floats=2, ?string $language=null): string {
    global $CaMykS;

    /* Check language */
    if ($language == null) $language = $CaMykS->get_currentLanguage(true);

    /* Return result */
    return string_numberFormat($number, $floats, $language);
}

/* Mark: CaMykS specific sequence */

/**
 * Return CaMykS locale loading sequence.
 * @param mixed $language
 * @return array
 */
function language_getLocaleLoadingSequence(mixed $language): array {
    /* Initialise sequence */
    $result = array();

    /* Optimise language */
    $language = new \CaMykS2\Language($language);

    /* Check for given language */
    if (!is_null($language->get_Country()))
        $result[] = (new \CaMykS2\Language($language))->get_value();

    /* Check for default language */
    if (!is_null($language->get_Language())) {
        $default = new \CaMykS2\Language($language->get_Language());
        if (!is_null($default->get_Country()) and $default->get_Country() != $language->get_Country())
            $result[] = (new \CaMykS2\Language($default))->get_value();
    }

    /* Add no country language
    $result[] = (new \CaMykS2\Language($language))->get_value(true);
    */

    /* Return sequence */
    return $result;
}

/**
 * Return system languages list.
 * @param string $language
 * @return array
 */
function language_getSystemLanguages(string $language): array {
    /* Return built values */
    return array($language.'.utf8', $language.'.UTF-8', $language.'.UTF8', $language.'.utf-8', $language);
}
?>
