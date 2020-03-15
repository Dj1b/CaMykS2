<?php
/**
 * @brief CaMykSManager plugin, configuration panel layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>

<div class="mainFrame configurationPanel">
    <div class="buttons rightFloated">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_BackToSiteList'),
            'Link'  => $this->get_actionLink('SiteList'),
        )); ?>
    </div>

    <h1 class="title1">
        <?= $this->get_locale('SiteList_ServerNameX', gethostname()); ?>
    </h1>

    <div class="clear">
        &nbsp;
    </div>

    <div class="columns">
        <div class="column half">
            <h2 class="title2">
                Configuration
            </h2>

            <h3 class="title3">
                <?= $this->get_locale('TemplateHeaderTitle'); ?>
            </h3>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/Manager.png'); ?>" alt="<?= $this->get_locale('Configuration_ManagerSettings'); ?>" />
                <?= $this->get_locale('Configuration_ManagerSettings'); ?>
            </a>

            <div class="">
                &nbsp;
            </div>

            <h3 class="title3">
                <?= $this->get_locale('Configuration_WebsitesSettings'); ?>
            </h3>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/Engine.png'); ?>" alt="<?= $this->get_locale('Configuration_EngineSettings'); ?>" />
                <?= $this->get_locale('Configuration_EngineSettings'); ?>
            </a>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/Visit.png'); ?>" alt="<?= $this->get_locale('Configuration_VisitSettings'); ?>" />
                <?= $this->get_locale('Configuration_VisitSettings'); ?>
            </a>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/Admin.png'); ?>" alt="<?= $this->get_locale('Configuration_AdminSettings'); ?>" />
                <?= $this->get_locale('Configuration_AdminSettings'); ?>
            </a>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/Database.png'); ?>" alt="<?= $this->get_locale('Configuration_DatabaseSettings'); ?>" />
                <?= $this->get_locale('Configuration_DatabaseSettings'); ?>
            </a>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/GDPR.png'); ?>" alt="<?= $this->get_locale('Configuration_GDPRSettings'); ?>" />
                <?= $this->get_locale('Configuration_GDPRSettings'); ?>
            </a>

            <a href="#" class="configButton">
                <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/Plugins.png'); ?>" alt="<?= $this->get_locale('Configuration_PluginsSettings'); ?>" />
                <?= $this->get_locale('Configuration_PluginsSettings'); ?>
            </a>

        </div>
        <div class="column half">
            <h2 class="title2">
                CaMykS
            </h2>

            <p class="content1">
                <?= $this->get_locale('Configuration_CurrentVersion'); ?> <?= $CaMykS->version; ?> <br />
                <?= $this->get_locale('Configuration_LatestVersion'); ?> <?= $CaMykS->version; ?> <br />
            </p>

            <div class="buttons">
                <?php $this->theme->display_button(array(
                    'Title' => $this->get_locale('Action_Update'),
                    'Link'  => '#',
                    'Size'  => 'Small',
                )); ?>

                <?php $this->theme->display_button(array(
                    'Title' => $this->get_locale('Action_CheckIntegrity'),
                    'Link'  => '#',
                    'Size'  => 'Small',
                )); ?>
            </div>

            <div class="">
                &nbsp;
            </div>

            <h2 class="title2">
                <?= $this->get_locale('Configuration_Server'); ?>
            </h2>

            <?php $server = server_getSoftware(); ?>
            <ul>
                <li>
                    <?= $server['Name'].' '.$server['Version']; ?>
                </li>
                <li>
                    <?= $this->get_locale('Configuration_PHP'); ?> <?= phpversion(); ?>
                </li>
            </ul>

            <div class="">
                &nbsp;
            </div>

            <?= $this->get_locale('Configuration_AvailableDatabaseDrivers'); ?>
            <ul>
                <?php foreach ($this->dbDrivers as $driver) : ?>
                <li>
                    <?= $driver; ?>
                </li>
                <?php endforeach; ?>
            </ul>
        </div>
    </div>
</div>
