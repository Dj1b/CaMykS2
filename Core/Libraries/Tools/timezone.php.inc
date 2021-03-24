<?php
/**
 * @brief CaMykS time zone(s) library.
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation:  Mar 2021
 * @date Modification: Mar 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */

/* Time zone data related methods */

/**
 * Return timezones data.
 * @return array
 */
function timezone_getTimezonesData() {
    global $CaMykS;

    return $CaMykS->get_assetData('timeZones');
}


/**
 * Return timezone locations data.
 * @return array
 */
function timezone_getTimezoneLocationsData() {
    global $CaMykS;

    return $CaMykS->get_assetData('timeZoneLocations');
}

/* Time zone list related methods */

/**
 * Return time zone title list.
 * @param array $params
 * @return array
 */
function timezone_getZoneTitles($params=array()) {
    /* Build default params */
    $defaultParams = array('Display'=>'Name', 'Type'=>'All');

    /* Merge params */
    $params = array_merge($defaultParams, $params);

    /* Load data file */
    $timezonesData = timezone_getTimezonesData();

    /* Initialise result */
    $result = array();

    /* Build list */
    foreach ($timezonesData as $name => $zone) {
        /* Check for Standard or Daylight zone */
        if (($params['Type'] == 'Standard' and $params['STZ'] == '') or ($params['Type'] == 'Daylight' and $params['DSTZ'] == ''))
            continue;
        if ($params['Display'] == 'Name')
            $result[$name] = $name;
        elseif ($params['Display'] == 'LongName')
            $result[$name] = timezone_getLongName($name);
    }

    /* Return result */
    return $result;
}

/**
 * Return time zone location title list.
 * @param array $params
 * @return array
 */
function timezone_getLocationTitles($params=array()) {
    /* Build default params */
    $defaultParams = array('Region'=>'', 'Country'=>'', 'Sort'=>'Name', 'HideRegion'=>false, 'AddOffset'=>false, 'GroupByZone'=>false);

    /* Merge params */
    $params = array_merge($defaultParams, $params);

    /* Load data file */
    $timezoneLocationsData = timezone_getTimezoneLocationsData();

    /* Initialise result */
    $result = array();

    /* Check for country zones */
    if ($params['Country'] != '')
        $subList = country_getTimezones($params['Country']);
    elseif ($params['Region'] !== '') {
        $subList = array();
        foreach ($timezoneLocationsData as $name => $location)
             if (string_startsWith($name, $params['Region']))
                $subList[] = $name;
    } else
        $subList = array_keys($timezoneLocationsData);

    /* Check for timezone data requirement */
    if ($params['Sort'] != 'Name' or $params['AddOffset'] or $params['GroupByZone'])
        $timezonesData = timezone_getTimezonesData();

    /* Check for Zone sorting (Grouping by Zone automatically enables Zone sorting) */
    if ($params['Sort'] === 'Zone' or $params['GroupByZone']) {
        $zones = array();
        foreach ($timezonesData as $name => $zone)
            $zones[$name] = array();
        foreach ($subList as $name)
            $zones[$timezoneLocationsData[$name]['STZ']][] = $name;
        $subList = call_user_func_array('array_merge', $zones);

    /* Check for Offset sorting */
    } elseif ($params['Sort'] === 'Offset') {
        $offsets = array();
        foreach ($subList as $name) {
            if (!isset($offsets[$timezonesData[$timezoneLocationsData[$name]['STZ']]['Offset']]))
                $offsets[$timezonesData[$timezoneLocationsData[$name]['STZ']]['Offset']] = array();
            $offsets[$timezonesData[$timezoneLocationsData[$name]['STZ']]['Offset']][] = $name;
        }
        ksort($offsets);
        $subList = call_user_func_array('array_merge', $offsets);
    }

    /* Prepare zone displaying */
    $previousZone = '';

    /* Build list */
    foreach ($subList as $name) {
        /* Prepare name value */
        $fullName = $name;

        /* Check for zone title insertion */
        if ($params['GroupByZone'] and $timezoneLocationsData[$name]['STZ'] != $previousZone) {
            $result[$timezoneLocationsData[$name]['STZ']] = array('value'=> timezone_getLongName($timezoneLocationsData[$name]['STZ']), 'class'=>'title', 'disabled'=>'disabled');
            $previousZone = $timezoneLocationsData[$name]['STZ'];
        }

        /* Check for hidding region */
        if ($params['HideRegion']) {
            $fullName = explode('/', $name);
            $fullName = $fullName[1];
        }

        /* Check for offset adding */
        if ($params['AddOffset'])
            $fullName = '('.timezone_getOffsetText($timezoneLocationsData[$name]['STZ']).') '.$name;

        /* Add to result */
        $result[$name] = $fullName;
    }

    /* Return result */
    return $result;
}

/* Time zone related methods */

/**
 * Return zone long name, include offset and name.
 * @param string $zone
 * @return string
 */
function timezone_getLongName($zone) {
    /* Load data file */
    $timezonesData = timezone_getTimezonesData();

    /* Check zone */
    if (!isset($timezonesData[$zone]))
        return '';

    /* Return value */
    return '('.timezone_getOffsetText($zone).') '.$zone;
}

/**
 * Return zone offest text.
 * @param string $zone
 * @return string
 */
function timezone_getOffsetText($zone) {
    /* Load data file */
    $timezonesData = timezone_getTimezonesData();

    /* Check zone */
    if (!isset($timezonesData[$zone]))
        return '';

    /* Build string */
    $string = 'UTC';

    /* Add sign */
    if ($timezonesData[$zone]['Offset'] < 0)
        $string .= '-';
    elseif ($timezonesData[$zone]['Offset'] === 0)
        $string .= '±';
    else
        $string .= '+';

    /* Add time and return */
    return $string.date_float2Time(abs($timezonesData[$zone]['Offset']));
}

?>
