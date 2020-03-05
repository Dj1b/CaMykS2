<?php
/**
 * @brief CaMykSManager plugin, french dictionnary.
 * @details Plugin / Dictionnary
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY,
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
$locales = array(
    /* Teader title */
    'TemplateHeaderTitle' => 'Gestionnaire de sites',

    /* Simple words */
    'Password' => 'Mot de passe',

    /* Buttons */
    'Error_Button_Retry' => 'R&eacute;essayer',

    /* No password defined error interface */
    'Error_NoPasswordDefined_Title' => 'Aucun mot de passe d&eacute;fini !',
    'Error_NoPasswordDefined_Content' => 'Par s&eacute;curit&eacute;, le gestionnaire de site n\'est pas accessible tant qu\'un mot de passe n\'a pas &eacute;t&eacute; d&eacute;fini. Ouvrez le fichier de configuration "Manager" situ&eacute; dans le dossier "Configuration" et ins&eacute;rez un mot de passe dans la variable "Manager_Password", puis rechargez la page.',

    /* Password saving error interface */
    'Error_CantSavePassword_Title' => 'Erreur lors de l\'enregistrement du mot de passe.',
    'Error_CantSavePassword_Content' => 'V&eacute;rifiez que votre serveur web dispose des autorisations pour &eacute;crire les fichiers de configuration, avant de r&eacute;essayer.',

    /* Login interface */
    'Login_Title' => 'Connexion',
    'Login_Content' => '',
    'Login_Access' => 'Acc&eacute;der',
    'Login_ConnectionNotScured' => 'Attention : Votre connexion n\'est pas s&eacute;curis&eacute;e. Vous prenez un risque en vous connectant, votre mot de passe sera transmis en clair vers le serveur.',
);
?>
