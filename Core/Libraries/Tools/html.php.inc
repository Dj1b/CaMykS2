<?php
/**
 * @brief HTML related methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Tags specific methods */

/**
 * Build and return img tag from given data.
 * @param array $params
 * @return string
 */
function html_getImg($params=array()) {
    /* Merge default params */
    $params = array_merge(array(
        'src'   => '',
        'alt'   => '',
    ), $params);

    /* Check source */
    if ($params['src'] == '')
        return '';

    /* Build tag */
    $s = '<img src="'.$params['src'].'" alt="'.$params['alt'].'"';
    $s .= html_getTagDefaultAttributes($params);
    $s .= ' />';

    /* Return result */
    return $s;
}

/**
 * Build and return a tag from given data.
 * @param array $params
 * @return string
 */
function html_getA($params=array()) {
    /* Merge default params */
    $params = array_merge(array(
        'href'          => '',
        'value'         => '',
        'anchor'        => '',
        'target'        => '',
        'rel'           => null,
        'nofollow'      => false,
        'noreferrer'    => false,
        'noopener'      => false,
    ), $params);

    /* Build tag */
    $s = '<a href="'.$params['href'];
    if ($params['anchor'] != '')
        $s .= '#'.$params['anchor'];
    $s .= '"';
    if ($params['target'] != '')
        $s .= ' target="'.$params['target'].'"';
    if (!is_null($params['rel']))
        $s .= ' rel="'.$params['rel'].'"';
    elseif ($params['nofollow'] or $params['noreferrer'] or $params['noopener']) {
        $s .= ' rel="';
        $t = array();
        if ($params['nofollow']) $t[] = 'nofollow';
        if ($params['noreferrer']) $t[] = 'noreferrer';
        if ($params['noopener']) $t[] = 'noopener';
        $s .= implode(' ', $t);
        $s .= '"';
    }
    $s .= html_getTagDefaultAttributes($params);
    $s .= '>'.$params['value'].'</a>';

    /* Return result */
    return $s;
}

/* Generic methods */

/**
 * Return html tag attributes for default parameters.
 * @param array $params
 * @return string
 */
function html_getTagDefaultAttributes($params) {
    /* Merge default params */
    $params = array_merge(array(
        'id'        => '',
        'style'     => '',
        'class'     => '',
        'title'     => '',
        'data'      => array(),
        'events'    => array(),
    ), $params);

    /* Build attributes */
    $s = '';
    if ($params['id'] != '')
        $s .= ' id="'.$params['id'].'"';
    if ($params['style'] != '')
        $s .= ' style="'.$params['style'].'"';
    if ($params['class'] != '')
        $s .= ' class="'.$params['class'].'"';
    if ($params['title'] != '')
        $s .= ' title="'.$params['title'].'"';
    if (is_array($params['data']) and count($params['data']) > 0)
        foreach ($params['data'] as $name => $value)
            $s .= 'data-'.$name.'="'.$value.'"';
    if (is_array($params['events']) and count($params['events']) > 0)
        foreach ($params['events'] as $name => $value)
            $s .= 'on'.$name.'="'.$value.'"';

    /* Return result */
    return $s;
}
?>
