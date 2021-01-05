<?php
/**
 * @brief PluginLibrary object.
 * <br />Abstract library to create plugin objects.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2021
 * @date Modification: Jan 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ModuleLibrary class.
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
     * @var array $libraryValues
     * @brief Values loaded from attributes.
     */
    protected $libraryValues = array();

    /**
     * Class constructor.
     * @param string $location
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
        if (isset($this->libraryValues[$name]))
            return $this->libraryValues[$name];
    }

    /**
     * Write attribute value as variable.
     * @param string $name
     * @param mixed $value
     * @return mixed
     */
    public function __set($name, $value) {
        if (isset($this->libraryValues[$name]))
            $this->libraryValues[$name] = $value;
    }

    /* Data management methods */

    /**
     * Initialise object.
     * @return void
     */
    public function initialise() {
        foreach ($this->libraryAttributes as $attribute) {
            $this->update_fromArray($attribute->get_defaultValue());
        }
    }

    /**
     * Load values from array.
     * @param array $values
     * @return void
     */
    public function load_fromArray($values=array()) {
        $this->libraryValues = $values;
    }

    /**
     * Update values from array.
     * @param array $values
     * @return void
     */
    public function update_fromArray($values=array()) {
        $this->libraryValues = array_merge($this->libraryValues, $values);
    }

    /**
     * Load values from session.
     * @param string $name
     * @return void
     */
    public function load_fromSession($name) {
        global $CaMykS;
        if (!is_null($values = $CaMykS->get_sessionValue($name)))
            $this->libraryValues = $values;
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
        $CaMykS->set_sessionValue($name, $this->libraryValues);
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
