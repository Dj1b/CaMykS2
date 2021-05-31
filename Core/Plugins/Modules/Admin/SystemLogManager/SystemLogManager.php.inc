<?php
/**
 * @brief SystemLogManager admin module.
 * <br />Module to manage logs and errors.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1.1
 * @date Creation: Jan 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemLogManager class.
 */
final class SystemLogManager extends AdminModule {

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Module methods */

    /**
     * Load module.
     * @return void
     */
    protected function load() {
        /* Check is already loaded */
        if ($this->moduleLoaded)
            return;

        /* Load libraries */

        /* Create internal object */

        /* Mark module as loaded */
        $this->moduleLoaded = true;
    }

    /* Public methods */

    /**
     * Log single message.
     * @param string $object
     * @param string $action
     * @param string $message
     * @return boolean success
     */
    public function log_message($object, $action, $message) {

    }

    /**
     * Log multiple messages.
     * @param string $object
     * @param array $messages
     * @return boolean success
     */
    public function log_messages($object, $messages) {

    }

    /**
     * Log error.
     * @param Object $error
     * @return boolean success
     */
    public function log_error($error) {

    }
}
?>
