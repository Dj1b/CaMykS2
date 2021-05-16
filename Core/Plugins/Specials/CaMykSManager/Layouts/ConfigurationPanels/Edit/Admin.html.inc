<?php
/**
 * @brief CaMykSManager plugin, configuration panel, edit admin section sublayout.
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
    <?= $this->get_locale('Configuration_AdminSettings'); ?>
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
        <?= $this->get_locale('Configuration_Languages'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Languages'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_EditionLanguages'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_DefaultLanguage'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_GraphicalAspect'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Theme'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Theme', 'default'=>$CaMykS->get_configValue('Admin_Theme'), 'values'=>$this->availableThemes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_URLs'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_EntryPoint'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'EntryPoint', 'default'=>$CaMykS->get_configValue('Admin_EntryPoint'), 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SSLMode'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'SSLMode', 'default'=>$CaMykS->get_configValue('Admin_SSLMode'), 'values'=>$this->availableSSLModes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_AdvancedParameters'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_KeepAlive'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SaveMode'); ?>
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
