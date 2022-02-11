<?php
/**
 * @brief ListItemPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: May 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ListItemPluginLibraryAttribute class.
 */
final class ListItemPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Length'=>0, 'Format'=>'Text', 'Values'=>array());

    /* Override PluginLibraryAttribute methods */

    /**
     * Class constructor.
     * @param Object $parent
     * @param string $name
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return void
     */
    public function __construct(&$parent, $name, $default, $params=array(), $isLocalised=false) {
        parent::__construct($parent, $name, $default, $params, $isLocalised);

        /* Define length value */
        if ($this->params['Length'] == 0) {
            foreach ($this->params['Values'] as $value)
                if (strlen((string)$value) + 4 > $this->params['Length'])
                    $this->params['Length'] = strlen((string)$value) + 4;
        }
    }


    /**
     * Return read value.
     * @param mixed $value
     * @return mixed
     */
    protected function read_value($value) {
        if ($this->params['Format'] == 'Integer')
            return (int)$value;
        if ($this->params['Format'] == 'Float')
            return (float)$value;
        return $value;
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Check value is in authorised values */
        if (!in_array($this->value, $this->params['Values']))
            return false;

        /* Return value is conform */
        return true;
    }
}
?>
