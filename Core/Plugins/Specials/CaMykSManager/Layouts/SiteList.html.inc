<?php
/**
 * @brief CaMykSManager plugin, site list layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre8
 * @date Creation: Mar 2020
 * @date Modification: Jan 2022
 * @copyright 2020 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<div class="mainFrame">
    <?php $this->externalLinksForm->display_header(); ?>
    <?php $this->externalLinksForm->display_hiddens(); ?>
    <?php $this->externalLinksForm->display_footer(); ?>

    <?php $this->newFolderForm->display_header(); ?>
    <?php $this->newFolderForm->display_hiddens(); ?>
    <?php $this->newFolderForm->display_footer(); ?>

    <?php $this->newC2SiteForm->display_header(); ?>
    <?php $this->newC2SiteForm->display_hiddens(); ?>
    <?php $this->newC2SiteForm->display_footer(); ?>

    <h1 class="title1">
        <img src="<?= $this->get_fileURL('Images/Home/SiteList.svg'); ?>" alt="<?= $this->get_locale('SiteList_ServerNameX', gethostname()); ?>" />
        <?= $this->get_locale('SiteList_ServerNameX', gethostname()); ?>
    </h1>

    <div class="siteList">
        <?php $this->display_folderContent($this->folderTree, 0); ?>
    </div>
</div>

<div id="WebsiteInfoPanel">
    <div class="buttons">
        <a href="javascript:SiteList.toggle_websiteInfoPanelDisplay();" class="button">
            <img src="<?= $this->get_fileURL('Images/SiteList/Close.svg'); ?>" alt="<?= $this->get_locale('Action_Close'); ?>" />
        </a>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('SiteList_InfoPanel_GeneralInfo'); ?>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Title'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Version'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_URL'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Engine'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_InstallationMode'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Status'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Author'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('SiteList_InfoPanel_VisitMode'); ?>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_SSL'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Languages'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('SiteList_InfoPanel_AdminMode'); ?>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_SSL'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_EntryPoint'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_Languages'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_EditionLanguages'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('SiteList_InfoPanel_Database'); ?>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_DatabaseDriver'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_DatabaseName'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_DatabaseServer'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_DatabaseUser'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>
    <div class="formLine">
        <div class="formLabel">
            <?= $this->get_locale('SiteList_InfoPanel_DatabasePrefix'); ?>
        </div>
        <div class="formValue">
            xxxx xxxxx yyy
        </div>
    </div>

</div>
