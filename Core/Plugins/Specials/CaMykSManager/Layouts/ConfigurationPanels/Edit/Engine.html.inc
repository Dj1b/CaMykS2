<?php
/**
 * @brief CaMykSManager plugin, configuration panel, edit engine section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Feb 2021
 * @date Modification: Jul 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_EngineSettings'); ?>
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
            <?= $this->configurationManager->get_locale('Configuration_TimeZone'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'TimeZone', 'default'=>$CaMykS->get_configValue('Engine_TimeZone'), 'values'=>$this->timeZoneLocations, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_WebmasterEmail'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'WebmasterEmail', 'default'=>$CaMykS->get_configValue('Engine_WebmasterEmail'), 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Configuration_ErrorLogging'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_ErrorLoggingLevel'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'ErrorLoggingLevel', 'default'=>$CaMykS->get_configValue('Engine_ErrorLoggingLevel'), 'values'=>$this->errorLoggingLevels, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_ErrorLoggingOutput'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'ErrorLoggingOutput', 'default'=>$CaMykS->get_configValue('Engine_ErrorLoggingOutput'), 'values'=>$this->errorLoggingOutputs, 'class'=>'longest')); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Configuration_DefaultTools'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_PictureEditor'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_selectInput(array('name'=>'PictureEditor', 'default'=>$CaMykS->get_configValue('Engine_PictureEditor'), 'values'=>$this->imageEditorEngines)); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Configuration_Requests'); ?>
    </div>

    <div class="formElement">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_RequestUserAgent'); ?>
        </div>
        <div class="formInput">
            <?php $this->configurationForm->display_textInput(array('name'=>'RequestUserAgent', 'default'=>$CaMykS->get_configValue('Engine_RequestUserAgent'), 'class'=>'longest', 'maxlength'=>256)); ?>
        </div>
    </div>

    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->configurationManager->get_locale('Cancel'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Save'); ?>
        </a>
    </div>
</div>

<?php $this->configurationForm->display_footer(); ?>
