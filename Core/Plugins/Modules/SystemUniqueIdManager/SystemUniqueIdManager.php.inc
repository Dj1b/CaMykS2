<?php
/**
 * @brief SystemUniqueIdManager admin module.
 * <br />Module to manage unique ids.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jan 2022
 * @date Modification: Feb 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemUniqueIdManager class.
 */
final class SystemUniqueIdManager extends AdminModule {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected $hasConfiguration = true;

    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected $libraries = array('Key');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* */

    /**
     * Return new unique id.
     * @param string $plugin
     * @param string $object
     * @param integer $objectId
     * @param array $params
     * @return mixed // unique ID on success, false on failure
     */
    public function get_newUniqueId($plugin, $object, $objectId, $params=array()) {
        /* Check for free key or initialise new one */
        if (count($params) == 0 or !$this->Key->load_freeItem())
            $this->Key->initialise();

        /* Update key attributes */
        $this->Key->Status = 1;
        $this->Key->Plugin = $plugin;
        $this->Key->Object = $object;
        $this->Key->ObjectId = $objectId;
        $this->Key->Params = $params;

        /* Save key */
        if ($this->Key->save())
            return $this->Key->UniqueId;

        /* Unique key not generated, return failure */
        return false;
    }
}
?>
