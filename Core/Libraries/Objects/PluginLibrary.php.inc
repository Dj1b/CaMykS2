<?php
/**
 * @brief PluginLibrary object.
 * <br />Abstract library to create plugin objects.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre9
 * @date Creation: Jan 2021
 * @date Modification: Oct 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * PluginLibrary class.
 */
abstract class PluginLibrary extends CaMykSLibrary {
    /**
     * @var string $libraryShortName
     * @brief Library short name.
     */
    protected string $libraryShortName;

    /**
     * @var Plugin $libraryParent
     * @brief Parent plugin.
     */
    protected Plugin $libraryParent;

    /**
     * @var array $libraryAttributes
     * @brief Library attributes.
     */
    protected array $libraryAttributes = array();

    /**
     * @var array $actionMessages
     * @brief Stores action messages.
     */
    protected array $actionMessages = array();

    /**
     * @var array $actionErrors
     * @brief Stores action errors.
     */
    protected array $actionErrors = array();

    /**
     * Class constructor.
     * @param Plugin $parent
     * @return void
     */
    public function __construct(Plugin &$parent) {
        /* Execute parent construct */
        parent::__construct();

        /* Define library variables */
        $this->libraryParent = $parent;
        $this->libraryShortName = str_replace($parent->libraryName, '', $this->libraryName);
    }

    /* Mark: Magic methods */

    /**
     * Read attribute value as variable.
     * @param string $name
     * @return mixed
     */
    public function __get(string $name): mixed {
        if (isset($this->libraryAttributes[$name]))
            return $this->libraryAttributes[$name]->get_value();
        return null;
    }

    /**
     * Write attribute value as variable.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function __set(string $name, mixed $value): void {
        if (isset($this->libraryAttributes[$name]))
            $this->libraryAttributes[$name]->set_value($value);
    }

    /* Mark: Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes(): void {
        $this->errorCodes = array(
            /* Attributes errors */
            1 => 'UnknownAttributeType',
            2 => 'DuplicatedAttributeName',
            3 => 'InvalidAttributeName',
        );
    }

    /* Mark: Data management methods */

    /**
     * Initialise object.
     * @return void
     */
    public function initialise(): void {
        foreach ($this->libraryAttributes as $attribute) {
            $attribute->initialise();
        }
    }

    /**
     * Load object from its main source.
     * @param mixed $language
     * @return boolean success
     */
    abstract public function load(mixed $language=true): bool;

    /**
     * Save object to its main source.
     * @return boolean success
     */
    abstract public function save(): bool;

    /**
     * Delete object source.
     * @return boolean success
     */
    abstract public function delete(): bool;

    /* Mark: Values related methods */

    /**
     * Return values as array.
     * @return array
     */
    public function toArray(): array {
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
    public function load_fromArray(array $array=array(), bool $update=false): void {
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
    public function load_fromHeader(string $prefix='', bool $update=false): void {
        foreach ($this->libraryAttributes as $attribute) {
            $attribute->load_fromHeader($prefix, $update);
        }
    }

    /**
     * Load values from session.
     * @param string $name
     * @return void
     */
    public function load_fromSession(string $name): void {
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
    public function save_toSession(string $name): void {
        global $CaMykS;
        $CaMykS->set_sessionValue($name, $this->toArray());
    }

    /* Mark: Override locales related methods */

    /**
     * Check if locale exists.
     * @param string $locale
     * @return boolean result
     */
    public function check_locale(string $locale=''): bool {
        return $this->libraryParent->check_locale($locale);
    }

    /**
     * Return locale.
     * @param string $locale
     * @param mixed $data
     * @return string
     */
    public function get_locale(string $locale='', mixed $data=null): string {
        return $this->libraryParent->get_locale($locale, $data);
    }

    /**
     * Load library locales.
     * @param string $folder
     * @param string $language
     * @return boolean success
     */
    protected function load_locales(string $folder='', ?string $language=null): bool {
        return $this->libraryParent->load_locales($folder, $language);
    }

    /* Mark: Attributes related methods */

    /**
     * Add attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean result
     */
    protected function define_attribute(string $name, string $type, mixed $default, array $params=array(), bool $isLocalised=false): bool {
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
    protected function check_attributesValues(): bool {
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

    /* Mark: Action errors & messages methods */

    /**
     * Add action message.
     * @param string $message
     * @return void
     */
    protected function add_actionMessage(string $message): void {
        $this->actionMessages[] = $message;
    }

    /**
     * Return action messages.
     * @return array
     */
    public function get_actionMessages(): array {
        return $this->actionMessages;
    }

    /**
     * Add action error.
     * @param Error $error
     * @return void
     */
    protected function add_actionError(CaMykS2\Error $error): void {
        $this->actionErrors[] = $error;
    }

    /**
     * Return action error.
     * @return array
     */
    public function get_actionErrors(): array {
        return $this->actionErrors;
    }

    /* Mark: Parent related methods */

    /**
     * Return configuration value.
     * @param string $name
     * @param string $language
     * @param mixed $default
     * @return mixed
     */
    public function get_configValue(string $name, ?string $language=null, mixed $default=null): mixed {
        return $this->libraryParent->get_configValue($name, $language, $default);
    }
}
?>
