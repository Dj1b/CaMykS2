<?php
/**
 * @brief PluginLibrary object.
 * <br />Abstract library to create plugin objects.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Jan 2021
 * @date Modification: Jan 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * PluginLibrary class.
 */
abstract class PluginLibrary extends CaMykSLibrary {
    /**
     * @var Object $libraryParent
     * @brief Parent plugin.
     */
    protected $libraryParent;

    /**
     * @var array $libraryAttributes
     * @brief Library attributes.
     */
    protected $libraryAttributes = array();

    /**
     * var array $actionMessages
     * @brief Stores action messages.
     */
    protected $actionMessages = array();

    /**
     * var array $actionErrors
     * @brief Stores action errors.
     */
    protected $actionrrors = array();

    /**
     * Class constructor.
     * @param Object $parent
     * @return void
     */
    public function __construct(&$parent) {
        /* Execute parent construct */
        parent::__construct();

        /* Define library variables */
        $this->libraryParent = $parent;
    }

    /* Magic methods */

    /**
     * Read attribute value as variable.
     * @param string $name
     * @return mixed
     */
    public function __get($name) {
        if (isset($this->libraryAttributes[$name]))
            return $this->libraryAttributes[$name]->get_value();
    }

    /**
     * Write attribute value as variable.
     * @param string $name
     * @param mixed $value
     * @return mixed
     */
    public function __set($name, $value) {
        if (isset($this->libraryValues[$name]))
            $this->libraryAttributes[$name]->set_value($value);
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            /* Attributes errors */
            1 => 'UnknownAttributeType',
            2 => 'DuplicatedAttributeName',
            3 => 'InvalidAttributeName',
        );
    }

    /* Data management methods */

    /**
     * Initialise object.
     * @return void
     */
    public function initialise() {
        foreach ($this->libraryAttributes as $attribute) {
            $attribute->initialise();
        }
    }

    /**
     * Load object from its main source.
     * @param mixed $language
     * @return boolean success
     */
    abstract public function load($language=true);

    /**
     * Save object to its main source.
     * @return boolean success
     */
    abstract public function save();

    /**
     * Delete object source.
     * @return boolean success
     */
    abstract public function delete();

    /* Values related methods */

    /**
     * Return values as array.
     * @return void
     */
    public function toArray() {
        $result = array();
        foreach ($this->libraryAttributes as $attribute)
            $result = array_merge($result, $attribute->toArray());
        return $result;
    }

    /**
     * Load values from array.
     * @param array $array
     * @param boolean $update
     * @return void
     */
    public function load_fromArray($array=array(), $update=false) {
        foreach ($this->libraryAttributes as $attribute) {
            $attribute->load_fromArray($array, $update);
        }
    }

    /**
     * Load values from header.
     * @param string $prefix
     * @param boolean $update
     * @return void
     */
    public function load_fromHeader($prefix='', $update=false) {
        foreach ($this->libraryAttributes as $attribute) {
            $attribute->load_fromHeader($prefix, $update);
        }
    }

    /**
     * Load values from session.
     * @param string $name
     * @return void
     */
    public function load_fromSession($name) {
        global $CaMykS;
        if (!is_null($values = $CaMykS->get_sessionValue($name)))
            $this->load_fromArray($values);
        else
            $this->initialise();
    }

    /**
     * Save values to session.
     * @param string $name
     * @return void
     */
    public function save_toSession($name) {
        global $CaMykS;
        $CaMykS->set_sessionValue($name, $this->toArray());
    }

    /* Override locales related methods */

    /**
     * Check if locale exists.
     * @param string $locale
     * @return boolean result
     */
    public function check_locale($locale='') {
        return $this->libraryParent->check_locale($locale);
    }

    /**
     * Return locale.
     * @param string $locale
     * @param mixed $data
     * @return string
     */
    public function get_locale($locale='', $data=null) {
        return $this->libraryParent->get_locale($locale, $data);
    }

    /**
     * Load library locales.
     * @param string $folder
     * @param string $language
     * @return boolean success
     */
    protected function load_locales($folder='', $language=null) {
        return $this->libraryParent->load_locales($folder, $language);
    }

    /* Attributes related methods */

    /**
     * Add attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean result
     */
    protected function define_attribute($name, $type, $default, $params=array(), $isLocalised=false) {
        /* Check name is already used */
        if (isset($this->libraryAttributes[$name])) {
            $this->add_error(2, 'Duplicate name is '.$name, __FILE__, __LINE__);
            return false;
        }

        /* Check name is invalid */


        /* Build attribute type */
        $attributeType = $this->libraryNamespace.'\\'.$type.'PluginLibraryAttribute';

        /* Check type */
        if (!class_exists($attributeType)) {
            $this->add_error(1, 'Unknown attribute type '.$type, __FILE__, __LINE__);
            return false;
        }

        /* Build attribute from type */
        $this->libraryAttributes[$name] = new $attributeType($this, $name, $default, $params, $isLocalised);

        /* Return success */
        return true;
    }

    /**
     * Check attributes values.
     * @return boolean success
     */
    protected function check_attributesValues() {
        /* Initialise result */
        $result = true;

        /* Check all attributes */
        foreach ($this->libraryAttributes as $attribute) {
            if (!$attribute->check_value())
                $result = false;
        }

        /* Return result */
        return $result;
    }

    /* Action errors & messages methods */

    /**
     * Add action message.
     * @return void
     */
    protected function add_actionMessage($message) {
        $this->actionMessages[] = $messages;
    }

    /**
     * Return action messages.
     * @return array
     */
    public function get_actionMessages() {
        return $this->actionMessages;
    }

    /**
     * Add action error.
     * @return void
     */
    protected function add_actionError($error) {
        $this->actionErrors[] = $error;
    }

    /**
     * Return action error.
     * @return array
     */
    public function get_actionErrors() {
        return $this->actionErrors;
    }
}
?>
