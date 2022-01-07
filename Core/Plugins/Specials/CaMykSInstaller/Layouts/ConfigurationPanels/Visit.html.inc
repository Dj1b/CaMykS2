<?php
/**
 * @brief CaMykSMInstaller plugin, visit configuration sub layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jun 2021
 * @date Modification: Jan 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h2 class="title3">
    <?= $this->configurationManager->get_locale('VisitConfiguration'); ?>
</h2>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayWebsiteConfiguration'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Continue'); ?>
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
            <?php $this->configurationForm->display_selectInput(array('name'=>'DefaultLanguage', 'default'=>$this->get_currentConfigurationValue('Visit', 'DefaultLanguage'), 'values'=>$this->availableLanguages, 'class'=>'longest')); ?>
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
            <?php $this->configurationForm->display_selectInput(array('name'=>'Theme', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_Theme'), 'values'=>$this->availableThemes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Template'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Template', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_Template'), 'values'=>$this->availableTemplates, 'class'=>'longest')); ?>
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
            <?php $this->configurationForm->display_selectInput(array('name'=>'URLMode', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_URLMode'), 'values'=>$this->availableURLModes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('SSLMode'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'SSLMode', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_SSLMode'), 'values'=>$this->availableSSLModes, 'class'=>'longest')); ?>
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
            <?php $this->configurationForm->display_booleanSelectInput(array('name'=>'ServerSideCache', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_ServerSideCache'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('ClientSideCache'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_booleanSelectInput(array('name'=>'ClientSideCache', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_ClientSideCache'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('AutoLoadCaMykSJS'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_booleanSelectInput(array('name'=>'AutoLoadCaMykSJS', 'default'=>$this->get_currentConfigurationValue('Visit', 'Visit_AutoLoadCaMykSJS'))); ?>
        </div>
    </div>

    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayWebsiteConfiguration'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Continue'); ?>
        </a>
    </div>
</div>

