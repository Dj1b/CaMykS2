<?php
/**
 * @brief ModuleLibrary object.
 * <br />Abstract library to create module objects to manage data.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Jan 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ModuleLibrary class.
 */
abstract class ModuleLibrary extends PluginLibrary {
    /**
     * @var integer $objectId
     * @brief Object identifier.
     */
    protected $objectId;

    /**
     * @var array $attributesList
     * @brief List off all attributes names in their sources.
     */
    protected $attributesList = array(
        'db'    => array(),
        'dbl'   => array(),
        'file'  => array(),
        'filel' => array(),
        'temp'  => array(),
        'templ' => array(),
    );

    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define library variables */
        $this->objectId = $id;
    }

    /* Data management methods */

    /**
     * Load object.
     * @param mixed $language
     * @return boolean success
     */
    public function load($language=true) {
        if ($this->objectId === 0)
            return $this->initialise($language);
        return $this->load_fromDatabase($language) and $this->load_files($language) and $this->load_temporary();
    }

    /**
     * Save object.
     * @return boolean success
     */
    public function save() {

    }

    /**
     * Delete object.
     * @return boolean success
     */
    public function delete() {

    }

    /* Data management sub methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data($loadPreviousVersion=false) {
        /* Check attributes */
        if (!$this->check_attributesValues())
            return false;

        if ($loadPreviousVersion) {
            /* Load previous version */
            $this->previousVersion = $this->get_newInstance($this->objectId);
            $this->previousVersion->load(true);
        }

        /* Return data is conform */
        return true;
    }

    /* Attributes related methods */

    /**
     * Define database attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_databaseAttribute($name, $type, $default=null, $params=array(), $isLocalised=false) {
        /* Build attribute type */
        $attributeType = $type.'PluginLibraryAttribute';

        /* Check attribute class */
        if (!class_exists($attributeType))
            $this->add_error(1, '', null, null, true);

        /* Load attribute object */
        $this->libraryAttributes[$name] = new $attributeType($this, $name, $default, $params, $isLocalised);

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['dbl'][] = $name;
        else
            $this->attributesList['db'][] = $name;
    }

    /**
     * Define temporary attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_temporaryAttribute($name, $type, $default=null, $params=array(), $isLocalised=false) {
        /* Build attribute type */
        $attributeType = $type.'PluginLibraryAttribute';

        /* Check attribute class */
        if (!class_exists($attributeType))
            $this->add_error(1, '', null, null, true);

        /* Load attribute object */
        $this->libraryAttributes[$name] = new $attributeType($this, $name, $default, $params, $isLocalised);

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['templ'][] = $name;
        else
            $this->attributesList['temp'][] = $name;
    }

    /**
     * Define database attribute.
     * @param string $name
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_fileAttribute($name, $params=array(), $isLocalised=false) {
        /* Load attribute object */
        $this->libraryAttributes[$name] = new FilePluginLibraryAttribute($this, $name, null, $params, $isLocalised);

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['filel'][] = $name;
        else
            $this->attributesList['file'][] = $name;
    }

    /* Multi object management methods */

}
?>
