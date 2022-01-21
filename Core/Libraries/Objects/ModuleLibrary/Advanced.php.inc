<?php
/**
 * @brief AdvancedModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * AdvancedModuleLibrary class.
 */
class AdvancedModuleLibrary extends SimpleModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Advanced module library attributes */
        $this->define_databaseAttribute('UniqueId', 'UniqueId', '', array());
        $this->define_databaseAttribute('CreationUser', 'ObjectId', 0);
        $this->define_databaseAttribute('CreationDate', 'Date', true);
        $this->define_databaseAttribute('LastEditionUser', 'ObjectId', 0);
        $this->define_databaseAttribute('LastEditionDate', 'Date', true);
    }

    /* Override ModuleLibrary methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data($loadPreviousVersion=false) {
        global $CaMykS;

        /* Perform parent check data loading previous version */
        if (!parent::check_data(true))
            return false;

        /* Check advanced module library attributes */
        if ($this->objectId > 0) {
            /* Object is updated */
            $this->UniqueId = $this->previousVersion->UniqueId;
            $this->CreationUser = $this->previousVersion->CreationUser;
            $this->CreationDate = $this->previousVersion->CreationDate;

        } else {
            /* Object is created */
            /* $this->CreationUser = $CaMykS->currentSession->AdminCurrentUser; */
            $this->CreationDate = time();
        }
        /* $this->LastEditionUser = $CaMykS->currentSession->AdminCurrentUser; */
        $this->LastEditionDate = time();

        /* Return data is conform */
        return true;
    }

    /* Unique id related methods */

    /**
     * Load object from its unique id.
     * @param string $uniqueId
     * @param boolean $createNew
     * @return boolean success
     */
    public function load_fromUniqueId($uniqueId='', $createNew=false) {

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
        $query = new DbQuery(array());

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
