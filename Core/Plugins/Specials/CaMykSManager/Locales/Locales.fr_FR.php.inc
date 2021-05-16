<?php
/**
 * @brief CaMykSManager plugin, french dictionary.
 * @details Plugin / Dictionary
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre9
 * @date Creation: Mar 2020
 * @date Modification: May 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY,
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
$locales = array(
    /* Teader title */
    'TemplateHeaderTitle' => 'Gestionnaire de sites',

    /* Simple words */
    'Password' => 'Mot de passe',

    /* Buttons */
    'Action_Retry' => 'R&eacute;essayer',
    'Action_ConfigureCaMykS2' => 'Configurer CaMykS 2',
    'Action_CaMykS2Information' => 'Serveur CaMykS 2',
    'Action_CloseWindow' => 'Fermer la fen&ecirc;tre',
    'Action_BackToSiteList' => 'Retour &agrave; la liste des sites',
    'Action_Update' => 'Mettre &agrave; jour',
    'Action_CheckIntegrity' => 'V&eacute;rifier l\'int&eacute;grit&eacute;',
    'Action_ResetOpcodeCache' => 'Vider les caches OpCode',

    /* Folder types */
    'File' => 'Fichier',
    'Folder' => 'Dossier',
    'CaMykSv1Engine' => 'CaMykS v1',
    'CaMykSv2Engine' => 'CaMykS v2',
    'CaMykSv1Site' => 'Site CaMykS v1',
    'CaMykSv2Site' => 'Site CaMykS v2',
    'NoCaMykSSite' => 'Autre site',

    /* No password defined error interface */
    'Error_NoPasswordDefined_Title' => 'Aucun mot de passe d&eacute;fini !',
    'Error_NoPasswordDefined_Content' => 'Par s&eacute;curit&eacute;, le gestionnaire de site n\'est pas accessible tant qu\'un mot de passe n\'a pas &eacute;t&eacute; d&eacute;fini. Ouvrez le fichier de configuration "Manager" situ&eacute; dans le dossier "Configuration" et ins&eacute;rez un mot de passe dans la variable "Manager_Password", puis rechargez la page.',

    /* Password saving error interface */
    'Error_CantSavePassword_Title' => 'Erreur lors de l\'enregistrement du mot de passe.',
    'Error_CantSavePassword_Content' => 'V&eacute;rifiez que votre serveur web dispose des autorisations pour &eacute;crire les fichiers de configuration, avant de r&eacute;essayer.',

    /*  Error on visiting or openin admin website */
    'Error_CantVisitNonCaMykSWebsites_Title' => 'Impossible de visiter le site',
    'Error_CantVisitNonCaMykSWebsites_Content' => 'Pour visiter le site, le dossier indiqu&eacute; doit &ecirc;tre valide et correspondre &agrave; un site g&eacute;rer gr&acirc;ce &agrave; CaMykS (version 1 ou 2).',

    /* Login interface */
    'Login_Title' => 'Connexion',
    'Login_Content' => '',
    'Login_Access' => 'Acc&eacute;der',
    'Login_ConnectionNotScured' => 'Attention : Votre connexion n\'est pas s&eacute;curis&eacute;e. Vous prenez un risque en vous connectant, votre mot de passe sera transmis en clair vers le serveur.',

    /* Site list interface */
    'SiteList_ServerNameX' => 'Serveur %s',
    'SiteList_NewCaMykSv2WebsiteInX' => 'Nouveau site CaMykS 2 dans %s',
    'SiteList_NewFolderInX' => 'Nouveau dossier dans %s',
    'SiteList_VisitWebsite' => 'Visiter le site internet',
    'SiteList_AccessWebsiteAdmin' => 'Acc&eacute;der &agrave; l\'administration du site internet',
    'SiteList_ViewWebsiteInfo' => 'Afficher les informations du site internet',

    /* Site list information panel */
    'SiteList_InfoPanel_GeneralInfo' => 'Informations g&eacute;n&eacute;rales',
    'SiteList_InfoPanel_Title' => 'Titre',
    'SiteList_InfoPanel_Version' => 'Version',
    'SiteList_InfoPanel_URL' => 'URL',
    'SiteList_InfoPanel_Engine' => 'Moteur',
    'SiteList_InfoPanel_InstallationMode' => 'Mode d\'installation',
    'SiteList_InfoPanel_Status' => '&Eacute;tat',
    'SiteList_InfoPanel_Author' => 'Auteur',
    'SiteList_InfoPanel_VisitMode' => 'Visite',
    'SiteList_InfoPanel_SSL' => 'SSL',
    'SiteList_InfoPanel_Languages' => 'Langues',
    'SiteList_InfoPanel_AdminMode' => 'Administration',
    'SiteList_InfoPanel_EntryPoint' => 'Point d\'entr&eacute;e',
    'SiteList_InfoPanel_EditionLanguages' => 'Langues en &eacute;dition',
    'SiteList_InfoPanel_Database' => 'Base de donn&eacute;es',
    'SiteList_InfoPanel_DatabaseDriver' => 'Driver',
    'SiteList_InfoPanel_DatabaseName' => 'Nom',
    'SiteList_InfoPanel_DatabaseServer' => 'Serveur',
    'SiteList_InfoPanel_DatabaseUser' => 'Utilisateur',
    'SiteList_InfoPanel_DatabasePrefix' => 'Pr&eacute;fixe des tables',

    /* Information panel interface */
    'Information_CurrentVersion' => 'Version courante',
    'Information_LatestVersion' => 'Derni&egrave;re version',
    'Information_Server' => 'Serveur',
    'Information_WebServer' => 'Serveur internet',
    'Information_Modules_Required' => 'Modules Requis',
    'Information_Modules_Recommended' => 'Modules Recommand&eacute;s',
    'Information_Modules_Dependences' => 'Modules d&eacute;pendants pour les librairies',
    'Information_PHP' => 'PHP',
    'Information_AvailableDatabaseDrivers' => 'Connecteurs de base de donn&eacute;es',

    /* ResetOpcodeCache */
    'ResetOpcodeCache_Success' => 'Le cache OpCode a &eacute;t&eacute; vid&eacute; avec succ&egrave;s.',
    'ResetOpcodeCache_Error' => 'Une erreur est survenue lors de la suppression du cache OpCode.',

    /* Configuration panel interface */
    'Configuration_ManagerSettings' => 'Gestionnaire de sites',
    'Configuration_EngineSettings' => 'Moteur CaMykS',
    'Configuration_VisitSettings' => 'Mode visiteur',
    'Configuration_AdminSettings' => 'Mode administration',
    'Configuration_DatabaseSettings' => 'Base de donn&eacute;es',
    'Configuration_PluginsSettings' => 'Plugins par d&eacute;faut',
    'Configuration_NoneTitle' => 'S&eacute;lectionnez une section &agrave; configurer.',

    /* Configuration panel all interface */
    'Configuration_Edit' => 'Modifier la configuration',
    'Configuration_Parameters' => 'Param&egrave;tres',

    /* Configuration Manager panel interface */
    'Configuration_EditManagerPassword' => 'Modifier le mot de passe',
    'Configuration_Language' => 'Langue',
    'Configuration_DefaultLanguage' => 'Langue par d&eacute;faut',
    'Configuration_DefaultLanguageAutomatic' => 'Automatique',
    'Configuration_Theme' => 'Th&egrave;me',
    'Configuration_ThemeAutomatic' => 'Automatique',
    'Configuration_SiteList' => 'Liste des sites',
    'Configuration_SiteListPath' => 'Dossier racine',
    'Configuration_SiteListSearchLevel' => 'Niveaux de recherche',
    'Configuration_SiteListHiddenFiles' => 'Fichiers &agrave; cacher',
    'Configuration_SiteListHiddenFilesNone' => 'Aucun',
    'Configuration_SiteListHiddenFolders' => 'Dossiers &agrave; cacher',
    'Configuration_SiteListHiddenFoldersNone' => 'Aucun',
    'Configuration_Security' => 'S&eacute;curit&eacute',
    'Configuration_SSLEnabled' => 'Activer SSL',
    'Configuration_RestrainedIPs' => 'Adresses IP Limit&eacute;es',
    'Configuration_RestrainedIPsNone' => 'Aucune',

    /* Configuration Admin panel interface */
    'Configuration_Languages' => 'Langues',
    'Configuration_LanguagesUndefined' => 'Non d&eacute;finies',
    'Configuration_EditionLanguages' => 'Langues en &eacute;ditions',
    'Configuration_EntryPoint' => 'Point d\'entr&eacute;e',
    'Configuration_EntryPointUndefined' => 'Non d&eacute;fini',
    'Configuration_ThemeUndefined' => 'Non d&eacute;fini',
    'Configuration_SSLMode' => 'SSL',
    'Configuration_SSLUndefined' => 'Non d&eacute;fini',
    'Configuration_KeepAlive' => 'Conserver la connexion',
    'Configuration_KeepAliveUndefined' => 'Non d&eacute;fini',
    'Configuration_SaveMode' => 'Mode d\'enregistrement',
    'Configuration_SaveModeUndefined' => 'Non d&eacute;fini',

    /* Configuration Database panel interface */
    'Configuration_Driver' => 'Driver',
    'Configuration_DriverUndefined' => 'Non d&eacute;fini',
    'Configuration_Host' => 'H&ocirc;te',
    'Configuration_HostUndefined' => 'Non d&eacute;fini',
    'Configuration_Port' => 'Port',
    'Configuration_PortAutomatic' => 'Automatique',
    'Configuration_Secure' => 'S&eacute;curit&eacute;',
    'Configuration_SecureUndefined' => 'Non d&eacute;fini',
    'Configuration_TablePrefix' => 'Pr&eacute;fix des tables',
    'Configuration_TablePrefixUndefined' => 'Non d&eacute;fini',

    /* Configuration Engine panel interface */
    'Configuration_TimeZone' => 'Fuseau horaire',
    'Configuration_TimeZoneUndefined' => 'Non d&eacute;fini',
    'Configuration_WebmasterEmail' => 'Email du webmaster',
    'Configuration_WebmasterEmailUndefined' => 'Non d&eacute;fini',
    'Configuration_ErrorLogging' => 'Gestion des erreurs',
    'Configuration_ErrorLoggingLevel' => 'Niveau',
    'Configuration_ErrorLoggingLevelUndefined' => 'Non d&eacute;fini',
    'Configuration_ErrorLoggingOutput' => 'Sortie',
    'Configuration_ErrorLoggingOutputUndefined' => 'Non d&eacute;finie',
    'Configuration_DefaultTools' => 'Outils',
    'Configuration_PictureEditor' => 'Editeur d\'images',
    'Configuration_PictureEditorUndefined' => 'Non d&eacute;fini',
    'Configuration_Requests' => 'Requ&ecirc;tes',
    'Configuration_RequestUserAgent' => 'Agent utilisateur',
    'Configuration_RequestUserAgentUndefined' => 'Non d&eacute;fini',

    /* Configuration Visit panel interface */
    'Configuration_GraphicalAspect' => 'Aspect graphique',
    'Configuration_Template' => 'Template',
    'Configuration_TemplateUndefined' => 'Non d&eacute;fini',
    'Configuration_URLs' => 'Param&eacute;trage des URLS',
    'Configuration_URLMode' => 'Mode',
    'Configuration_AdvancedParameters' => 'Param&egrave;tres avanc&eacute;s',
    'Configuration_ServerSideCache' => 'Cache c&ocirc;t&eacute; serveur',
    'Configuration_ClientSideCache' => 'Cache c&ocirc;t&eacute; client',
    'Configuration_AutoLoadCaMykSJS' => 'Chargement auto. de CaMykS.js',
);
?>
