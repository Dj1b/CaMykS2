<?php
/**
 * @brief CaMykSInstaller special plugin.
 * @details Plugin / Plugin Engine
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: May 2021
 * @date Modification: Nov 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * CaMykSInstaller class.
 */
final class CaMykSInstaller extends Plugin {
    /**
     * @var string $action
     * @brief Current action to execute in installer.
     */
    protected $action = 'Welcome';

    /**
     * @var Object $theme
     * @brief Link to current CaMykS theme.
     */
    protected $theme;

    /**
     * @var Object $configurationManager
     * @brief Link to configuration manager module.
     */
    protected $configurationManager;

    /**
     * @var array $currentConfiguration
     * @brief Current configuration
     */
    protected $currentConfiguration;

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
        $CaMykS->set_configValue('Website_Title', 'Website Installer');
        $CaMykS->set_configValue('Website_URL', dirname($this->get_actionLink()).'/');

        /* Load current configuration */
        $this->load_currentConfiguration();

        /* Load locales */
        $this->load_locales();

        /* Apply title */
        $CaMykS->HTMLPage->set_pageInformation($this->get_locale('TemplateHeaderTitle'));

        /* Load theme */
        $this->theme = &$CaMykS->currentTheme;

        /* Load configuration module */
        $this->configurationManager = $CaMykS->load_module('SystemConfigurationManager', 'System');
        if ($this->configurationManager === false) {
            /* Throw fatal error */

            die();
        }
        $this->configurationManager->initialise();
        $this->configurationManager->load_locales();

        /* Load specific styles */
        $this->add_styleIncludedFile('Installer.css',);
        $bkgdPicture = new File($this->get_filePath('Images/Background.png'));
        $this->add_styleScript("\n".'body {background-repeat:no-repeat; background-position:50% 50%; background-image:url("'.$bkgdPicture->get_base64Data().'");}');

        /* Load action to execute */
        $this->action = http_getStringRequest('InstallerAction', 'Welcome');

