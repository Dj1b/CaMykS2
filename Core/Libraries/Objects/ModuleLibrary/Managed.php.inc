<?php
/**
 * @brief ManagedModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Jan 2022
 * @date Modification: Oct 2026
 * @copyright 2022 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ManagedModuleLibrary class.
 */
class ManagedModuleLibrary extends SimpleModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Managed module library attributes */
        $this->define_databaseAttribute('CreationUser',     'ObjectId', 0);
        $this->define_databaseAttribute('CreationDate',     'Date',     true);
        $this->define_databaseAttribute('LastEditionUser',  'ObjectId', 0);
        $this->define_databaseAttribute('LastEditionDate',  'Date',     true);
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

        /* Check managed module library attributes */
        if ($this->objectId > 0) {
            /* Object is updated */
            $this->CreationUser = $this->previousVersion->CreationUser;
            $this->CreationDate = $this->previousVersion->CreationDate;

        } else {
            /* Object is created */
            /* $this->CreationUser = $CaMykS->currentSession->AdminCurrentUser->id; */
            $this->CreationDate = time();
        }
        /* $this->LastEditionUser = $CaMykS->currentSession->AdminCurrentUser->id; */
        $this->LastEditionDate = time();

        /* Return data is conform */
        return true;
    }
}
?>
