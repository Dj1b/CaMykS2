<?php
/**
 * @brief Phone specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2026
 * @date Modification: Feb 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Validate phone number.
 * @param string $phone
 * @param string $country
 * @return boolean result
 */
function phone_validate(string $phone, string $country='') {
    /* Check country */
    if ($country === '')
        $country = phone_getCountryFromNumber($phone);

    /* Check specific country rules */
    if ($country !== false) {
        /* Get phone length specification */
        $lengthSpec = country_getPhoneLength($country);
        if ($lengthSpec !== 0 && $lengthSpec !== '') {
            /* Clean phone to count digits */
            $cleanPhone = preg_replace('/[^0-9]/', '', $phone);
            $actualLength = strlen($cleanPhone);

            /* Parse length specification */
            if (strpos($lengthSpec, '-') !== false) {
                /* Range format: "10-12" */
                list($min, $max) = explode('-', $lengthSpec);
                if ($actualLength < (int)$min || $actualLength > (int)$max)
                    return false;
            } elseif (strpos($lengthSpec, ',') !== false) {
                /* Comma-separated format: "7,12" */
                $validLengths = array_map('intval', explode(',', $lengthSpec));
                if (!in_array($actualLength, $validLengths))
                    return false;
            } else {
                /* Single value: "10" */
                if ($actualLength !== (int)$lengthSpec)
                    return false;
            }
            return true;
        }
    }

    /* Fallback to generic validation */
    return preg_match('/^\+?[0-9\s\-\.]{7,20}$/', $phone);
}

/**
 * Format phone number.
 * @param string $phone
 * @return string
 */
function phone_format(string $phone) {
    /* Remove all non-numeric characters except + */
    return preg_replace('/[^0-9\+]/', '', $phone);
}

/**
 * Return country code from phone number.
 * @param string $phone
 * @return string
 */
function phone_getCountryFromNumber(string $phone) {
    /* Clean phone number to keep only digits */
    $phone = preg_replace('/[^0-9]/', '', $phone);

    /* Load countries data */
    $countries = country_getCountriesData();

    /* Search for matching country */
    $candidate = false;
    $maxLen = 0;

    foreach ($countries as $code => $data) {
        /* Check if phone area code is defined */
        if (!isset($data['Phone']['IntlAreaCode']) or $data['Phone']['IntlAreaCode'] == '')
            continue;

        /* Clean area code (remove potential non-digits like -) */
        $areaCode = preg_replace('/[^0-9]/', '', $data['Phone']['IntlAreaCode']);

        /* Check if phone starts with this area code */
        if (strpos($phone, $areaCode) === 0) {
            /* Keep the longest matching area code (e.g. match 1-284 over 1) */
            if (strlen($areaCode) > $maxLen) {
                $maxLen = strlen($areaCode);
                $candidate = $code;
            }
        }
    }

    return $candidate;
}
?>
