<?php
/**
 * @brief CaMykSManager plugin, configuration panel layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Mar 2020
 * @date Modification: Feb 2021
 * @copyright 2020 - 2021 CaMykS Team
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

    <h2 class="title2">
        Configuration
    </h2>

    <div>
        &nbsp;
    </div>

    <div class="columns">
        <div class="column buttonList">
            <?php foreach ($this->configurationSections as $section) : ?>
                <a href="<?= $this->get_actionLink('DisplayConfigurationPanel', array('Section'=>$section)); ?>" class="configButton">
                    <img src="<?= $this->get_fileURL('Images/ConfigurationPanel/'.$section.'.png'); ?>" alt="<?= $this->get_locale('Configuration_'.$section.'Settings'); ?>" />
                    <?= $this->get_locale('Configuration_'.$section.'Settings'); ?>
                </a>
            <?php endforeach; ?>
        </div>

        <div class="column configurationForm">
            <?php $this->load_layout('ConfigurationPanels/'.$this->subAction.'/'.$this->section); ?>
        </div>
    </div>
</div>
