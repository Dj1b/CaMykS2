<?php
/**
 * @brief CaMykSManager special plugin.
 * @details Plugin / Plugin engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre17
 * @date Creation: Dec 2019
 * @date Modification: Mar 2023
 * @copyright 2019 - 2023 CaMykS Team
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

    /* Configuration variables */

    /**
     * @var array $configurationSections
     * @brief List sections available to configure.
     */
    protected $configurationSections = array('Manager', 'Engine', 'Visit', 'Admin', 'Database', 'Plugins'/*, 'Request'*/);

    /**
     * @var string $section
     * @brief Current section to configure.
     */
    protected $section;

    /**
     * @var string $subAction
     * @brief Action to execute on configure.
     */
    protected $subAction;

    /**
     * @var array $panelButtons
     * @brief List of interface panel buttons.
     */
    protected $panelButtons;

    /**
     * @var array $folderTree
     * @brief Web folder tree.
     */
    protected $folderTree;

    /**
     * @var string $currentFolder
     * @brief Current folder value.
     */
    protected $currentFolder;

    /**
     * @var string $currentLevel
     * @brief Current level value.
     */
    protected $currentLevel;

    /**
     * @var object $template
     * @brief Link to template object.
     */
    protected $template;

    /**
     * @var object $mediaPopupComponent
     * @brief Link to MediaPopup component.
     */
    private $mediaPopupComponent;

    /**
     * @var object $externalLinksForm
     * @brief External links form.
     */
    protected $externalLinksForm;

    /**
     * @var object $newFolderForm
     * @brief New folder form.
     */
    protected $newFolderForm;

    /**
     * @var object $newC2SiteForm
     * @brief New CaMykS 2 site form.
     */
    protected $newC2SiteForm;

    /**
     * @var object $configurationManager
     * @brief Link to configuration manager.
     */
    protected $configurationManager;

    /**
     * @var array $dbDrivers
     * @brief List of database drivers.
     */
    protected $dbDrivers;

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
        $CaMykS->set_configValue('Admin_HeaderLogoFile', $this->get_fileURL('Images/Header/Banner.svg'));

        /* Load locales */
        $this->load_locales();

        /* Apply title */
        $CaMykS->HTMLPage->set_pageInformation($this->get_locale('TemplateHeaderTitle'));

        /* Load theme */
        $this->theme = &$CaMykS->currentTheme;

        /* Define body class */
        $CaMykS->HTMLPage->set_bodyClass('manager');

        /* Load template */
        $this->template = &$CaMykS->currentTemplate;

        /* Check manager is in a website installed in standalone mode */
        if (!$this->check_isStandaloneMode())
            return false;

        /* Check manager security */
        if (!$this->check_security())
            return false;

        /* Load action to execute */
        $this->action = http_getStringRequest('ManagerAction', 'Home');

        /* Check manager has password defined */
        if ($this->check_hasPasswordDefined()) {

        /* Check manager has a logged user */
        } elseif ($this->check_hasLoggedUser()) {

            /* Add buttons */
            $this->template->add_actionBarButton(array('Title'=>$this->get_locale('Action_SiteList'), 'Link'=>$this->get_actionLink('SiteList'), 'Icon'=>$this->get_fileURL('Images/Home/SiteList.svg')));
            $this->template->add_actionBarButton(array('Title'=>$this->get_locale('Action_ConfigureCaMykS2'), 'Link'=>$this->get_actionLink('DisplayConfigurationPanel'), 'Icon'=>$this->get_fileURL('Images/Home/Settings.svg')));
            $this->template->add_actionBarButton(array('Title'=>$this->get_locale('Action_CaMykS2Information'), 'Link'=>$this->get_actionLink('DisplayInformationPanel'), 'Icon'=>$this->get_fileURL('Images/Home/Server.svg')));

            /* Load Media Popup component */
            $this->mediaPopupComponent = $CaMykS->load_component('MediaPopup');
            if ($this->mediaPopupComponent !== false)
                $this->mediaPopupComponent->plug(array('Control_ClickBkgdToClose'=>0));
        }

        /* Initialise action to execute */
        switch ($this->action) {
            case 'DisplayMessage': /* Display message */
                $this->update_selectedLayout('Message');
                break;

            case 'Login': /* Display login frame */
                $this->initialise_Login();
                break;

            case 'Logout': /* Close session */
                $this->initialise_Logout();
                break;

            case 'SiteList':
                $this->initialise_SiteList();
                break;

            case 'InsertFolder':
                $this->initialise_InsertFolder();
                break;

            case 'InsertCaMykS2Site':
                $this->initialise_InsertCaMykS2Site();
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

            case 'EditConfigurationPanel':
                $this->initialise_EditConfigurationPanel();
                break;

            case 'DisplayInformationPanel':
                $this->initialise_DisplayInformationPanel();
                break;

            case 'ResetOpcodeCache':
                $this->initialise_ResetOpcodeCache();
                break;

            case 'Home': /* Main, default */
            default:
                $this->initialise_Home();
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
            /* Load configuration file */
            $config = new ConfigurationFile('Manager', 'Engine');
            $config->load();


            /* Encode password with hash */
            $salt = substr(session_id(), 1, 8);
            $config->define('Manager_Password', $salt.':'.password_encode($password, $this->passwordEncodingLength, $salt));

            /* Check for document root setting */
            if ($CaMykS->get_configValue('Manager_SiteListPath') == '')
                $config->define('Manager_SiteListPath', server_getDocumentRoot().'/');

            /* Save configuration */
            if (!$config->save()) {
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
     * Initialise plugin in Home mode.
     * @return void
     */
    private function initialise_Home() {
        global $CaMykS;

        /* Update background color */
        $CaMykS->HTMLPage->set_bodyClass('manager colored');

        /* Build button list */
        $this->panelButtons = array(
            array('Title'=>$this->get_locale('Action_SiteList'), 'Link'=>$this->get_actionLink('SiteList'), 'Icon'=>$this->get_fileURL('Images/Home/SiteList.svg')),
            array('Title'=>$this->get_locale('Action_ConfigureCaMykS2'), 'Link'=>$this->get_actionLink('DisplayConfigurationPanel'), 'Icon'=>$this->get_fileURL('Images/Home/Settings.svg')),
            array('Title'=>$this->get_locale('Action_CaMykS2Information'), 'Link'=>$this->get_actionLink('DisplayInformationPanel'), 'Icon'=>$this->get_fileURL('Images/Home/Server.svg')),
        );

        /* Update layout */
        $this->update_selectedLayout('Home');
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
            'Files'     => array(),
            'Folders'   => array('/' => array_merge(
                array('Path' => '/', 'Files' => array(), 'Folders' => array(), 'Type' => 'Folder', 'Id' => 'Root', 'Class' => 'opened'),
                $this->load_folderTree('', 1)
            )),
        );

        /* Build external link HTML form */
        $this->externalLinksForm = new HTMLForm(array('Name'=>'ExternalLinks', 'Target'=>'_blank', 'Action'=>$this->get_homeLink()));
        $this->externalLinksForm->add_hidden('ManagerAction', '');
        $this->externalLinksForm->add_hidden('Folder', '');

        /* Build new folder HTML form */
        $this->newFolderForm = new HTMLForm(array('Name'=>'NewFolderForm', 'Action'=>$this->get_actionLink('InsertFolder')));
        $this->newFolderForm->add_hidden('Path', '');
        $this->newFolderForm->add_hidden('FolderName', '');

        /* Build new CaMykS 2 site HTML form */
        $this->newC2SiteForm = new HTMLForm(array('Name'=>'NewC2SiteForm', 'Action'=>$this->get_actionLink('InsertCaMykS2Site')));
        $this->newC2SiteForm->add_hidden('Path', '');
        $this->newC2SiteForm->add_hidden('SiteName', '');

        /* Load plugin scripts */
        $this->add_JSFile('SiteList.js?v=1.0pre2', array('async'=>''));
        $this->add_JSFile('Locales/SiteList.'.$CaMykS->get_currentLanguageValue().'.js?v=1.0pre0', array('async'=>''));
        $this->add_JSScript('SiteList.set_param("form", "'.$this->externalLinksForm->get_name().'");', false, 'Load');
        $this->add_JSScript('SiteList.initialise();', true, 'Load');

        /* Update layout */
        $this->update_selectedLayout('SiteList');
    }

    /**
     * Initialise plugin in InsertFolder mode.
     * @return void
     */
    private function initialise_InsertFolder() {
        global $CaMykS;

        /* Load data from header */
        $path = http_getStringRequest('Path', 'null');
        $folderName = http_getStringRequest('FolderName', 'null');
        $folder = $path.'/'.$folderName;

        /* Check folder name */
        if (!preg_match('#^[A-Za-z][A-Za-z0-9_\-]*[A-Za-z0-9]$#', $folderName)) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_FolderNameIsNotValid'));
        }

        /* Check folder exists */
        elseif (file_exists($CaMykS->get_configValue('Manager_SiteListPath').$folder)) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_FolderAlreadyExists'));
        }

        /* Create folder */
        elseif (!folder_completePath($CaMykS->get_configValue('Manager_SiteListPath').$folder)) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_CantCreateFolder'));
        } else
            $CaMykS->set_sessionValue('AdminInterface-Message', $this->get_locale('SiteList_FolderCreated'));

        /* Redirect to SiteList interface */
        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('SiteList'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in InsertCaMykS2Site mode.
     * @return void
     */
    private function initialise_InsertCaMykS2Site() {
        global $CaMykS;

        /* Load data from header */
        $path = http_getStringRequest('Path', 'null');
        $siteName = http_getStringRequest('SiteName', 'null');
        $fullName = $path.'/'.$siteName;

        /* Check folder name */
        if (!preg_match('#^[A-Za-z][A-Za-z0-9_\-]*[A-Za-z0-9]$#', $siteName)) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_FolderNameIsNotValid'));
        }

        /* Check folder exists */
        elseif (file_exists($CaMykS->get_configValue('Manager_SiteListPath').$fullName)) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_FolderAlreadyExists'));
        }

        /* Duplicate skeleton site */
        elseif (!folder_copy($CaMykS->get_engineFileFullPath('Core/Skeleton'), $CaMykS->get_configValue('Manager_SiteListPath').$fullName)) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_CantCreateSite'));
        }

        else {
            /* Edit configuration file to set  site name and CaMykS path */
            $configuration = new ConfigurationFile($CaMykS->get_configValue('Manager_SiteListPath').$fullName.'/Configuration/Website'.CAMYKS_LIBRARYEXTENSION, 'FullPath');
            $configuration->load();
            $configuration->define('Website_Name', $siteName);
            $configuration->define('Website_CaMykSPath', $CaMykS->get_engineFileFullPath(''));

            /* Save configuration */
            if (!$configuration->save())
                $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('SiteList_Error_CantEditSiteConfiguration'));
            else
                $CaMykS->set_sessionValue('AdminInterface-Message', $this->get_locale('SiteList_SiteCreated'));
        }

        /* Redirect to SiteList interface */
        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('SiteList'));

        /* Close CaMykS */
        $CaMykS->close();
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

                    /* Check website configuration */
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
                    $configuration = new ConfigurationFile($CaMykS->get_configValue('Manager_SiteListPath').$folder.'/Configuration/Website.php.inc', 'FullPath');
                    $configuration->load();

                    /* Check website configuration */
                    $url = $configuration->value('Website_URL');
                    if (is_null($url) or $url === '') {
                        /* Create URL from root */
                        $url = url_getCurrentURL();
                        $url = preg_replace('#CaMykS2/Manager.php$#i', '', $url);
                        $url .= $folder;
                    }

                    /* Check admin mode */
                    if ($adminMode) {
                        /* Load admin entry point from website configuration */
                        $configuration = new ConfigurationFile($CaMykS->get_configValue('Manager_SiteListPath').$folder.'/Configuration/Admin.json.inc', 'FullPath');
                        $configuration->load();

                        $adminPage = $configuration->value('Admin_EntryPoint');
                        if (is_null($adminPage) or $adminPage === '') {
                            /* Load admin entry point from engine configuration */
                            $configuration = new ConfigurationFile('Admin', 'Engine');
                            $configuration->load();

                            $adminPage = $configuration->value('Admin_EntryPoint');
                            if (is_null($adminPage) or $adminPage === '') {
                                /* Load admin entry point from default value */
                                $adminPage = 'admin.php';
                            }
                        }

                        /* Update URL */
                        $url .= '/'.$adminPage;
                    }

                    /* Redirect to main page */
                    $CaMykS->HTMLPage->set_redirect($url);

                    /* Close CaMykS */
                    $CaMykS->close();

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
        $this->dbDrivers = DatabaseConnector::get_availableDrivers();

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

        /* Load configuration module */
        $this->configurationManager = $CaMykS->load_module('SystemConfigurationManager', 'System');
        if ($this->configurationManager === false) {
            /* Throw fatal error */

            die();
        }
        $this->configurationManager->initialise();
        $this->configurationManager->load_locales();

        /* Load section to configure */
        if (isset($_REQUEST['Section']) and in_array($_REQUEST['Section'], $this->configurationSections)) {
            $this->section = $_REQUEST['Section'];

            /* Load configuration */
            $CaMykS->load_configuration($this->section);

            /* Load specific data for sections */
            switch ($this->section) {
                case 'Engine':
                    /* Load error logging data */
                    $this->errorLoggingOutputs = $CaMykS->errorHandler->get_availableLoggingOutputs();
                    $this->errorLoggingLevels = $CaMykS->errorHandler->get_availableLoggingLevels();
                    break;
            }
        } else
            $this->section = 'None';

        /* Update layout */
        $this->subAction = 'View';
        $this->update_selectedLayout('ConfigurationPanel');

        /* End action  */
        return;
    }

    /**
     * Initialise plugin in EditConfigurationPanel mode.
     * @return void
     */
    private function initialise_EditConfigurationPanel() {
        global $CaMykS;

        /* Enable action bar buttons */
        $CaMykS->currentTemplate->enable_actionBar();

        /* Check section to configure */
        if (!isset($_REQUEST['Section']) or !in_array($_REQUEST['Section'], $this->configurationSections))
            return $this->initialise_DisplayConfigurationPanel();

        /* Load configuration module */
        $this->configurationManager = $CaMykS->load_module('SystemConfigurationManager', 'System');
        if ($this->configurationManager === false) {
            /* Throw fatal error */

            die();
        }
        $this->configurationManager->initialise();
        $this->configurationManager->load_locales();

        /* Load section */
        $this->section = $_REQUEST['Section'];

        /* Load configuration */
        $CaMykS->load_configuration($this->section);

        /* Build configuration form */
        $this->configurationForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveConfiguration')));
        $this->configurationForm->add_hidden('Section', $this->section);

        /* Load specific data for sections */
        switch ($this->section) {
            case 'Manager':
                /* Load available themes */
                $this->availableThemes = $this->configurationManager->get_availableAdminThemesTitles();

                /* Load available languages */
                $this->availableLanguages = language_getLocalisedLongTitles(array('ShortList'=>$this->get_availableLocaleLanguages()));
                break;
            case 'Engine':
                /* Load timezone library and data */
                $CaMykS->load_library('timezone', 'Tools');
                $this->timeZoneLocations = timezone_getLocationTitles(array('GroupByZone'=>true));

                /* Load error logging data */
                $this->errorLoggingOutputs = $CaMykS->errorHandler->get_availableLoggingOutputs();
                $this->errorLoggingLevels = $CaMykS->errorHandler->get_availableLoggingLevels();

                /* Load Image library and data */
                $CaMykS->load_library('Image', 'Objects');
                $this->imageEditorEngines = Image::get_availableEngines();
                break;
            case 'Admin':
                /* Load available themes */
                $this->availableThemes = $this->configurationManager->get_availableAdminThemesTitles();

                /* Load URL modes */
                $this->availableSSLModes = url_getSSLModes();

                /* Load available languages */
                $this->availableLanguages = language_getLocalisedLongTitles(array('ShortList'=>$CaMykS->get_availableEngineLocaleLanguages()));
                $this->availableDefaultLanguages = array_merge(array('' => $this->get_locale('Configuration_DefaultLanguageAutomatic')), $this->availableLanguages);

                break;
            case 'Visit':
                /* Load available themes */
                $this->availableThemes = $this->configurationManager->get_availableVisitThemesTitles();

                /* Load available templates */
                $this->availableTemplates = $this->configurationManager->get_availableVisitTemplatesTitles();

                /* Load URL modes */
                $this->availableURLModes = url_getBaseURLModes();
                $this->availableSSLModes = url_getSSLModes();

                /* Load available languages */
                $this->availableLanguages = language_getLocalisedLongTitles(array('ShortList'=>$CaMykS->get_availableEngineLocaleLanguages()));
                $this->availableDefaultLanguages = array_merge(array('' => $this->get_locale('Configuration_DefaultLanguageAutomatic')), $this->availableLanguages);

                break;
            case 'Database':
                /* Load available database drivers */
                $this->databaseDrivers = array_merge(array('Undefined' => $this->get_locale('Configuration_DriverUndefined')), DatabaseConnector::get_availableDrivers());
                break;
        }

        /* Update layout */
        $this->subAction = 'Edit';
        $this->update_selectedLayout('ConfigurationPanel');

        /* End action */
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
