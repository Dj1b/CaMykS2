<?php
/**
 * @brief CaMykSManager special plugin.
 * @details Plugin / Plugin engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Dec 2019
 * @date Modification: Mar 2020
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
     * @var string $action
     * @brief Current action to execute in manager.
     */
    protected $action = 'main';

    /**
     * @var Object $theme
     * @brief Link to current CaMykS theme.
     */
    protected $theme;

    /**
     * @var array $message
     * @brief Message to display in message mode.
     */
    protected $message = array(
        'Title'         => '',
        'Content'       => '',
        'ButtonLink'    => '',
        'ButtonTitle'   => '',
    );

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
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        global $CaMykS;
        parent::initialise($params);

        /* Override some website configuration variables */
        $CaMykS->set_configValue('Website_Title', 'Website Manager');
        $CaMykS->set_configValue('Website_URL', dirname(client_getCurrentURL()).'/');

        /* Load locales */
        $this->load_locales('', $CaMykS->currentLanguage);

        /* Load theme */
        $this->theme = &$CaMykS->currentTheme;

        /* Check website is installed in standalone mode */
        $this->check_isStandaloneMode();

        /* Load action to execute */
        $this->action = http_getStringRequest('ManagerAction', 'Main');

        /* Check manager has password defined */
        if ($this->check_hasPasswordDefined()) {

        /* Check manager has a logged user */
        } elseif ($this->check_hasLoggedUser()) {

        }

        /* Initialise action to execute */
        switch ($this->action) {
            case 'DisplayMessage': /* Display message */
                $this->update_selectedLayout('Message');
                break;

            case 'Main': /* Main, default */
            default:
                $this->initialise_main();
        }
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
        global $CaMykS;

        /* Read password from manager configuration */
        $password = $CaMykS->get_configValue('Manager_Password', '');

        /* Check password is empty */
        if ($password === '') {
            /* Display message page */
            $this->action = 'DisplayMessage';
            $this->message['Title'] = $this->get_locale('Error_NoPasswordDefined_Title');
            $this->message['Content'] = $this->get_locale('Error_NoPasswordDefined_Content');
            $this->message['ButtonTitle'] = $this->get_locale('Error_NoPasswordDefined_ButtonTitle');
            $this->message['ButtonLink'] = '#';
            return true;
        }

        /* Check password is not encoded */
        if (!preg_match('#^[0-9a-z]{8}\:[0-9a-z]{64}$#', $password)) {
            /* Encode password with hash */
            $salt = substr(session_id(), 1, 8);
            $newPassword = $salt.':'.password_encode($password, 64, $salt);

            /* Save configuration */

            /* Display login */
            $this->action = 'Login';

            /* Return as success */
            return true;
        }
        return false;
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
