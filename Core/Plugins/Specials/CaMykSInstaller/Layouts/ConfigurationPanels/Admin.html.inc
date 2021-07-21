<?php
/**
 * @brief CaMykSMInstaller plugin, admin configuration sub layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jun 2021
 * @date Modification: Jun 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h2 class="title3">
    <?= $this->configurationManager->get_locale('AdminConfiguration'); ?>
</h2>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayVisitConfiguration'); ?>">
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
            <?= $this->configurationManager->get_locale('EditionLanguages'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('DefaultLanguage'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'DefaultLanguage', 'default'=>$this->currentConfiguration['Admin']['Admin_DefaultLanguage'], 'values'=>$this->availableLanguages, 'class'=>'longest')); ?>
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
            <?php $this->configurationForm->display_selectInput(array('name'=>'Theme', 'default'=>$this->currentConfiguration['Admin']['Admin_Theme'], 'values'=>$this->availableThemes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('URLs'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('EntryPoint'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'EntryPoint', 'default'=>$this->currentConfiguration['Admin']['Admin_EntryPoint'], 'placeholder'=>$CaMykS->get_configValue('Admin_EntryPoint'), 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('SSLMode'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'SSLMode', 'default'=>$this->currentConfiguration['Admin']['Admin_SSLMode'], 'values'=>$this->availableSSLModes, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('AdvancedProperties'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('KeepAlive'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('SaveMode'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayVisitConfiguration'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Continue'); ?>
        </a>
    </div>
</div>
