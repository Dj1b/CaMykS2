<?php
/**
 * @brief PluginConfiguration object.
 * <br />Abstract library to create plugin configuration objects.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Jan 2021
 * @date Modification: Feb 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * PluginConfig class.
 */
abstract class PluginConfiguration extends PluginLibrary {
    /**
     * @var string $objectName
     * @brief Object name.
     */
    protected $objectName;

    /**
     * Class constructor.
     * @param Object $parent
     * @param string $name
     * @return void
     */
    public function __construct(&$parent, $name='config') {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define library variables */
        $this->objectName = $name;
    }

    /* Data management methods */

    /**
     * Load object from its main source.
     * @param mixed $language
     * @return boolean success
     */
    public function load($language=true) {
        global $CaMykS;

        /* Load default values */
        $this->initialise();

        /* Load saved values */
        $values = $CaMykS->LocalStorage->get_configuration($this->libraryParent->libraryName, $this->libraryShortName, $this->objectName);
        if ($values === false)
            return false;

        /* Update values */
        $this->libraryValues = $this->update_fromArray($values);

        /* Return success */
        return true;
    }

    /**
     * Save object to its main source.
     * @return boolean success
     */
    public function save() {
        global $CaMykS;

        /* Save data in LocalStorage */
        return $CaMykS->LocalStorage->save_configuration($this->libraryParent->libraryName, $this->libraryShortName, $this->objectName, $this->toArray());
    }

    /**
     * Delete object source.
     * @return boolean success
     */
    public function delete() {
        global $camyks;

        /* Delete data from LocalStorage */
        return $CaMykS->LocalStorage->delete_configuration($this->libraryParent->libraryName, $this->libraryShortName, $this->objectName);
    }
}
?>
