<?php
/**
 * @brief Phone specific methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Feb 2026
 * @date Modification: Sep 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * Parse phone number to extract components.
 * @param string $phone
 * @param string $country
 * @return array|false components list
 */
function phone_parse(string $phone, string $country='') {
    /* Check country */
    if ($country === '')
        $country = phone_getCountryFromNumber($phone);
    if ($country === false) return false;

    /* Get country data */
    $data = country_get($country);
    if ($data === false) return false;

    /* Clean phone number to keep only digits */
    $phone = preg_replace('/[^0-9]/', '', $phone);

    /* Extract components */
    $fullAreaCode = preg_replace('/[^0-9]/', '', $data['Phone']['IntlAreaCode']);
    $trunkPrefix = (string)$data['Phone']['TrunkPrefix'];

    $prefix = $fullAreaCode;
    $trunk = $trunkPrefix;
    $number = $phone;

    /* If number starts with international area code */
    if (strpos($phone, $fullAreaCode) === 0) {
        /* Check if IntlAreaCode has a dash (separating country code from area code) */
        if (strpos($data['Phone']['IntlAreaCode'], '-') !== false) {
            list($countryCode, $areaPart) = explode('-', $data['Phone']['IntlAreaCode']);
            $prefix = preg_replace('/[^0-9]/', '', $countryCode);
            $number = substr($phone, strlen($prefix));
        } else {
            $number = substr($phone, strlen($fullAreaCode));
        }
    } elseif ($trunkPrefix !== '' && strpos($phone, $trunkPrefix) === 0) {
        /* If number starts with trunk prefix */
        $number = substr($phone, strlen($trunkPrefix));
    }

    return array(
        'country' => $country,
        'prefix'  => $prefix,
        'trunk'   => $trunk,
        'number'  => $number,
    );
}

/**
 * Validate phone number.
 * @param string $phone
 * @param string $country
 * @return boolean result
 */
function phone_validate(string $phone, string $country='') {
    /* Parse phone number */
    $parsed = phone_parse($phone, $country);

    /* Check specific country rules */
    if ($parsed !== false) {
        /* Get phone length specification */
        $lengthSpec = country_getPhoneLength($parsed['country']);
        if ($lengthSpec !== 0 && $lengthSpec !== '') {
            $actualLength = strlen($parsed['trunk'] . $parsed['number']);

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
 * Check if phone number is a mobile number.
 * @param string $phone
 * @param string $country
 * @return boolean result
 */
function phone_isMobile(string $phone, string $country='') {
    /* Parse phone number */
    $parsed = phone_parse($phone, $country);

    /* Check specific country rules */
    if ($parsed !== false) {
        $data = country_get($parsed['country']);
        if ($data !== false && isset($data['Phone']['MobileCheck']) && $data['Phone']['MobileCheck'] !== '') {
            return preg_match($data['Phone']['MobileCheck'], $parsed['trunk'] . $parsed['number']);
        }
    }

    /* Fallback to false if no specific rule */
    return false;
}

/**
 * Format phone number.
 * @param string $phone
 * @param string $country
 * @param boolean $intl
 * @param string $separator
 * @return string
 */
function phone_format(string $phone, string $country='', bool $intl=true, string $separator='') {
    /* Parse phone number */
    $parsed = phone_parse($phone, $country);

    /* Fallback to simple format if parsing failed */
    if ($parsed === false)
        return preg_replace('/[^0-9\+]/', '', $phone);

    /* Return formatted number */
    if ($intl)
        return '+' . $parsed['prefix'] . $separator . $parsed['number'];
    return $parsed['trunk'] . $separator . $parsed['number'];
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
