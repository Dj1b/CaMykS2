<?php
/**
 * @brief CaMykSManager plugin, configuration panel, edit database section sublayout.
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
    <?= $this->get_locale('Configuration_DatabaseSettings'); ?>
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
            <?= $this->get_locale('Configuration_Driver'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'Driver', 'default'=>$CaMykS->get_configValue('Database_Driver'), 'values'=>$this->databaseDrivers)); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Host'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'Host', 'default'=>$CaMykS->get_configValue('Database_Host'))); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Port'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_numberInput(array('name'=>'Port', 'default'=>$CaMykS->get_configValue('Database_Port'), 'class'=>'shortest', 'min'=>0, 'max'=>65536)); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Secure'); ?>
        </div>
        <div class="formInput">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_TablePrefix'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'TablePrefix', 'default'=>$CaMykS->get_configValue('Database_TablePrefix'))); ?>
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
