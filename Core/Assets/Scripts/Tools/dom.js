/**
 * @brief DOM and HTML elements related methods.
 * @details Plugin / Plugin Javascripts
 * @author CaMykS Team
 * @version 1.0pre0
 * @date Creation: Dec 2021
 * @date Modification: Dec 2021
 * @copyright 2021 CaMykS
 */

/**
 * Return maximum z-index value for given element tags.
 * @param string tagname
 * @return integer
 */
function dom_getMaxZIndex(tagName) {
    /* Check tag */
    if (!tagName) tagName = '*';

    /* Define the lowest value */
    var highest = Number.MIN_SAFE_INTEGER || -(Math.pow(2, 53) - 1);

    /* Load all relevant tag elements */
    var elements = document.getElementsByTagName(tagName);

    /* Compare zindexes */
    for (var i = 0; i < elements.length; i++) {
        var zindex = Number.parseInt(document.defaultView.getComputedStyle(elements[i], null).getPropertyValue("z-index"), 10);
        if (zindex > highest)
            highest = zindex;
    }

    /* Return result */
    return highest;
}
