<?php
/**
 * @brief ManagedIdentifiableModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Oct 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ManagedIdentifiableModuleLibrary class.
 */
class ManagedIdentifiableModuleLibrary extends ManagedModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Managed identifiable module library attributes */
        $this->define_databaseAttribute('UniqueId', 'UniqueId', '', array());
    }

    /* Mark: Override ModuleLibrary methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        global $CaMykS;

        /* Perform parent check data loading previous version */
        if (!parent::check_data(true))
            return false;

        /* Check managed identifiable module library attributes */
        if ($this->objectId > 0) {
            /* Object is updated */
            $this->UniqueId = $this->previousVersion->UniqueId;

        } else {
            /* Object is created */
            /* $this->UniqueId = $this->get_newUniqueId(); */
        }

        /* Return data is conform */
        return true;
    }

    /* Mark: Unique id related methods */

    /**
     * Load object from its unique id.
     * @param string $uniqueId
     * @param boolean $createNew
     * @return boolean success
     */
    public function load_fromUniqueId(string $uniqueId='', bool $createNew=false): bool {

        /* Check empty unique ID value */
        if ($uniqueId == '') {
            if ($createNew) {
                $this->initialise();
                return true;
            }
            return false;
        }

        /* Check unique id value */
        $this->UniqueId = $uniqueId;
        if (!$this->libraryAttributes['UniqueId']->check_value())
            return false;

        /* Build query to load object */
        $query = new DatabaseQuery(array());

        /* Execute query */
        if (!$this->execute_DBBuiltQuery($query))
            return false;

        /* Check has data */
        if (!($data = $this->get_DBQueryDataLine()))
            return false;

        /* Load data */
        $this->objectId = $data['id'];
        $this->load_fromArray($data);

        /* Return success */
        return true;
    }
}
?>
