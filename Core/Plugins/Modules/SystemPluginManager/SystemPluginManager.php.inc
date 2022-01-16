<?php
/**
 * @brief SystemPluginManager admin module.
 * <br />Module to manage installed plugins.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: May 2021
 * @date Modification: Jan 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemPluginManager class.
 */
final class SystemPluginManager extends AdminModule {
    /**
     * @var array $libraries
     * @brief List of module internal libraries name.
     */
    protected $libraries = array('Plugin');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Plugin methods */

     /**
     * Install plugin.
     * @return boolean success
     */
    public function install() {
        /* Perform plugin installation */
        if (!parent::install())
            return false;

        /* Add its own information in plugin list data */
        $this->Plugin->load_fromArray(array(
            'Status'    => 1,
            'Name'      => $this->libraryName,
            'Type'      => $this->pluginType,
            'Location'  => 'System',
            'Version'   => $this->pluginVersion,
        ));
        if (!$this->Plugin->save())
            return false;

        /* Return success */
        return true;
    }

    /* Admin mode methods */

}
?>
