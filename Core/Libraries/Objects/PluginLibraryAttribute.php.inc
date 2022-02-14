<?php
/**
 * @brief PluginLibraryAttribute object.
 * <br />Abstract library to create plugin library attributes.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre6
 * @date Creation: Jan 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

define('ATTRIBUTE_FULLNAME_SEPARATOR', '_');

/**
 * PluginLibraryAttribute class.
 */
abstract class PluginLibraryAttribute {
    /**
     * @var Object $libraryParent
     * @brief Parent plugin.
     */
    protected $libraryParent;

    /**
     * @var string $libraryName
     * @brief Library name.
     */
    public $libraryName;

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    protected const DEFAULTPARAMS = array('Mandatory'=>false, 'IndexName'=>false, 'IndexLength'=>12, 'IsUnique'=>false);

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array();

    /**
     * @var array $params
     * @brief Attribute parameters
     */
    protected $params = array();

    /**
     * @var mixed $defaultValue
     * @brief Attribute default value
     */
    protected $defaultValue;

     /**
     * @var mixed $value
     * @brief Attribute current value
     */
    protected $value;

    /**
     * @var boolean $isLocalised
     * @brief Is attribute localised
     */
    public $isLocalised = false;

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
        /* Define library variables */
        $this->libraryParent = $parent;
        $this->libraryName = $name;

        /* Define other variables */
        $this->defaultValue = $default;
        $this->isLocalised = $isLocalised;

        /* Define original value */
        $this->value = $this->get_defaultValue();

        /* Merge parameters */
        $this->params = array_merge(self::DEFAULTPARAMS, $this::DEFAULTPARAMS, $this::SUBPARAMS, $params);
    }

    /* Value related methods */

    /**
     * Return current value.
     * @param string $language
     * @return mixed
     */
    public function get_value($language=null) {
        global $CaMykS;

        if ($this->isLocalised) {
            if (is_null($language))
                $language = $CaMykS->get_currentLanguage();
            if (isset($this->value[$language]))
                return $this->value[$language];
            return null;
        }
        return $this->value;
    }

    /**
     * Set current value.
     * @param mixed $value
     * @param string $language
     * @return void
     */
    public function set_value($value, $language=null) {
        global $CaMykS;

        if ($this->isLocalised) {
            if (is_null($language))
                $language = $CaMykS->get_currentLanguage();
            if (isset($this->value[$language]))
                $this->value[$language] = $value;
        }
        $this->value = $value;
    }

    /**
     * Initialise attribute.
     * @return void
     */
    public function initialise() {
        if ($this->isLocalised) {
            $this->value = array();
            foreach ($this->get_configValue('Admin_EditionLanguages') as $language) {
                $this->value[$language] = $this->get_defaultValue();
            }
        } else {
            $this->value = $this->get_defaultValue();
        }
    }

    /**
     * Load values from saving location.
     * @param array $array
     * @return void
     */
    public function load_fromSave($array=array()) {
        if ($this->isLocalised) {
            foreach ($this->get_configValue('Admin_EditionLanguages') as $language) {
                if (isset($array[$this->get_fullName($language)]))
                    $this->value[$language] = $this->read_valueFromSave($array[$this->get_fullName($language)]);
                else
                    $this->value[$language] = $this->get_defaultValue();
            }
        } else {
            if (isset($array[$this->get_fullName()]))
                $this->value = $this->read_valueFromSave($array[$this->get_fullName()]);
            else
                $this->value = $this->get_defaultValue();
        }
    }

    /**
     * Return optimised value for saving.
     * @return void
     */
    public function export_forSaving() {
        return $this->value;
    }

    /**
     * Load values from header.
     * @param string $prefix
     * @param boolean $update
     * @return void
     */
    public function load_fromHeader($prefix='', $update=false) {
        if ($this->isLocalised) {
            foreach ($this->get_configValue('Admin_EditionLanguages') as $language) {
                if (!is_null($value = $this->load_valueFromHeader($this->get_fullName($language, $prefix))))
                    $this->value[$language] = $value;
                elseif ($update === false)
                    $this->value[$language] = $this->get_defaultValue();
            }
        } else {
            if (!is_null($value = $this->load_valueFromHeader($this->get_fullName($language, $prefix))))
                $this->value[$language] = $value;
            elseif ($update === false)
                $this->value[$language] = $this->get_defaultValue();
        }
    }

    /**
     * Load attribute value from array.
     * @param array $array
     * @param boolean $update
     * @return void
     */
    public function load_fromArray($array=array(), $update=false) {
        if ($this->isLocalised) {
            foreach ($this->get_configValue('Admin_EditionLanguages') as $language) {
                if (isset($array[$this->get_fullName($language)]))
                    $this->value[$language] = $this->read_value($array[$this->get_fullName($language)]);
                elseif ($update === false)
                    $this->value[$language] = $this->get_defaultValue();
            }
        } else {
            if (isset($array[$this->get_fullName()]))
                $this->value = $this->read_value($array[$this->get_fullName()]);
            elseif ($update === false)
                $this->value = $this->get_defaultValue();
        }
    }

    /**
     * Return values as array.
     * @return void
     */
    public function toArray() {
        if ($this->isLocalised) {
            $result = array();
            foreach ($this->get_configValue('Admin_EditionLanguages') as $language) {
                $result[$this->get_fullName($language)] = $this->value[$language];
            }
            return $result;
        }
        return array($this->get_fullName() => $this->value);
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    abstract public function export_databaseParameters();

    /* Specific tool methods */

    /**
     * Return read value from save.
     * @param mixed $value
     * @return mixed
     */
    protected function read_valueFromSave($value) {
        return $value;
    }

    /**
     * Return optimised value for save.
     * @param mixed $value
     * @return $mixed
     */
    protected function optimise_valueToSave($value) {
        return $value;
    }

    /**
     * Return read value.
     * @param mixed $value
     * @return mixed
     */
    protected function read_value($value) {
        return $value;
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        return true;
    }

    /**
     * Return default value.
     * @return mixed
     */
    protected function get_defaultValue() {
        return $this->defaultValue;
    }

    /**
     * Load single value from header.
     * @param string $name
     * @return mixed
     */
    protected function load_valueFromHeader($name) {
        if (!isset($_REQUEST[$name]))
            return null;
        return $this->read_value($_REQUEST[$name]);
    }

    /**
     * Return attribute full name including name, language and prefix.
     * @param string $language
     * @param string $prefix
     * @return string
     */
    protected function get_fullName($language=null, $prefix=null) {
        /* Initialise name */
        $name = $this->libraryName;

        /* Check for prefix */
        if (!is_null($prefix))
            $name = $prefix.ATTRIBUTE_FULLNAME_SEPARATOR.$name;

        /* Check for language */
        if (!is_null($language))
            $name .= ATTRIBUTE_FULLNAME_SEPARATOR.$language;

        /* Return built name */
        return $name;
    }
}
?>