        /* Initialise action to execute */
        switch ($this->action) {
            case 'DisplayWebsiteConfiguration': /* Display website configuration */
                $this->initialise_DisplayWebsiteConfiguration();
                break;
            case 'SaveWebsiteConfiguration': /* Save website configuration */
                $this->initialise_SaveWebsiteConfiguration();
                break;

            case 'DisplayVisitConfiguration': /* Display visit configuration */
                $this->initialise_DisplayVisitConfiguration();
                break;
            case 'SaveVisitConfiguration': /* Save visit configuration */
                $this->initialise_SaveVisitConfiguration();
                break;

            case 'DisplayAdminConfiguration': /* Display admin configuration */
                $this->initialise_DisplayAdminConfiguration();
                break;
            case 'SaveAdminConfiguration': /* Save admin configuration */
                $this->initialise_SaveAdminConfiguration();
                break;

            case 'DisplayDatabaseConfiguration': /* Display database configuration */
                $this->initialise_DisplayDatabaseConfiguration();
                break;
            case 'SaveDatabaseConfiguration': /* Save database configuration */
                $this->initialise_SaveDatabaseConfiguration();
                break;

            case 'DisplayWebmasterConfiguration': /* Display webmaster account configuration */
                $this->initialise_DisplayWebmasterConfiguration();
                break;
            case 'SaveWebmasterConfiguration': /* Save webmaster account configuration */
                $this->initialise_SaveWebmasterConfiguration();
                break;

            case 'FinaliseInstallation': /* Finalise installation */
                $this->initialise_FinaliseInstallation();
                break;
            case 'DisplayInstallationDone': /* Display installation done */
                $this->initialise_DisplayInstallationDone();
                break;

            case 'Welcome': /* Main, default */
            default:
                $this->initialise_Welcome();
        }
    }

    /* Installation action methods */

    /**
     * Initialise plugin in Welcome mode.
     * @return void
     */
    private function initialise_Welcome() {
        global $CaMykS;

        /* Load CaMykS logo */
        $this->welcomePicture = new File($CaMykS->get_assetFilePath('Images/Logo/Logo500.png'));

        /* Update layout */
        $this->update_selectedLayout('Welcome');
    }

    /**
     * Initialise plugin in DisplayWebsiteConfiguration mode.
     * @return void
     */
    private function initialise_DisplayWebsiteConfiguration() {
        global $CaMykS;

        /* Build configuration form */
        $this->configurationForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveWebsiteConfiguration')));

        /* Load data */
        $this->localStorageValues = $CaMykS->localStorage->get_availableModes();
        $this->CMSSignatureValues = $this->configurationManager->get_displayCMSSignatureValues();
        $this->encodingValues = $this->configurationManager->get_encodingValues();

        /* Update layout */
        $this->section = 'Website';
        $this->update_selectedLayout('Configuration');
    }

    /**
     * Initialise plugin in SaveWebsiteConfiguration mode.
     * @return void
     */
    private function initialise_SaveWebsiteConfiguration() {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Website']['Website_Title'] = trim($_REQUEST['Title']);
        $this->currentConfiguration['Website']['Website_Version'] = trim($_REQUEST['Version']);
        $this->currentConfiguration['Website']['Website_Encoding'] = $_REQUEST['Encoding'];
        $this->currentConfiguration['Website']['Website_Author'] = trim($_REQUEST['Author']);
        $this->currentConfiguration['Website']['Website_DisplayCMSSignature'] = $_REQUEST['DisplayCMSSignature'];
        $this->currentConfiguration['Website']['Website_LocalStorage'] = $_REQUEST['LocalStorage'];

        /* Save configuration */
        $this->save_currentConfiguration();

        /* Check configuration */
        if ($this->check_websiteConfiguration())
            /* Redirect to next page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayVisitConfiguration'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in DisplayVisitConfiguration mode.
     * @return void
     */
    private function initialise_DisplayVisitConfiguration() {
        global $CaMykS;

        /* Load configuration */
        $CaMykS->load_configuration('Visit');

        /* Build configuration form */
        $this->configurationForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveVisitConfiguration')));

        /* Load available themes */
        $this->availableThemes = $this->configurationManager->get_availableVisitThemesTitles();
        if (isset($this->availableThemes[$CaMykS->get_configValue('Visit_Theme')]))
            $this->availableThemes = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableThemes[$CaMykS->get_configValue('Visit_Theme')])), $this->availableThemes);

        /* Load available templates */
        $this->availableTemplates = $this->configurationManager->get_availableVisitTemplatesTitles();
        if (isset($this->availableTemplates[$CaMykS->get_configValue('Visit_Template')]))
            $this->availableTemplates = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableTemplates[$CaMykS->get_configValue('Visit_Template')])), $this->availableTemplates);

        /* Load URL modes */
        $this->availableSSLModes = url_getSSLModes();
        if (isset($this->availableSSLModes[$CaMykS->get_configValue('Visit_SSLMode')]))
            $this->availableSSLModes = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableSSLModes[$CaMykS->get_configValue('Visit_SSLMode')])), $this->availableSSLModes);
        $this->availableURLModes = url_getBaseURLModes();
        if (isset($this->availableURLModes[$CaMykS->get_configValue('Visit_URLMode')]))
            $this->availableURLModes = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableURLModes[$CaMykS->get_configValue('Visit_URLMode')])), $this->availableURLModes);

        /* Load available languages */
        $this->availableLanguages = language_getLocalisedLongTitles(array('ShortList'=>$CaMykS->get_availableEngineLocaleLanguages()));


        /* Update layout */
        $this->section = 'Visit';
        $this->update_selectedLayout('Configuration');
    }

    /**
     * Initialise plugin in SaveVisitConfiguration mode.
     * @return void
     */
    private function initialise_SaveVisitConfiguration() {
        global $CaMykS;

        /* Laod values */

        /* Save configuration */
        $this->save_currentConfiguration();

        /* Load available languages */
        $this->availableLanguages = language_getLocalisedLongTitles(array('ShortList'=>$CaMykS->get_availableEngineLocaleLanguages()));

        /* Redirect to next page */
        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayAdminConfiguration'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in DisplayAdminConfiguration mode.
     * @return void
     */
    private function initialise_DisplayAdminConfiguration() {
        global $CaMykS;

        /* Load configuration */
        $CaMykS->load_configuration('Admin');

        /* Build configuration form */
        $this->configurationForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveAdminConfiguration')));

        /* Load available themes */
        $this->availableThemes = $this->configurationManager->get_availableAdminThemesTitles();
        if (isset($this->availableThemes[$CaMykS->get_configValue('Admin_Theme')]))
            $this->availableThemes = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableThemes[$CaMykS->get_configValue('Admin_Theme')])), $this->availableThemes);

        /* Load URL modes */
        $this->availableSSLModes = url_getSSLModes();
        if (isset($this->availableSSLModes[$CaMykS->get_configValue('Admin_SSLMode')]))
            $this->availableSSLModes = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableSSLModes[$CaMykS->get_configValue('Admin_SSLMode')])), $this->availableSSLModes);

        /* Load available languages */
        $this->availableLanguages = language_getLocalisedLongTitles(array('ShortList'=>$CaMykS->get_availableEngineLocaleLanguages()));

        /* Update layout */
        $this->section = 'Admin';
        $this->update_selectedLayout('Configuration');
    }

    /**
     * Initialise plugin in SaveAdminConfiguration mode.
     * @return void
     */
    private function initialise_SaveAdminConfiguration() {
        global $CaMykS;

        /* Load values */

        /* Save configuration */
        $this->save_currentConfiguration();

        /* Redirect to next page */
        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayDatabaseConfiguration'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in DisplayDatabaseConfiguration mode.
     * @return void
     */
    private function initialise_DisplayDatabaseConfiguration() {
        global $CaMykS;

        /* Load configuration */
        $CaMykS->load_configuration('Database');

        /* Build configuration form */
        $this->configurationForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveDatabaseConfiguration')));

        /* Load available database drivers */
        $this->databaseDrivers = DatabaseConnector::get_availableDrivers();
        if (isset($this->databaseDrivers[$CaMykS->get_configValue('Database_Driver')]))
            $this->databaseDrivers = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->databaseDrivers[$CaMykS->get_configValue('Database_Driver')])), $this->databaseDrivers);

        /* Update layout */
        $this->section = 'Database';
        $this->update_selectedLayout('Configuration');
    }

    /**
     * Initialise plugin in SaveDatabaseConfiguration mode.
     * @return void
     */
    private function initialise_SaveDatabaseConfiguration() {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Database']['Database_Driver'] = $_REQUEST['Driver'];
        $this->currentConfiguration['Database']['Database_Host'] = trim($_REQUEST['Host']);
        $this->currentConfiguration['Database']['Database_Name'] = trim($_REQUEST['Name']);
        $this->currentConfiguration['Database']['Database_Login'] = trim($_REQUEST['Login']);
        $this->currentConfiguration['Database']['Database_Password'] = $_REQUEST['Password'];
        $this->currentConfiguration['Database']['Database_Port'] = (int)$_REQUEST['Port'];
        $this->currentConfiguration['Database']['Database_TablePrefix'] = $_REQUEST['TablePrefix'];

        /* Save configuration */
        $this->save_currentConfiguration();

        /* Check configuration */
        if ($this->check_databaseConfiguration())
            /* Redirect to next page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayWebmasterConfiguration'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in DisplayWebmasterConfiguration mode.
     * @return void
     */
    private function initialise_DisplayWebmasterConfiguration() {
        global $CaMykS;

        /* Build configuration form */
        $this->configurationForm = new HTMLForm(array('Name'=>'ConfigurationForm', 'Action'=>$this->get_actionLink('SaveWebmasterConfiguration')));

        /* Load additional dictionnary */
        $CaMykS->load_engineLocales('Generic/PersonalData');

        /* Update layout */
        $this->section = 'Webmaster';
        $this->update_selectedLayout('Configuration');
    }

    /**
     * Initialise plugin in SaveWebmasterConfiguration mode.
     * @return void
     */
    private function initialise_SaveWebmasterConfiguration() {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Webmaster']['FirstName'] = trim($_REQUEST['FirstName']);
        $this->currentConfiguration['Webmaster']['LastName'] = trim($_REQUEST['LastName']);
        $this->currentConfiguration['Webmaster']['Email'] = trim($_REQUEST['Email']);
        $this->currentConfiguration['Webmaster']['Login'] = trim($_REQUEST['Login']);
        $this->currentConfiguration['Webmaster']['Password'] = $_REQUEST['Password'];
        $this->currentConfiguration['Webmaster']['PasswordConfirmation'] = $_REQUEST['PasswordConfirmation'];

        /* Load administrator manager module */
        $this->administratorManager = $CaMykS->load_module('SystemAdministratorManager', 'System');
        if ($this->administratorManager === false) {
            /* Throw fatal error */

            die();
        }
        $this->administratorManager->initialise();
        $this->administratorManager->load_locales();

        /* Save configuration */
        $this->save_currentConfiguration();

        /* Check configuration */
        if ($this->check_webmasterConfiguration())
            /* Redirect to next page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('FinaliseInstallation'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in FinaliseInstallation mode.
     * @return void
     */
    private function initialise_FinaliseInstallation() {
        global $CaMykS;

        /* Check again website configuration */
        $this->check_websiteConfiguration();

        /* Check again database configuation */
        $this->check_databaseConfiguration();

        /* Check write permission on website folder */
        if (!is_writable($CaMykS->get_websiteFileFullPath('Configuration'))) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_ConfigurationFolderIsNotWritable'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }

        /* Check database connection */

        /* Prepare installation manifest */
        $installationManifest = array();

        /* Write website configuration file */

        /* Write other configuration file */

        /* Connect to database server */

        /* Create database if not exists */

        /* Install system plugins */

        /* Create webmaster account in admin account manager */

        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('Welcome'));
        return;

        /* Save installation manifest */
        $CaMykS->set_sessionValue('InstallationManifest', $installationManifest);

        /* Redirect to installation done page */
        $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayInstallationDone'));

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Initialise plugin in DisplayInstallationDone mode.
     * @return void
     */
    private function initialise_DisplayInstallationDone() {
        global $CaMykS;

        /* Load installation manifest */
        $this->installationManifest = $CaMykS->set_gessionValue('InstallationManifest');

        /* Update layout */
        $this->update_selectedLayout('Done');
    }

    /* Configuration check methods */

    /**
     * Check website configuration.
     * @return boolean success
     */
    private function check_websiteConfiguration() {
        global $CaMykS;

        /* Check website title */
        if ($this->currentConfiguration['Website']['Website_Title'] === '') {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->configurationManager->get_locale('Error_TitleIsEmpty'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayWebsiteConfiguration'));
            $CaMykS->close();
        }

        /* Return succes */
        return true;
    }

    /**
     * Check database configuration.
     * @return boolean success
     */
    private function check_databaseConfiguration() {
        global $CaMykS;

        /* Check database name */
        if ($this->currentConfiguration['Database']['Database_Name'] === '') {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->configurationManager->get_locale('Error_DatabaseNameIsEmpty'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayDatabaseConfiguration'));
            $CaMykS->close();
        }

        /* Check database connexion */
        $dbConnector = new DatabaseConnector(array(
            'Driver'    => $this->currentConfiguration['Database']['Database_Driver'],
            'Host'      => $this->currentConfiguration['Database']['Database_Host'],
            'Port'      => $this->currentConfiguration['Database']['Database_Port'],
            'Login'     => $this->currentConfiguration['Database']['Database_Login'],
            'Password'  => $this->currentConfiguration['Database']['Database_Password'],
        ));

        if (!$dbConnector->check_status()) {
            /* Load last error from database connector */
            $error = $dbConnector->get_lastError();

            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $error['Message']);

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayDatabaseConfiguration'));
            $CaMykS->close();
        }

        if (!$dbConnector->open_connection()) {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->configurationManager->get_locale('Error_DatabaseCantConnectServer'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayDatabaseConfiguration'));
            $CaMykS->close();
        }

        /* Prepare message */
        if ($dbConnector->get_driverArchitecture())
            $message = $this->get_locale('Message_DatabaseConnected').' ';
        else
            $message = '';

        /* Check database already exists */
        if ($dbConnector->check_database($this->currentConfiguration['Database']['Database_Name']))
            $CaMykS->set_sessionValue('AdminInterface-Message', $message.$this->get_locale('Message_DatabaseExists'));
        else
            $CaMykS->set_sessionValue('AdminInterface-Message', $message.$this->get_locale('Message_DatabaseDoesntExist'));

        /* Check database driver is compatible with database local storage */
        if ($this->currentConfiguration['Website']['Website_LocalStorage'] === 'database' and !DatabaseConnector::is_driverCompatibleWithDatableLocalStorage($this->currentConfiguration['Database']['Database_Driver'])) {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->configurationManager->get_locale('Error_DatabaseDriverIsNotCompatibleWithDatabaseLocalStorage', $this->currentConfiguration['Database']['Database_Driver']));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayDatabaseConfiguration'));
            $CaMykS->close();
        }

        /* Return success */
        return true;
    }

    /**
     * Check webmaster configuration.
     * @return boolean success
     */
    private function check_webmasterConfiguration() {
        global $CaMykS;

        /* Check email value */
        if ($this->currentConfiguration['Webmaster']['Email'] === '') {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->administratorManager->get_locale('Error_AdminUserEmailIsEmpty'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayWebmasterConfiguration'));
            $CaMykS->close();
        } elseif (!email_isValid($this->currentConfiguration['Webmaster']['Email'])) {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->administratorManager->get_locale('Error_AdminUserEmailIsNotValid'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayWebmasterConfiguration'));
            $CaMykS->close();
        }

        /* Check password and confirmation */
        if ($this->currentConfiguration['Webmaster']['Password'] === '') {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->administratorManager->get_locale('Error_AdminUserPasswordIsEmpty'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayWebmasterConfiguration'));
            $CaMykS->close();
        } elseif ($this->currentConfiguration['Webmaster']['Password'] !== $this->currentConfiguration['Webmaster']['PasswordConfirmation']) {
            /* Store error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->administratorManager->get_locale('Error_AdminUserPasswordIsNotConfirmed'));

            /* Redirect to main page */
            $CaMykS->HTMLPage->set_redirect($this->get_actionLink('DisplayWebmasterConfiguration'));
            $CaMykS->close();
        }

        /* Return success */
        return true;
    }

    /* Tools methods */

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
            $link .= '?InstallerAction='.$action;

            /* Add params */
            if (is_array($params) and count($params) > 0) {
                foreach ($params as $name => $value)
                    $link .= '&'.$name.'='.$value;
            }
        }

        /* Return link */
        return $link;
    }

    /**
     * Load current configuration.
     * @return void
     */
    public function load_currentConfiguration() {
        global $CaMykS;

        /* Load configuration from session */
        $this->currentConfiguration = $CaMykS->get_sessionValue('InstallationConfiguration', null);

        //$this->currentConfiguration = null;

        /* Check if configuration has been defined */
        if (!is_null($this->currentConfiguration))
            return;

        /* Create new configuration */
        $this->currentConfiguration = array();

        /* Load website configuration */
        @include($CaMykS->get_websiteFileFullPath('Configuration/Website.php.inc'));
        if (isset($configuration))
            $this->currentConfiguration['Website'] = $configuration;

        /* Load system configurations */
        $config = new ConfigurationFile();

        foreach (array('Visit', 'Admin', 'Database') as $section) {
            $data = $config->load($section, 'Engine');
            /* Clean pre-defined configuration */
            foreach ($data as $name => $value)
                $data[$name] = '';
            $this->currentConfiguration[$section] = $data;
        }

        /* Adds default webmaster configuration */
        $this->currentConfiguration['Webmaster'] = array(
            'FirstName' => '',
            'LastName'  => '',
            'Email'     => '',
            'Login'     => '',
            'Password'  => '',
        );

        /* Save configuration */
        $this->save_currentConfiguration();
    }

    /**
     * Save current configuration.
     * @return void
     */
    public function save_currentConfiguration() {
        global $CaMykS;

        /* Load configuration from session */
        $CaMykS->set_sessionValue('InstallationConfiguration', $this->currentConfiguration);
    }
}
?>
