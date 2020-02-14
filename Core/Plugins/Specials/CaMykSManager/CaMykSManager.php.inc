<?php
/**
 * @brief CaMykSManager special plugin.
 * @details Plugin / Plugin engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Dec 2019
 * @date Modification: Feb 2020
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * CaMykSManager class.
 */
final class CaMykSManager extends Plugin {

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Special';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /**
     * Initialise plugin (in manager mode).
     * @return void
     */
    public function initialise() {
        /* Check website is installed in standalone mode */
        $this->check_isStandaloneMode();

        /* Check manager has password defined */
        $this->check_hasPasswordDefined();

        /* Check manager has a logged user */
        $this->check_hasLoggedUser();

        /* Load action to execute */
        $action = http_getStringRequest('ManagerAction', 'main');

        /* Initialise action to execute */
        switch ($action) {

            /* main, default */
            case 'main':
            default:
                $this->initialise_main();
        }
    }

    /**
     * Display plugin (in manager mode).
     * @return void
     */
    public function display() {

    }

    /* Pre-check methods */

    /**
     * Check website is installed in standalone mode.
     * @return void
     */
    private function check_isStandaloneMode() {
        /* Read website configuration */

        /* Check is standalone mode */

        /* Redirect visitor to website */

    }

    /**
     * Check manager has password defined.
     * @return void
     */
    private function check_hasPasswordDefined() {
        /* Read password from manager configuration */

        /* Check password is empty */
        if (false) {
            /* Display message page */

        }

        /* Check password is not encoded */
        if (false) {
            /* Encode password with hash */

            /* Save configuration */

            /* Display message page */

            /* Redirect to manager page */
        }
    }

    /**
     * Check manager has a logged user.
     * @return void
     */
    private function check_hasLoggedUser() {
        /* Check user logged */
        if (false)
            return;

        /* Check user is logging in */
        if (false) {

        }

        /* Display login page */
        if (false) {

        }
    }

    /* Initialise methods */

    /**
     * Initialise plugin in main mode.
     * @return void
     */
    private function initialise_main() {

    }

    /**
     * Initialise plugin in save manager configuration mode.
     * @return void
     */
    private function initialise_saveManagerConfiguration() {

    }
}
?>
