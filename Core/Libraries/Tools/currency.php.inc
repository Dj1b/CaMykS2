<?php
/**
 * @brief CaMykS currency(ies) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Oct 2019
 * @date Modification: Oct 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @todo Adds short title (US$, GB£, JP¥, €, ... ) to data, and create method to read them (list & single)
 */

/* Currency data related methods */

/**
 * Load currencies data.
 * @return void
 */
function currency_loadCurrenciesData() {
    global $CaMykS;

    $CaMykS->load_library('Data/currencies');
}

/**
 * Return currencies data.
 * @return array
 */
function currency_getCurrenciesData() {
    global $cCurrencies;

    /* Load data file */
    currency_loadCurrenciesData();

    /* Return languages data */
    return $cCurrencies;
}

/* Currency locale related methods */

/**
 * Load currencies locale.
 * @param string $language
 * @return void
 */
function currency_loadCurrenciesLocale($language=null) {
    global $CaMykS;

    $CaMykS->load_locales('Libraries/currencies', $language);
}

/**
 * Return currencies localised title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function currency_getLocalisedTitles($params=array(), $language=null) {
    global $cCurrencies, $CaMykS;

    /* Define default params and merge with params */
    $params = array_merge(array('ShortList'=>array()), $params);

    /* Load data file */
    language_getCurrenciesData();

    /* Load locales */
    currency_loadCurrenciesLocale($language);

    /* Initialise result */
    $result = array();

    if (is_array($params['ShortList']) and count($params['ShortList'])) {
        /* Add currencies from the short list */
        foreach ($params['ShortList'] as $c)
            if (isset($cCurrencies[$c]))
                $result[$c] = $CaMykS->get_locale('currency_'.$c);
    } else {
        /* Add all currencies */
        foreach ($cCurrencies as $c => $data)
            $result[$c] = $CaMykS->get_locale('currency_'.$c);
    }

    /* Return result */
    return $result;
}

/**
 * Return currency localised title.
 * @param string $currency
 * @param string $language
 * @return array
 */
function currency_getLocalisedTitle($currency, $language=null) {
    global $cCurrencies, $CaMykS;

    /* Load data file */
    language_getCurrenciesData();

    /* Check currency */
    if (!isset($cCurrencies[$currency]))
        return '';

    /* Load locales */
    currency_loadCurrenciesLocale($language);

    /* Return result */
    return $CaMykS->get_locale('currency_'.$currency);
}
?>
