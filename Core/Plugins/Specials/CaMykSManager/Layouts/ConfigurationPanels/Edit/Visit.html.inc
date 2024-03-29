<?php
/**
 * @brief CaMykSManager plugin, configuration panel, edit visit section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Feb 2021
 * @date Modification: Jul 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_VisitSettings'); ?>
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
        <?= $this->configurationManager->get_locale('Languages'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Languages'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('DefaultLanguage'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'DefaultLanguage', 'default'=>$CaMykS->get_configValue('Visit_DefaultLanguage'), 'values'=>$this->availableDefaultLanguages, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('GraphicalAspect'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Theme'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Theme', 'default'=>$CaMykS->get_configValue('Visit_Theme'), 'values'=>$this->availableThemes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Template'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Template', 'default'=>$CaMykS->get_configValue('Visit_Template'), 'values'=>$this->availableTemplates, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('URLs'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('URLMode'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'URLMode', 'default'=>$CaMykS->get_configValue('Visit_URLMode'), 'values'=>$this->availableURLModes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('SSLMode'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'SSLMode', 'default'=>$CaMykS->get_configValue('Visit_SSLMode'), 'values'=>$this->availableSSLModes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('AdvancedProperties'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('ServerSideCache'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_booleanSelectInput(array('name'=>'ServerSideCache', 'default'=>$CaMykS->get_configValue('Visit_ServerSideCache'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('ClientSideCache'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_booleanSelectInput(array('name'=>'ClientSideCache', 'default'=>$CaMykS->get_configValue('Visit_ClientSideCache'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('AutoLoadCaMykSJS'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_booleanSelectInput(array('name'=>'AutoLoadCaMykSJS', 'default'=>$CaMykS->get_configValue('Visit_AutoLoadCaMykSJS'))); ?>
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
