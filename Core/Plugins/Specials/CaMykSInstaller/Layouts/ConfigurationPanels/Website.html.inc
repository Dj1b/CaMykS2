<?php
/**
 * @brief CaMykSMInstaller plugin, website configuration sub layout.
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
    <?= $this->configurationManager->get_locale('WebsiteConfiguration'); ?>
</h2>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('Welcome'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Continue'); ?>
        </a>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Properties'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Title'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Title', 'default'=>$this->currentConfiguration['Website']['Website_Title'], 'class'=>'long')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Version'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Version', 'default'=>$this->currentConfiguration['Website']['Website_Version'], 'class'=>'')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Encoding'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Encoding', 'values'=>$this->encodingValues, 'default'=>$this->currentConfiguration['Website']['Website_Encoding'], 'class'=>'')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Author'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Author', 'default'=>$this->currentConfiguration['Website']['Website_Author'], 'class'=>'')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('AdvancedProperties'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('CMSSignature'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'DisplayCMSSignature', 'values'=>$this->CMSSignatureValues, 'default'=>$this->currentConfiguration['Website']['Website_DisplayCMSSignature'], 'class'=>'')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('LocalStorage'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'LocalStorage', 'values'=>$this->localStorageValues, 'default'=>$this->currentConfiguration['Website']['Website_LocalStorage'], 'class'=>'')); ?>
        </div>
    </div>

    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('Welcome'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Continue'); ?>
        </a>
    </div>
</div>
