<?php
/**
 * @brief CaMykSInstaller special plugin.
 * @details Plugin / Plugin Engine
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre11
 * @date Creation: May 2021
 * @date Modification: Oct 2026
 * @copyright 2021 - 2026 CaMykS Team
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
    protected string $action = 'Welcome';

    /**
     * @var Object $theme
     * @brief Link to current CaMykS theme.
     */
    protected object $theme;

    /**
     * @var Object $configurationManager
     * @brief Link to configuration manager module.
     */
    protected object $configurationManager;

    /**
     * @var array $currentConfiguration
     * @brief Current configuration
     */
    protected array $currentConfiguration;

    /**
     * @var string $section
     * @brief Current section to configure.
     */
    protected string $section;

    /**
     * @var object $welcomePicture
     * @brief Welcome picture file.
     */
    protected object $welcomePicture;

    /**
     * @var object $configurationForm
     * @brief Configuration form.
     */
    protected object $configurationForm;

    /**
     * @var array $localStorageValues
     * @brief List of available storage values.
     */
    protected array $localStorageValues;

    /**
     * @var array $CMSSignatureValues
     * @brief List of available signature values.
     */
    protected array $CMSSignatureValues;

    /**
     * @var array $encodingValues
     * @brief List of available encoding values.
     */
    protected array $encodingValues;

    /**
     * @var array $availableThemes
     * @brief List of available themes.
     */
    protected array $availableThemes;

    /**
     * @var array $availableTemplates
     * @brief List of available templates.
     */
    protected array $availableTemplates;

    /**
     * @var array $availableSSLModes
     * @brief List of available SSL modes.
     */
    protected array $availableSSLModes;

    /**
     * @var array $availableURLModes
     * @brief List of available URL modes.
     */
    protected array $availableURLModes;

    /**
     * @var array $availableLanguages
     * @brief List of available languages.
     */
    protected array $availableLanguages;

    /**
     * @var array $databaseDrivers
     * @brief List of available database drivers.
     */
    protected array $databaseDrivers;

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
    public function initialise($params=array()): void {
        global $CaMykS;
        parent::initialise($params);

        /* Override some website configuration variables */
        //$CaMykS->set_configValue('Website_Name', $this->libraryName);
        $CaMykS->set_configValue('Website_Title', 'Website Installer');
        $CaMykS->set_configValue('Website_URL', dirname($this->get_actionLink()).'/');
        $CaMykS->set_configValue('Admin_HeaderLogoFile', $this->get_filePath('Images/Banner.svg'));

        /* Load current configuration */
        $this->load_currentConfiguration();

        /* Load locales */
        $this->load_locales();

        /* Apply title */
        $CaMykS->HTMLPage->set_pageInformation($this->get_locale('TemplateHeaderTitle'));

        /* Load theme */
        $this->theme = &$CaMykS->currentTheme;

        /* Define body class */
        $CaMykS->HTMLPage->set_bodyClass('installer');

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
    private function initialise_Welcome(): void {
        global $CaMykS;

        /* Load CaMykS logo */
        $this->welcomePicture = new File($CaMykS->get_assetFilePath('Images/Logo/Logo500.png'));

        /* Reset configuration if requered */
        $this->currentConfiguration = array();
        $this->save_currentConfiguration();

        /* Update layout */
        $this->update_selectedLayout('Welcome');
    }

    /**
     * Initialise plugin in DisplayWebsiteConfiguration mode.
     * @return void
     */
    private function initialise_DisplayWebsiteConfiguration(): void {
        global $CaMykS;

        /* Load configuration */
        $this->load_currentConfiguration();

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
    private function initialise_SaveWebsiteConfiguration(): void {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Website']['Website_CaMykSMode'] = $this->get_configValue('Website_CaMykSPath') == 'CaMykS2' ? 'Dedicated' : 'Shared';
        $this->currentConfiguration['Website']['Website_Title'] = trim(http_getStringRequest('Title'));
        $this->currentConfiguration['Website']['Website_Version'] = trim(http_getStringRequest('Version'));
        $this->currentConfiguration['Website']['Website_Encoding'] = http_getStringRequest('Encoding');
        $this->currentConfiguration['Website']['Website_Author'] = trim(http_getStringRequest('Author'));
        $this->currentConfiguration['Website']['Website_DisplayCMSSignature'] = http_getIntRequest('DisplayCMSSignature');
        $this->currentConfiguration['Website']['Website_LocalStorage'] = http_getStringRequest('LocalStorage');

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
    private function initialise_DisplayVisitConfiguration(): void {
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

        /* Load SSL modes */
        $this->availableSSLModes = url_getSSLModes();
        if (isset($this->availableSSLModes[$CaMykS->get_configValue('Visit_SSLMode')]))
            $this->availableSSLModes = array_merge(array('' => $this->get_locale('UseCaMykSConfiguration', $this->availableSSLModes[$CaMykS->get_configValue('Visit_SSLMode')])), $this->availableSSLModes);

        /* Load URL modes */
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
    private function initialise_SaveVisitConfiguration(): void {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Visit']['Visit_DefaultLanguage'] = http_getStringRequest('DefaultLanguage');
        $this->currentConfiguration['Visit']['Visit_Languages'] = array(http_getStringRequest('DefaultLanguage'));
        $this->currentConfiguration['Visit']['Visit_Theme'] = http_getStringRequest('Theme');
        $this->currentConfiguration['Visit']['Visit_Template'] = http_getStringRequest('Template');
        $this->currentConfiguration['Visit']['Visit_URLMode'] = http_getStringRequest('URLMode');
        $this->currentConfiguration['Visit']['Visit_SSLMode'] = http_getStringRequest('SSLMode');
        $this->currentConfiguration['Visit']['Visit_AutoLoadCaMykSJS'] = http_getIntRequest('AutoLoadCaMykSJS');
        $this->currentConfiguration['Visit']['Visit_ClientSideCache'] = http_getIntRequest('ClientSideCache');
        $this->currentConfiguration['Visit']['Visit_ServerSideCache'] = http_getIntRequest('ServerSideCache');

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
    private function initialise_DisplayAdminConfiguration(): void {
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
    private function initialise_SaveAdminConfiguration(): void {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Admin']['Admin_DefaultLanguage'] = http_getStringRequest('DefaultLanguage');
        $this->currentConfiguration['Admin']['Admin_Languages'] = array(http_getStringRequest('DefaultLanguage'));
        $this->currentConfiguration['Admin']['Admin_EditionLanguages'] = array(http_getStringRequest('DefaultLanguage'));
        $this->currentConfiguration['Admin']['Admin_Theme'] = http_getStringRequest('Theme');
        $this->currentConfiguration['Admin']['Admin_EntryPoint'] = http_getStringRequest('EntryPoint');
        $this->currentConfiguration['Admin']['Admin_SSLMode'] = http_getStringRequest('SSLMode');
        $this->currentConfiguration['Admin']['Admin_KeepAlive'] = http_getIntRequest('KeepAlive');
        $this->currentConfiguration['Admin']['Admin_SaveMode'] = http_getIntRequest('SaveMode');

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
    private function initialise_DisplayDatabaseConfiguration(): void {
        global $CaMykS;

        /* Load configuration */
        $CaMykS->load_configuration('Database');

        /* Check for default database name */
        if ($this->get_currentConfigurationValue('Database', 'Database_Name') == '')
            $this->set_currentConfigurationValue('Database', 'Database_Name', $CaMykS->get_configValue('Website_Name'));

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
    private function initialise_SaveDatabaseConfiguration(): void {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Database']['Database_Driver'] = http_getStringRequest('Driver');
        $this->currentConfiguration['Database']['Database_Host'] = trim(http_getStringRequest('Host'));
        $this->currentConfiguration['Database']['Database_Name'] = trim(http_getStringRequest('Name'));
        $this->currentConfiguration['Database']['Database_Login'] = trim(http_getStringRequest('Login'));
        $this->currentConfiguration['Database']['Database_Password'] = http_getStringRequest('Password');
        $this->currentConfiguration['Database']['Database_Port'] = http_getIntRequest('Port');
        $this->currentConfiguration['Database']['Database_TablePrefix'] = http_getStringRequest('TablePrefix');

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
    private function initialise_DisplayWebmasterConfiguration(): void {
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
    private function initialise_SaveWebmasterConfiguration(): void {
        global $CaMykS;

        /* Load values */
        $this->currentConfiguration['Webmaster']['FirstName'] = trim(http_getStringRequest('FirstName'));
        $this->currentConfiguration['Webmaster']['LastName'] = trim(http_getStringRequest('LastName'));
        $this->currentConfiguration['Webmaster']['Email'] = trim(http_getStringRequest('Email'));
        $this->currentConfiguration['Webmaster']['Login'] = trim(http_getStringRequest('Login'));
        $this->currentConfiguration['Webmaster']['Password'] = http_getStringRequest('Password');
        $this->currentConfiguration['Webmaster']['PasswordConfirmation'] = http_getStringRequest('PasswordConfirmation');

        /* Load administrator manager module */
        $administratorManager = $CaMykS->load_module('SystemAdministratorManager', 'System');
        if ($administratorManager === false) {
            /* Throw fatal error */

            die();
        }
        $administratorManager->initialise();
        $administratorManager->load_locales();

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
    private function initialise_FinaliseInstallation(): void {
        global $CaMykS;

        /* Check again website configuration */
        $this->check_websiteConfiguration();

        /* Check again database configuation */
        $this->check_databaseConfiguration();

        /* Clear messages */
        $CaMykS->set_sessionValue('AdminInterface-Message', '');

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
        $installationManifest[] = $this->get_locale('Manifest_WebsiteInstallationStarted');

        /* Write website configuration file */
        $configuration = new ConfigurationFile('Website', 'Website', 'Write');
        $configuration->load();
        $configuration->merge($this->currentConfiguration['Website']);
        if (!$configuration->save()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CantWriteWebsiteConfiguration'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }
        $installationManifest[] = $this->get_locale('Manifest_WebsiteConfigurationFileWritten');

        /* Drop empty visit configuration values */
        if ($this->currentConfiguration['Visit']['Visit_Theme'] === '') unset($this->currentConfiguration['Visit']['Visit_Theme']);
        if ($this->currentConfiguration['Visit']['Visit_Template'] === '') unset($this->currentConfiguration['Visit']['Visit_Template']);
        if ($this->currentConfiguration['Visit']['Visit_URLMode'] === '') unset($this->currentConfiguration['Visit']['Visit_URLMode']);
        if ($this->currentConfiguration['Visit']['Visit_SSLMode'] === '') unset($this->currentConfiguration['Visit']['Visit_SSLMode']);

        /* Write visit configuration file */
        $configuration = new ConfigurationFile('Visit', 'Website', 'Write');
        $configuration->merge($this->currentConfiguration['Visit']);
        if (!$configuration->save()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CantWriteVisitConfiguration'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }
        $installationManifest[] = $this->get_locale('Manifest_VisitConfigurationFileWritten');

        /* Drop empty admin configuration values */
        if ($this->currentConfiguration['Admin']['Admin_Theme'] === '') unset($this->currentConfiguration['Admin']['Admin_Theme']);
        if ($this->currentConfiguration['Admin']['Admin_EntryPoint'] === '') unset($this->currentConfiguration['Admin']['Admin_EntryPoint']);
        if ($this->currentConfiguration['Admin']['Admin_SSLMode'] === '') unset($this->currentConfiguration['Admin']['Admin_SSLMode']);

        /* Write admin configuration file */
        $configuration = new ConfigurationFile('Admin', 'Website', 'Write');
        $configuration->merge($this->currentConfiguration['Admin']);
        if (!$configuration->save()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CantWriteAdminConfiguration'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }
        $installationManifest[] = $this->get_locale('Manifest_AdminConfigurationFileWritten');

        /* Drop empty database configuration values */
        if ($this->currentConfiguration['Database']['Database_Driver'] === '') unset($this->currentConfiguration['Database']['Database_Driver']);
        if ($this->currentConfiguration['Database']['Database_Host'] === '') unset($this->currentConfiguration['Database']['Database_Host']);
        if ($this->currentConfiguration['Database']['Database_Login'] === '') unset($this->currentConfiguration['Database']['Database_Login']);
        if ($this->currentConfiguration['Database']['Database_Password'] === '') unset($this->currentConfiguration['Database']['Database_Password']);
        if ($this->currentConfiguration['Database']['Database_Port'] === '') unset($this->currentConfiguration['Database']['Database_Port']);

        /* Write database configuration file */
        $configuration = new ConfigurationFile('Database', 'Website', 'Write');
        $configuration->merge($this->currentConfiguration['Database']);
        if (!$configuration->save()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CantWriteDatabaseConfiguration'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }
        $installationManifest[] = $this->get_locale('Manifest_DatabaseConfigurationFileWritten');

        /* Declare website as installed to let CaMykS access database configuration */
        $CaMykS->set_configValue('Website_Status', 1);

        /* Connect to database server */
        $CaMykS->initialise_mainDatabase();
        if (!$CaMykS->databaseConnection->open_connection()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->configurationManager->get_locale('Error_DatabaseCantConnectServer'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }
        $installationManifest[] = $this->get_locale('Manifest_DatabaseConnectionOpened');

        /* Create database if not exists */
        if (!$CaMykS->databaseConnection->check_database()) {
            if (!$CaMykS->databaseConnection->create_database()) {
                /* Define error */
                $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_DatabaseCantCreate'));

                /* Update layout */
                $this->update_selectedLayout('FinalisationError');

                /* Avoid doing other checks and installation */
            return;
            }
            $installationManifest[] = $this->get_locale('Manifest_DatabaseConnectionCreated');
        }

        /* Open database */
        if (!$CaMykS->databaseConnection->select_database()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_DatabaseCantOpen'));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }

        $installationManifest[] = $this->get_locale('Manifest_DatabaseOpened');

        /* Create symbolic links for websites using shared CaMykS */
        if ($this->currentConfiguration['Website']['Website_CaMykSMode'] === 'Shared') {
            /* Check and create Core Plugin symbolic link */
            if (($l = readlink($CaMykS->corePluginLink)) !== false) {
                if ($l !== $this->currentConfiguration['Website']['Website_CaMykSPath'].'/Core/Plugins') {
                    /* Define error */
                    $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CorePluginsSymLinkExistsAndIsNotValid'));

                    /* Update layout */
                    $this->update_selectedLayout('FinalisationError');

                    /* Avoid doing other checks and installation */
            return;
                }
                $installationManifest[] = $this->get_locale('Manifest_SymLinkCorePluginsAlreadyExists');
            } elseif (!symlink($this->currentConfiguration['Website']['Website_CaMykSPath'].'/Core/Plugins', $CaMykS->corePluginLink)) {
                /* Define error */
                $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_SymLinkCantCreate'));

                /* Update layout */
                $this->update_selectedLayout('FinalisationError');

                /* Avoid doing other checks and installation */
            return;
            } else
                $installationManifest[] = $this->get_locale('Manifest_SymLinkCorePluginsCreated');

            /* Check and create Shared Plugins symbolic link */
            if (($l = readlink($CaMykS->sharedPluginLink)) !== false) {
                if ($l !== $this->currentConfiguration['Website']['Website_CaMykSPath'].'/Plugins') {
                    /* Define error */
                    $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_SharedPluginsSymLinkExistsAndIsNotValid'));

                    /* Update layout */
                    $this->update_selectedLayout('FinalisationError');

                    /* Avoid doing other checks and installation */
            return;
                }
                $installationManifest[] = $this->get_locale('Manifest_SymLinkSharedPluginsAlreadyExists');
            } elseif (!symlink($this->currentConfiguration['Website']['Website_CaMykSPath'].'/Plugins', $CaMykS->sharedPluginLink)) {
                /* Define error */
                $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_SymLinkCantCreate'));

                /* Update layout */
                $this->update_selectedLayout('FinalisationError');

                /* Avoid doing other checks and installation */
            return;
            } else
                $installationManifest[] = $this->get_locale('Manifest_SymLinkSharedPluginsCreated');

            /* Check and create Shared Plugins symbolic link */
            if (($l = readlink($CaMykS->coreAssetLink)) !== false) {
                if ($l !== $this->currentConfiguration['Website']['Website_CaMykSPath'].'/Core/Assets') {
                    /* Define error */
                    $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CoreAssetsSymLinkExistsAndIsNotValid'));

                    /* Update layout */
                    $this->update_selectedLayout('FinalisationError');

                    /* Avoid doing other checks and installation */
            return;
                }
                $installationManifest[] = $this->get_locale('Manifest_SymLinkCoreAssetsAlreadyExists');
            } elseif (!symlink($this->currentConfiguration['Website']['Website_CaMykSPath'].'/Core/Assets', $CaMykS->coreAssetLink)) {
                /* Define error */
                $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_SymLinkCantCreate'));

                /* Update layout */
                $this->update_selectedLayout('FinalisationError');

                /* Avoid doing other checks and installation */
            return;
            } else
                $installationManifest[] = $this->get_locale('Manifest_SymLinkAssetsCreated');
        }

        /* Install system plugin manager plugin */
        $pluginManager = $CaMykS->get_module('SystemPluginManager', true, 'System');

        if (!$pluginManager->install()) {
            /* Define error */
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_PluginCantInstall', $pluginManager->libraryName));

            /* Update layout */
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation */
            return;
        }
        $installationManifest[] = $this->get_locale('Manifest_PluginInstalled', $pluginManager->libraryName);

        /* Load other default system plugins */
        $CaMykS->load_configuration('DefaultPlugins');

        /* Remove plugin manager */
        if (isset($pluginList['Module']['SystemPluginManager']))
            unset($pluginList['Module']['SystemPluginManager']);

        /* Install other default system plugins */
        foreach ($CaMykS->get_pluginTypes() as $type) {
            foreach ($CaMyk->get_configValue('Default'.$type.'s') as $name) {
                if (!$pluginManager->enable_plugin($name, $type, 'System')) {
                    /* Define error */
                    $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_PluginCantInstall', $name));

                    /* Update layout */
                    $this->update_selectedLayout('FinalisationError');

                    /* Avoid doing other checks and installation */
            return;
                }
                $installationManifest[] = $this->get_locale('Manifest_PluginInstalled', $name);
            }
        }

        /* Create webmaster account in admin account manager */

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
    private function initialise_DisplayInstallationDone(): void {
        global $CaMykS;

        /* Update website configuration status *
        $configuration = new ConfigurationFile('Website', 'Website', 'Write');
        $configuration->load();
        $configuration->merge($this->currentConfiguration['Website']);
        if (!$configuration->save()) {
            /* Define error *
            $CaMykS->set_sessionValue('AdminInterface-Error', $this->get_locale('Error_CantWriteWebsiteConfiguration'));

            /* Update layout *
            $this->update_selectedLayout('FinalisationError');

            /* Avoid doing other checks and installation *
            return;
        }

        /* Read admin configuration to get admin entry point */
        $CaMykS->load_configuration('Admin');
        $this->adminLink = $CaMykS->get_configValue('Admin_EntryPoint');

        /* Load master scripts */
        $CaMykS->load_CaMykSJSScript();

        /* Load Media Popup component */
        $this->mediaPopupComponent = $CaMykS->load_component('MediaPopup');
        if ($this->mediaPopupComponent !== false)
            $this->mediaPopupComponent->plug();

        /* Load installation manifest */
        $this->installationManifest = $CaMykS->get_sessionValue('InstallationManifest');

        /* Clear configuration */
        // $this->currentConfiguration = array();

        /* Update layout */
        $this->update_selectedLayout('Done');
    }

    /* Configuration check methods */

    /**
     * Check website configuration.
     * @return boolean success
     */
    private function check_websiteConfiguration(): bool {
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
    private function check_databaseConfiguration(): bool {
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
    private function check_webmasterConfiguration(): bool {
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
    protected function get_actionLink($action='', $params=array()): string {
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
    private function load_currentConfiguration(): void {
        global $CaMykS;

        /* Load configuration from session */
        $this->currentConfiguration = $CaMykS->get_sessionValue('InstallationConfiguration', array());

        /* Check if configuration has been defined */
        if (!empty($this->currentConfiguration))
            return;

        /* Create new configuration */
        $this->currentConfiguration = array();

        /* Load website configuration */
        @include($CaMykS->get_websiteFileFullPath('Configuration/Website.php.inc'));
        if (isset($configuration))
            $this->currentConfiguration['Website'] = $configuration;

        /* Load system configurations */
        foreach (array('Visit', 'Admin', 'Database') as $section) {
            $config = new ConfigurationFile($section, 'Engine');
            $data = $config->export();

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
    private function save_currentConfiguration(): void {
        global $CaMykS;

        /* Load configuration from session */
        $CaMykS->set_sessionValue('InstallationConfiguration', $this->currentConfiguration);
    }

    /**
     * Return current configuration value.
     * @param string $section
     * @param string $name
     * @return mixed
     */
    protected function get_currentConfigurationValue($section, $name): mixed {
        if (isset($this->currentConfiguration[$section][$name]))
            return $this->currentConfiguration[$section][$name];
        return '';
    }

    /**
     * Define current configuration value.
     * @param string $section
     * @param string $name
     * @param mixed $value
     * @return void
     */
    protected function set_currentConfigurationValue($section, $name, $value): void {
        if (!isset($this->currentConfiguration[$section][$name]))
            return;
        $this->currentConfiguration[$section][$name] = $value;
    }
}
?>
