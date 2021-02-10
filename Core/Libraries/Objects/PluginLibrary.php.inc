<?php
/**
 * @brief PluginLibrary object.
 * <br />Abstract library to create plugin objects.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1.1
 * @date Creation: Jan 2021
 * @date Modification: Jan 2021
 * @copyright 2021 CaMykS Team
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
    public function &__get($name) {
        if (isset($this->libraryAttributes[$name]))
            return $this->libraryAttributes[$name]->value;
    }

    /**
     * Write attribute value as variable.
     * @param string $name
     * @param mixed $value
     * @return mixed
     */
    public function __set($name, $value) {
        if (isset($this->libraryValues[$name]))
            $this->libraryAttributes[$name]->value = $value;
    }

    /* Installation methods */

    /**
     * Install object.
     * @return boolean success
     */
    abstract public function install();

    /**
     * Uninstall object.
     * @return boolean success
     */
    abstract public function uninstall();

    /**
     * Check object installation.
     * @return boolean result
     */
    abstract public function is_installed();

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
     * @return boolean success
     */
    abstract public function load();

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
            $result = array_merge($result, $attribute->get_asArray());
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
}
?>
