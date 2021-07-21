<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view visit section sublayout.
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
    <?= $this->get_locale('Configuration_VisitSettings'); ?>
</h3>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('Languages'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Languages'); ?>
        </div>
        <?php if (!is_array($CaMykS->get_configValue('Visit_Languages')) or count($CaMykS->get_configValue('Visit_Languages')) == 0) : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_LanguagesUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?php foreach ($CaMykS->get_configValue('Visit_Languages') as $language) : ?>
                    <?= language_getLocalisedTitle($language); ?> <br />
                <?php endforeach; ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('DefaultLanguage'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Visit_DefaultLanguage') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_DefaultLanguageAutomatic'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= language_getLocalisedTitle($CaMykS->get_configValue('Visit_DefaultLanguage')); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('GraphicalAspect'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Theme'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Visit_Theme') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_ThemeUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Visit_Theme'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('Template'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Visit_Template') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_TemplateUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Visit_Template'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->configurationManager->get_locale('URLs'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('URLMode'); ?>
        </div>
        <div class="formValue">

        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('SSLMode'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_SSLMode') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_SSLUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Visit_SSLMode'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('AdvancedProperties'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('ServerSideCache'); ?>
        </div>
        <div class="formValue">
            <?= $CaMykS->get_locale($CaMykS->get_configValue('Visit_ServerSideCache') ? 'Yes' : 'No'); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('ClientSideCache'); ?>
        </div>
        <div class="formValue">
            <?= $CaMykS->get_locale($CaMykS->get_configValue('Visit_ClientSideCache') ? 'Yes' : 'No'); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->configurationManager->get_locale('AutoLoadCaMykSJS'); ?>
        </div>
        <div class="formValue">
            <?= $CaMykS->get_locale($CaMykS->get_configValue('Visit_AutoLoadCaMykSJS') ? 'Yes' : 'No'); ?>
        </div>
    </div>

    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>
</div>
