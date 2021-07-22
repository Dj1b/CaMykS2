<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view engine section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Sep 2020
 * @date Modification: Jul 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_EngineSettings'); ?>
</h3>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_Parameters'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_TimeZone'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Engine_TimeZone') == '') : ?>
            <div class="formNoValue">
                <?= $this->configurationManager->get_locale('Configuration_TimeZoneUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Engine_TimeZone'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_WebmasterEmail'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Engine_WebmasterEmail') == '') : ?>
            <div class="formNoValue">
                <?= $this->configurationManager->get_locale('Configuration_WebmasterEmailUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Engine_WebmasterEmail'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Configuration_ErrorLogging'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_ErrorLoggingLevel'); ?>
        </div>
        <?php if (!isset($this->errorLoggingLevels[$CaMykS->get_configValue('Engine_ErrorLoggingLevel')])) : ?>
            <div class="formNoValue">
                <?= $this->configurationManager->get_locale('Configuration_ErrorLoggingLevelUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $this->errorLoggingLevels[$CaMykS->get_configValue('Engine_ErrorLoggingLevel')]; ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_ErrorLoggingOutput'); ?>
        </div>
        <?php if (!isset($this->errorLoggingOutputs[$CaMykS->get_configValue('Engine_ErrorLoggingOutput')])) : ?>
            <div class="formNoValue">
                <?= $this->configurationManager->get_locale('Configuration_ErrorLoggingOutputUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $this->errorLoggingOutputs[$CaMykS->get_configValue('Engine_ErrorLoggingOutput')]; ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Configuration_DefaultTools'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_PictureEditor'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Engine_PictureEditor') == '') : ?>
            <div class="formNoValue">
                <?= $this->configurationManager->get_locale('Configuration_PictureEditorUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Engine_PictureEditor'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Configuration_Requests'); ?>
    </div>

    <div class="formElement">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Configuration_RequestUserAgent'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Engine_RequestUserAgent') == '') : ?>
            <div class="formNoValue">
                <?= $this->configurationManager->get_locale('Configuration_RequestUserAgentUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Engine_RequestUserAgent'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>
</div>
