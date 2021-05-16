<?php
/**
 * @brief CaMykSManager plugin, configuration panel, edit manager section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_ManagerSettings'); ?>
</h3>

<?php $this->configurationForm->display_header(); ?>
<?php $this->configurationForm->display_hiddens(); ?>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Cancel'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Save'); ?>
        </a>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_Parameters'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_DefaultLanguage'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'DefaultLanguage', 'default'=>$CaMykS->get_configValue('Manager_DefaultLanguage'), 'values'=>$this->availableLanguages)); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Theme'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Theme', 'default'=>$CaMykS->get_configValue('Manager_Theme'), 'values'=>$this->availableThemes)); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_SiteList'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListPath'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'SiteListPath', 'default'=>$CaMykS->get_configValue('Manager_SiteListPath'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListSearchLevel'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_numberInput(array('name'=>'SiteListSearchLevel', 'default'=>$CaMykS->get_configValue('Manager_SiteListSearchLevel'), 'class'=>'shortest', 'min'=>1, 'max'=>5)); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListHiddenFiles'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListHiddenFolders'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_Security'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SSLEnabled'); ?>
        </div>
        <div class="formInput">
            <?php  $this->configurationForm->display_toggleSwitchInput(array('name'=>'SSLEnabled', 'value'=>4, 'offValue'=>2, 'default'=>$CaMykS->get_configValue('Manager_SSLEnabled'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_RestrainedIPs'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Cancel'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Save'); ?>
        </a>
    </div>
</div>

<?php $this->configurationForm->display_footer(); ?>

