<?php
/**
 * @brief CaMykSMInstaller plugin, database configuration sub layout.
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
    <?= $this->configurationManager->get_locale('DatabaseConfiguration'); ?>
</h2>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayAdminConfiguration'); ?>">
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
            <?= $this->configurationManager->get_locale('Driver'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Driver', 'default'=>$this->currentConfiguration['Database']['Database_Driver'], 'values'=>$this->databaseDrivers)); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Host'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Host', 'default'=>$this->currentConfiguration['Database']['Database_Host'], 'placeholder'=>$CaMykS->get_configValue('Database_Host'), 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Login'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Login', 'default'=>$this->currentConfiguration['Database']['Database_Login'], 'placeholder'=>$CaMykS->get_configValue('Database_Login'), 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Password'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_passwordInput(array('name'=>'Password', 'default'=>$this->currentConfiguration['Database']['Database_Password'], 'placeholder'=>$CaMykS->get_configValue('Database_Password') != '' ? '******' : '')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Name'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Name', 'default'=>$this->currentConfiguration['Database']['Database_Name'], 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('AdvancedProperties'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Port'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_numberInput(array('name'=>'Port', 'default'=>$this->currentConfiguration['Database']['Database_Port'], 'placeholder'=>$CaMykS->get_configValue('Database_Port'), 'class'=>'shortest', 'min'=>0, 'max'=>65536)); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Secure'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('TablePrefix'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'TablePrefix', 'default'=>$this->currentConfiguration['Database']['Database_TablePrefix'], 'placeholder'=>$CaMykS->get_configValue('Database_TablePrefix'), 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayAdminConfiguration'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Continue'); ?>
        </a>
    </div>
</div>
