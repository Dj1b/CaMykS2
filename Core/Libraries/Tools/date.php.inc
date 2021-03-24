<?php
/**
 * @brief Date related methods.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Mar 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * @def NOW
 */
define('NOW',       time());

/**
 * @def TODAY
 */
define('TODAY',     mktime(0, 0, 0, date('m'), date('d'), date('Y')));

/**
 * @def TOMORROW
 */
define('TOMORROW',  mktime(0, 0, 0, date('m'), date('d')+1, date('Y')));

/**
 * @def YESTERDAY
 */
define('YESTERDAY', mktime(0, 0, 0, date('m'), date('d')-1, date('Y')));

/* Textual display methods */

/**
 * Return textual representation of day, date and hour.
 * @param date $date
 * @return string
 */
function date_getTextualDayDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%A %e %B %Y &agrave; %H:%M', $date));
        case 'en':
            return strftime('%A, %B the %e'.date('S', $date).' %Y, %H:%M', $date);
        case 'es':
            return ucwords(strftime('%A %e', $date)).' de '.ucwords(strftime('%B %Y, %H:%M', $date));
        default:
            return strftime('%A %e %B %Y, %H:%M', $date);
    }
}

/**
 * Display textual representation of date and hour.
 * @param date $date
 * @return string
 */
function date_getTextualDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%e %B %Y &agrave; %H:%M', $date));
        case 'en':
            return strftime('%B the %e'.date('S', $date).' %Y, %H:%M', $date);
        case 'es':
            return strftime('%e', $date).' de '.ucwords(strftime('%B %Y, %H:%M', $date));
        default:
            return strftime('%e %B %Y, %H:%M', $date);
    }
}

/**
 * Display textual representation of day and date.
 * @param date $date
 * @return string
 */
function date_getTextualDayDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%A %e %B %Y', $date));
        case 'en':
            return strftime('%A, %B  the %e'.date('S', $date).' %Y', $date);
        case 'es':
            return ucwords(strftime('%A %e', $date)).' de '.ucwords(strftime('%B %Y', $date));
        default:
            return strftime('%A %e %B %Y', $date);
    }
}

/**
 * Display textual representation of date.
 * @param date $date
 * @return string
 */
function date_getTextualDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%e %B %Y', $date));
        case 'en':
            return strftime('%B the %e'.date('S', $date).' %Y', $date);
        case 'es':
            return ucwords(strftime('%e', $date)).' de '.ucwords(strftime('%B %Y', $date));
        default:
            return strftime('%e %B %Y', $date);
    }
}

/* Long display methods */

/**
 * Display long representation of day, date and hour.
 * @param date $date
 * @return string
 */
function date_getLongDayDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%A %e %B %Y, %H:%M', $date));
        case 'en':
            return strftime('%A, %e'.date('S', $date).' %B %Y, %H:%M', $date);
        case 'es':
            return ucwords(strftime('%A %e %B %Y, %H:%M', $date));
        default:
            return strftime('%A %e %B %Y, %H:%M', $date);
    }
}

/**
 * Display long textual representation of date and hour.
 * @param date $date
 * @return string
 */
function date_getLongDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%e %B %Y, %H:%M', $date));
        case 'en':
            return strftime('%e'.date('S', $date).' %B %Y, %H:%M', $date);
        case 'es':
            return ucwords(strftime('%e %B %Y, %H:%M', $date));
        default:
            return strftime('%e %B %Y, %H:%M', $date);
    }
}

/**
 * Display long textual representation of day and date.
 * @param date $date
 * @return string
 */
function date_getLongDayDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%A %e %B %Y', $date));
        case 'en':
            return strftime('%A, %e'.date('S', $date).' %B %Y', $date);
        case 'es':
            return ucwords(strftime('%A %e %B %Y', $date));
        default:
            return strftime('%A %e %B %Y', $date);
    }
}

/**
 * Display long textual representation of date.
 * @param date $date
 * @return string
 */
function date_getLongDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%e %B %Y', $date));
        case 'en':
            return strftime('%e'.date('S', $date).' %B %Y', $date);
        case 'es':
            return ucwords(strftime('%e %B %Y', $date));
        default:
            return strftime('%e %B %Y', $date);
    }
}

/**
 * Display long textual representation of date interval.
 * @param date $date1
 * @param date $date2
 * @return string
 */
function date_getLongDateInterval($date1=NOW, $date2=NOW) {
    global $CaMykS;
    $l = $CaMykS->get_currentLanguage();

    if ($date1 > $date2) {
        $tmp = $date1;
        $date1 = $date2;
        $date2 = $tmp;
        unset($tmp);
    }

    if (date_isSameDay($date1, $date2))
        return date_getShortDate($date1);

    if (date_isSameMonth($date1, $date2))
        return strftime('%e', $date1).' - '.ucwords(strftime('%e %B %Y', $date2));

    if (date_isSameYear($date1, $date2))
        return ucwords(strftime('%e %B', $date1)).' - '.ucwords(strftime('%e %B %Y', $date2));

    return ucwords(strftime('%e %B %Y', $date1)).' - '.ucwords(strftime('%e %B %Y', $date2));
}

/* Short display methods */

/**
 * Display short textual representation of day, date and hour.
 * @param date $date
 * @return string
 */
function date_getShortDayDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%a %e %b %y - %H:%M', $date));
        case 'en':
            return strftime('%a %b %e'.date('S', $date).' %y, %H:%M', $date);
        default:
            return strftime('%a %e %b %y, %H:%M', $date);
    }
}

/**
 * Display short textual representation of date and hour.
 * @param date $date
 * @return string
 */
