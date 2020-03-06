<?php
/**
 * @brief CaMykSManager special plugin.
 * @details Plugin / Plugin engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.4
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
     * @var Object $managerForm
     * @brief Manager form object.
     */
    protected $managerForm;

    /**
     * @var integer $passwordEncodingLength
     * @brief Password encoding used for manager master password.
     */
    private $passwordEncodingLength = 64;

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
        $CaMykS->set_configValue('Website_Name', $this->libraryName);
        $CaMykS->set_configValue('Website_Title', 'Website Manager');
        $CaMykS->set_configValue('Website_URL', dirname($this->get_homeLink()).'/');

        /* Load locales */
        $this->load_locales('', $CaMykS->currentLanguage);

        /* Apply title */
        $CaMykS->HTMLPage->set_pageInformation($this->get_locale('TemplateHeaderTitle'));

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

            case 'Login': /* Display login frame */
                $this->initialise_login();
                break;

            case 'Logout': /* Close connection */
                $this->initialise_logout();
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
            $this->message['ButtonTitle'] = $this->get_locale('Error_Button_Retry');
            $this->message['ButtonLink'] = $this->get_homeLink();
            return true;
        }

        /* Check password is not encoded */
        if (!preg_match('#^[0-9a-z]{8}\:[0-9a-z]{'.$this->passwordEncodingLength.'}$#', $password)) {
            /* Encode password with hash */
            $salt = substr(session_id(), 1, 8);
            $newPassword = $salt.':'.password_encode($password, $this->passwordEncodingLength, $salt);

            /* Save configuration */
            $config = new ConfigurationFile();

            if (!$config->save('Manager', 'Engine', array('Manager_Password'=>$newPassword))) {
                /* Display error message page */
                $this->action = 'DisplayMessage';
                $this->message['Title'] = $this->get_locale('Error_CantSavePassword_Title');
                $this->message['Content'] = $this->get_locale('Error_CantSavePassword_Content');
                $this->message['ButtonTitle'] = $this->get_locale('Error_Button_Retry');
                $this->message['ButtonLink'] = $this->get_homeLink();
                return true;
            }

            /* Display login */
            $this->action = 'Login';

            /* Return as success */
            return true;
        }

        /* No action required */
        return false;
    }

    /**
     * Check manager has a logged user.
     * @return void
     */
    private function check_hasLoggedUser() {
        global $CaMykS;

        /* Check user is logged */
        if ($CaMykS->get_sessionValue('ManagerConnection') == 1)
            return true;

        /* Check user is logging in */
        if (($p = http_getStringRequest('ManagerPassword')) != '') {

            /* Check password */
            if (password_check($p, $CaMykS->get_configValue('Manager_Password'))) {
                /* Set user to session */
                $CaMykS->set_sessionValue('ManagerConnection', 1);

                /* Redirect to main page */
                $CaMykS->HTMLPage->set_redirect($this->get_homeLink());

                /* Close $CaMykS */
                $CaMykS->close();
            }
        }

        /* Display login page */
        $this->action = 'Login';
    }

    /* Initialise methods */

    /**
     * Initialise plugin in main mode.
     * @return void
     */
    private function initialise_main() {
        global $CaMykS;

        /* Enable action bar buttons */
        $CaMykS->currentTemplate->enable_actionBar();

    }

    /**
     * Initialise plugin in login mode.
     * @return void
     */
    private function initialise_login() {
        /* Build login form */
        $this->managerForm = new HTMLForm(array('name'=>'LoginForm'));

        /* Update layout */
        $this->update_selectedLayout('Login');
    }

    /**
     * Initialise plugin in logout mode.
     * @return void
     */
    private function initialise_logout() {
        global $CaMykS;

        /* Remove user from session */
        $CaMykS->set_sessionValue('ManagerConnection', 0);

        /* Redirect to main page */
        $CaMykS->HTMLPage->set_redirect($this->get_homeLink());

        /* Close $CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in save manager configuration mode.
     * @return void
     */
    private function initialise_saveManagerConfiguration() {

    }

    /* Links methods */

    /**
     * Return manager home link.
     * @return string
     */
    public function get_homeLink() {
        return $this->get_actionLink();
    }

    /**
     * Return manager logout link.
     * @return string
     */
    public function get_logoutLink() {
        return $this->get_actionLink('Logout');
    }

    /**
     * Return manager action link.
     * @param string $action
     * @return string
     */
    public function get_actionLink($action='') {
        /* Create link base url */
        $link = client_getCurrentURL();

        /* Add action */
        if ($action !== '')
            $link .= '?ManagerAction='.$action;

        /* Return link */
        return $link;
    }
}
?>
