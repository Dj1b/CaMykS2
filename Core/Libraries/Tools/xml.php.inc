<?php
/**
 * @brief XML specific methods
 * @details Engine / Tool Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */

/**
 * Encode PHP data to XML.
 * @param mixed $data
 * @param string $name
 * @return string
 */
function xml_encode($data, $name='xml') {
    /* Initialise result */
    $xml = '';

    /* Check name is an integer */
    if (is_int($name))
        $name = 'Item'.$name;

    if (is_array($data)) {
        /* Add array to XML */
        $xml .= '<'.$name.'>';
        if ($data == array_values($data)) {
            /* Indexed array */
            foreach($data as $index => $value) {
                $xml .= xml_encode($value, 'Item'.$name);
            }
        } else {
            /* Structured array */
            foreach($data as $index => $value) {
                $xml .= xml_encode($value, $index);
            }
        }
        $xml .= '</'.$name.'>';
    } elseif (is_string($data)) {
        /* Add string to XML */
        $xml .= '<'.$name.' type="string"><![CDATA['.$data.']]></'.$name.'>';
    } else {
        /* Add other type element to XML */
        $xml .= '<'.$name.' type="'.gettype($data).'">'.$data.'</'.$name.'>';
    }
    return $xml;
}
?>