function date_getShortDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%e %b %y - %H:%M', $date));
        case 'en':
            return strftime('%b %e'.date('S', $date).' %y, %H:%M', $date);
        default:
            return strftime('%e %b %y, %H:%M', $date);
    }
}

/**
 * Display short textual representation of day and date.
 * @param date $date
 * @return string
 */
function date_getShortDayDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%a %e %b %y', $date));
        case 'en':
            return strftime('%a %b %e'.date('S', $date).' %y', $date);
        default:
            return strftime('%a %e %b %y', $date);
    }
}

/**
 * Display short textual representation of date.
 * @param date $date
 * @return string
 */
function date_getShortDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return ucwords(strftime('%e %b %y', $date));
        case 'en':
            return strftime('%b %e'.date('S', $date).' %y', $date);
        default:
            return strftime('%e %b %y', $date);
    }
}

/**
 * Display short textual representation of date interval.
 * @param date $date1
 * @param date $date2
 * @return string
 */
function date_getShortDateInterval($date1=NOW, $date2=NOW) {
    global $CaMykS;
    $l = $CaMykS->get_currentLanguage();

    if ($date1 > $date2) {
        $tmp = $date1;
        $date1 = $date2;
        $date2 = $tmp;
        unset($tmp);
    }

    if (date_isSameDay($date1, $date2))
        return date_getShortDate($date1);

    if (date_isSameMonth($date1, $date2))
        return strftime('%e', $date1).' - '.ucwords(strftime('%e %b %y', $date2));

    if (date_isSameYear($date1, $date2))
        return ucwords(strftime('%e %b', $date1)).' - '.ucwords(strftime('%e %b %y', $date2));

    return ucwords(strftime('%e %b %y', $date1)).' - '.ucwords(strftime('%e %b %y', $date2));
}

/* Nimeric display methods */

/**
 * Display numeric representation of date and hour.
 * @param date $date
 * @return string
 */
function date_getNumericDateHour($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return strftime('%d/%m/%y - %H:%M', $date);
        case 'en':
            return strftime('%D - %H:%M', $date);
        default:
            return strftime('%D - %H:%M', $date);
    }
}

/**
 * Display numeric representation of date.
 * @param date $date
 * @return string
 */
function date_getNumericDate($date=NOW) {
    global $CaMykS;

    /* Load language */
    $language = $CaMykS->get_currentLanguage();
    $language = $language['Language'];

    /* Check language for result */
    switch ($language) {
        case 'fr':
            return strftime('%d/%m/%y', $date);
        case 'en':
            return strftime('%D', $date);
        default:
            return strftime('%D', $date);
    }
}

/* Other quick display methods */

/**
 * Display time.
 * @param date $date
 * @return string
 */
function date_getTime($date=NOW) {
    return strftime('%H:%M', $date);
}

/**
 * Display day.
 * @param date $date can be a week day
 * @return string
 */
function date_getDay($date=NOW) {
    if ($date <= 7)
        $date = mktime(0, 0, 0, 1, 2+$date, 0);
    return strftime('%A', $date);
}

/* Test methods */

/**
 * Check if dates are the same day.
 * @param date $date1
 * @param date $date2
 * @return boolean result
 */
function date_isSameDay($date1=NOW, $date2=NOW) {
    return mktime(0, 0, 0, date('m', $date1), date('d', $date1), date('Y', $date1)) == mktime(0, 0, 0, date('m', $date2), date('d', $date2), date('Y', $date2));
}

/**
 * Check if dates are the same month.
 * @param date $date1
 * @param date $date2
 * @return boolean result
 */
function date_isSameMonth($date1=NOW, $date2=NOW) {
    return mktime(0, 0, 0, date('m', $date1), 1, date('Y', $date1)) == mktime(0, 0, 0, date('m', $date2), 1, date('Y', $date2));
}

/**
 * Check if dates are the same year.
 * @param date $date1
 * @param date $date2
 * @return boolean result
 */
function date_isSameYear($date1=NOW, $date2=NOW) {
    return date('Y', $date1) == date('Y', $date2);
}

/**
 * Check if date is today.
 * @param date $date
 * @return boolean result
 */
function date_isToday($date=NOW) {
    return date_isSameDay($date);
}

/**
 * Check if date is tomorrow.
 * @param date $date
 * @return boolean result
 */
function date_isTomorrow($date=NOW) {
    return mktime(0, 0, 0, date('m', $date), date('d', $date), date('Y', $date)) == TOMORROW;
}

/**
 * Check if date is yesterday.
 * @param date $date
 * @return boolean result
 */
function date_isYesterday($date=NOW) {
    return mktime(0, 0, 0, date('m', $date), date('d', $date), date('Y', $date)) == YESTERDAY;
}

/* Time specific methods */

/**
 * Transform an float to a time.
 * @params integer $value
 * @param string $hSep
 * @param mixed $mSep
 * @return string
 */
function date_float2Time($value=0, $hSep=':', $mSep=false) {
    /* Make sure value is a float  */
    $value = (float)$value;

    /* Make sure value is a correct hour of a day */
    $value = fmod($value, 24);

    /* Add hours */
    $result = str_pad((int)$value, 2, '0', STR_PAD_LEFT);

    /* Add minutes */
    $result .= $hSep;
    $minutes = ($value-(int)$value) * 60;
    $result .= str_pad((int)$minutes, 2, '0', STR_PAD_LEFT);

    /* Add seconds */
    if ($mSep !== false) {
        $result .= $mSep;
        $seconds = ($minutes-(int)$minutes) * 60;
        $result .= str_pad((int)$seconds, 2, '0', STR_PAD_LEFT);
    }

    /* Return result */
    return $result;
}
?>
