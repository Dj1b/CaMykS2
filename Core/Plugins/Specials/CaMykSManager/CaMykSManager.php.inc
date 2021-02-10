<?php
/**
 * @brief CaMykSManager special plugin.
 * @details Plugin / Plugin engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre8
 * @date Creation: Dec 2019
 * @date Modification: Feb 2021
 * @copyright 2019 - 2021 CaMykS Team
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
     * @var array $configurationSections
     * @brief List sections available to configure.
     */
    protected $configurationSections = array('Manager', 'Engine', 'Visit', 'Admin', 'Database', 'GDPR', 'Plugins'/*, 'Request'*/);

    /**
     * @var string $section
     * @brief Current section to configure.
     */
    protected $section;

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

        /* Check manager is in a website installed in standalone mode */
        if (!$this->check_isStandaloneMode())
            return false;

        /* Check manager security */
        if (!$this->check_security())
            return false;

        /* Load action to execute */
        $this->action = http_getStringRequest('ManagerAction', 'SiteList');

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
                $this->initialise_Login();
                break;

            case 'Logout': /* Close connection */
                $this->initialise_Logout();
                break;

            case 'OpenSiteVisit':
                $this->initialise_OpenSiteVisit();
                break;

            case 'OpenSiteAdmin':
                $this->initialise_OpenSiteAdmin();
                break;

            case 'DisplayConfigurationPanel':
                $this->initialise_DisplayConfigurationPanel();
                break;

            case 'DisplayInformationPanel':
                $this->initialise_DisplayInformationPanel();
                break;

            case 'ResetOpcodeCache':
                $this->initialise_ResetOpcodeCache();
                break;

            case 'SiteList': /* Main, default */
            default:
                $this->initialise_SiteList();
        }
    }

    /* Pre-check methods */

    /**
     * Check manager security.
     * @return boolean result
     */
    private function check_security() {
        global $CaMykS;

        /* Check restrained IPs */
        $ips = $CaMykS->get_configValue('Manager_RestrainedIPs');
        if (is_array($ips) and count($ips) > 0) {
            if (!in_array(client_getIp(), $ips))
                return false;
        }

        /* By default, return OK */
        return true;
    }

    /**
     * Check manager is in a website installed in standalone mode.
     * @return boolean result
     */
    private function check_isStandaloneMode() {
        /* Read website configuration */

        /* Check is standalone mode */

        /* Redirect visitor to website */

        return true;
    }

    /**
     * Check manager has password defined.
     * @return boolean result
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
            $this->message['ButtonTitle'] = $this->get_locale('Action_Retry');
            $this->message['ButtonLink'] = $this->get_homeLink();
            return true;
        }

        /* Check password is not encoded */
        if (!preg_match('#^[0-9a-z]{8}\:[0-9a-z]{'.$this->passwordEncodingLength.'}$#', $password)) {
            /* Encode password with hash */
            $salt = substr(session_id(), 1, 8);
            $data = array('Manager_Password'=> $salt.':'.password_encode($password, $this->passwordEncodingLength, $salt));

            /* Check for document root setting */
            if ($CaMykS->get_configValue('Manager_SiteListPath') == '')
                $data['Manager_SiteListPath'] = server_getDocumentRoot().'/';

            /* Save configuration */
            $config = new ConfigurationFile();
            if (!$config->save('Manager', 'Engine', $data)) {
                /* Display error message page */
                $this->action = 'DisplayMessage';
                $this->message['Title'] = $this->get_locale('Error_CantSavePassword_Title');
                $this->message['Content'] = $this->get_locale('Error_CantSavePassword_Content');
                $this->message['ButtonTitle'] = $this->get_locale('Action_Retry');
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

                /* Close CaMykS */
                $CaMykS->close();
            }
        }

        /* Display login page */
        $this->action = 'Login';
    }

    /* Initialise methods */

    /**
     * Initialise plugin in Login mode.
     * @return void
     */
    private function initialise_Login() {
        /* Build login form */
        $this->managerForm = new HTMLForm(array('Name'=>'LoginForm'));

        /* Update layout */
        $this->update_selectedLayout('Login');
    }

    /**
     * Initialise plugin in Logout mode.
     * @return void
     */
    private function initialise_Logout() {
        global $CaMykS;

        /* Remove user from session */
        $CaMykS->set_sessionValue('ManagerConnection', 0);

        /* Redirect to main page */
        $CaMykS->HTMLPage->set_redirect($this->get_homeLink());

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in SiteList mode.
     * @return void
     */
    private function initialise_SiteList() {
        global $CaMykS;

        /* Enable action bar buttons */
        $CaMykS->currentTemplate->enable_actionBar();

        /* Load folder tree */
        $this->folderTree = array(
            'Files'     =>array(),
            'Folders'   => array('/' => array_merge(
                array('Path' => '/', 'Files' => array(), 'Folders' => array(), 'Type' => 'Folder', 'Id' => 'Root', 'Class' => 'opened'),
                $this->load_folderTree('', 1)
            )),
        );

        /* Build external link HTML form */
        $this->externalLinksForm = new HTMLForm(array('Name'=>'ExternalLinks', 'Target'=>'_blank', 'Action'=>$this->get_homeLink()));
        $this->externalLinksForm->add_hidden('ManagerAction', '');
        $this->externalLinksForm->add_hidden('Folder', '');

        /* Load plugin scripts */
        $this->add_JSFile('SiteList.js', array('async'=>''));
        $this->add_JSScript('SiteList.set_param("form", "'.$this->externalLinksForm->get_name().'");', false, 'Load');
        $this->add_JSScript('SiteList.initialise();', true, 'Load');

        /* Update layout */
        $this->update_selectedLayout('SiteList');
    }

    /**
     * Initialise plugin in OpenSiteVisit mode.
     * @param boolean $adminMode
     * @return void
     */
    private function initialise_OpenSiteVisit($adminMode=false) {
        global $CaMykS;

        /* Load folder */
        $folder = http_getStringRequest('Folder', 'null');

        /* Check folder */
        if (file_exists($CaMykS->get_configValue('Manager_SiteListPath').$folder)) {

            /* Load folder type */
            $type = $this->detect_folderType($folder);

            /* Check folder type */
            switch ($type) {
                case 'CaMykSv1Site':
                    /* Load website configuration */
                    include $CaMykS->get_configValue('Manager_SiteListPath').$folder.'/etc/site.php.inc';

                    /* Check configuration */
                    if (isset($conf) and isset($conf['url'])) {
                        $url = preg_replace('#/$#', '', $conf['url']);

                        /* Check admin mode */
                        if ($adminMode)
                            $url .= '/'.(isset($conf['admin_entry']) ? $conf['admin_entry'] : 'admin.php');

                        /* Redirect to main page */
                        $CaMykS->HTMLPage->set_redirect($url);

                        /* Close CaMykS */
                        $CaMykS->close();

                        /* End action */
                        return;
                    }
                    break;
                case 'CaMykSv2Site':
                    /* Load website configuration */

                    /* Check website configuration */

                    /* End action */
                    return;
            }
        }

        /* Folder is not valid or not a CaMykS website, return error */
        $this->action = 'DisplayMessage';
        $this->message['Title'] = $this->get_locale('Error_CantVisitNonCaMykSWebsites_Title');
        $this->message['Content'] = $this->get_locale('Error_CantVisitNonCaMykSWebsites_Content');
        $this->message['ButtonTitle'] = $this->get_locale('Action_CloseWindow');
        $this->message['ButtonLink'] = 'javascript:window.close();';

        /* Update layout */
        $this->update_selectedLayout('Message');

        /* End action  */
        return;
    }

    /**
     * Initialise plugin in OpenSiteAdmin mode.
     * @return void
     */
    private function initialise_OpenSiteAdmin() {
        return $this->initialise_OpenSiteVisit(true);
    }

    /**
     * Initialise plugin in DisplayInformationPanel mode.
     * @return void
     */
    private function initialise_DisplayInformationPanel() {
        global $CaMykS;

        /* Enable action bar buttons */
        $CaMykS->currentTemplate->enable_actionBar();

        /* Load available database drivers */
        $this->dbDrivers = DatabaseConnector::get_availableDriverList();

        /* Update layout */
        $this->update_selectedLayout('InformationPanel');

        /* End action  */
        return;
    }

    /**
     * Initialise plugin in DisplayConfigurationPanel mode.
     * @return void
     */
    private function initialise_DisplayConfigurationPanel() {
        global $CaMykS;

        /* Enable action bar buttons */
        $CaMykS->currentTemplate->enable_actionBar();

        /* Load section to configure */
        if (isset($_REQUEST['Section']) and in_array($_REQUEST['Section'], $this->configurationSections)) {
            $this->section = $_REQUEST['Section'];

            /* Load configuration */

            /* Build login form */
            $this->managerForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveConfiguration')));

        } else
            $this->section ='None';

        /* Update layout */
        $this->update_selectedLayout('ConfigurationPanel');

        /* End action  */
        return;
    }

    /**
     * Initialise plugin in ResetOpcodeCache mode.
     * @return void
     */
    private function initialise_ResetOpcodeCache() {
        global $CaMykS;

        /* Try to delete OpCode cache */
        if (opcache_reset())
            $CaMykS->set_sessionValue('AdminInterface-Message', $this->get_locale('ResetOpcodeCache_Success'));
        else
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('ResetOpcodeCache_Error'));

        /* Redirect to main page */
        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayInformationPanel'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in save configuration section mode.
     * @return void
     */
    private function initialise_SaveConfigurationSection() {

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
     * @param array $params
     * @return string
     */
    public function get_actionLink($action='', $params=array()) {
        /* Create link base url */
        $link = client_getCurrentURL();

        /* Add action */
        if ($action !== '') {
            $link .= '?ManagerAction='.$action;

            /* Add params */
            if (is_array($params) and count($params) > 0) {
                foreach ($params as $name => $value)
                    $link .= '&'.$name.'='.$value;
            }
        }

        /* Return link */
        return $link;
    }

    /* Site list methods */

    /**
     * Load folder tree for given path and level.
     * @param string $path
     * @param integer $level
     * @return array
     */
    private function load_folderTree($path, $level) {
        global $CaMykS;

        $content = folder_listElements($CaMykS->get_configValue('Manager_SiteListPath').$path, array(
            'MaxDepth'              => 1,
            'ExcludeHiddenFolders'  => $CaMykS->get_configValue('Manager_SiteListHideHiddenFolders'),
            'ExcludeHiddenFiles'    => $CaMykS->get_configValue('Manager_SiteListHideHiddenFiles'),
        ));

        foreach ($content['Files'] as $name => &$file)
            $file['Path'] = $path.'/'.$name;

        foreach ($content['Folders'] as $name => &$folder) {
            $folder['Path'] = $path.'/'.$name;
            $folder['Type'] = $this->detect_folderType($folder['Path']);
            $folder['Id'] = preg_replace('#([^a-zA-Z0-9\-_]){1}#', '_', $folder['Path']);
            $folder['Class'] = '';
            if ($folder['Type'] == 'Folder' and $level < $CaMykS->get_configValue('Manager_SiteListSearchLevel'))
                $folder = array_merge($folder, $this->load_folderTree($folder['Path'], $level+1), array('Path'=>$path.'/'.$name));
        }

        return $content;
    }

    /**
     * Detect folder type.
     * @param string $path
     * @return string
     */
    private function detect_folderType($path) {
        global $CaMykS;

        $path = $CaMykS->get_configValue('Manager_SiteListPath').$path;

        /* Check CaMykS v2 site */
        if (file_exists($path.'/Configuration/Website.php.inc'))
            return 'CaMykSv2Site';

        /* Check CaMykS v1 site */
        if (file_exists($path.'/etc/site.php.inc'))
            return 'CaMykSv1Site';

        /* Check CaMykS v2 engine */
        if (file_exists($path.'/CaMykS.php.inc') and file_exists($path.'/Core'))
            return 'CaMykSv2Engine';

        /* Check CaMykS v1 engine */
        if (file_exists($path.'/Camyks.php.inc') and file_exists($path.'/engine'))
            return 'CaMykSv1Engine';

        /* Check other website */
        if (file_exists($path.'/index.php') or file_exists($path.'/index.htm') or file_exists($path.'/index.html'))
            return 'NoCaMykSSite';

        /* By default return Folder */
        return 'Folder';
    }

    /**
     * Display folder content.
     * @param array $folder
     * @param integer $level
     * @return void
     */
    protected function display_folderContent($folder, $level) {
        $this->currentFolder = $folder;
        $this->currentLevel = $level;
        $this->load_layout('SiteList/FolderContent');
    }

    /**
     * Return file description.
     * @param string $filePath
     * @return string
     */
    protected function get_fileDescription($filePath) {
        global $CaMykS;

        $file = new File($CaMykS->get_configValue('Manager_SiteListPath').$filePath);
        if (!$file->is_accessible())
            return '';

        return $file->get_description(array('AddSize'=>true));
    }
}
?>
