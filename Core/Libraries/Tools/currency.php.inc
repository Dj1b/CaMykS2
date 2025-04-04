<?php
/**
 * @brief CaMykS currency(ies) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2019
 * @date Modification: Apr 2025
 * @copyright 2019 - 2025 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @todo Add short title (US$, GB£, JP¥, €, ... ) to data, and write methods to read them (list & single)
 */

/* Mark: Currency data related methods */

/**
 * Return currencies data.
 * @return array
 */
function currency_getCurrenciesData() {
    global $CaMykS;

    return $CaMykS->get_assetData('currencies');
}

/* Mark: Currency locale related methods */

/**
 * Load currencies locale.
 * @param string $language
 * @return void
 */
function currency_loadCurrenciesLocale(?string $language=null) {
    global $CaMykS;

    $CaMykS->load_locales('Libraries/currencies', $language);
}

/**
 * Return currencies localised title list.
 * @param array $params
 * @param string $language
 * @return array
 */
function currency_getLocalisedTitles(array $params=array(), ?string $language=null) {
    global $CaMykS;

    /* Define default params and merge with params */
    $params = array_merge(array('ShortList'=>array()), $params);

    /* Load data file */
    $currenciesData = language_getCurrenciesData();

    /* Load locales */
    currency_loadCurrenciesLocale($language);

    /* Initialise result */
    $result = array();

    if (is_array($params['ShortList']) and count($params['ShortList'])) {
        /* Add currencies from the short list */
        foreach ($params['ShortList'] as $c)
            if (isset($currenciesData[$c]))
                $result[$c] = $CaMykS->get_locale('currency_'.$c);
    } else {
        /* Add all currencies */
        foreach ($currenciesData as $c => $data)
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
function currency_getLocalisedTitle(string $currency, ?string $language=null) {
    global $CaMykS;

    /* Load data file */
    $currenciesData = language_getCurrenciesData();

    /* Check currency */
    if (!isset($currenciesData[$currency]))
        return '';

    /* Load locales */
    currency_loadCurrenciesLocale($language);

    /* Return result */
    return $CaMykS->get_locale('currency_'.$currency);
}
?>
