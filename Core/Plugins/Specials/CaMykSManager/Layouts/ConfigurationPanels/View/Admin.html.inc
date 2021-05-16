<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view admin section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Sep 2020
 * @date Modification: May 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_AdminSettings'); ?>
</h3>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_Languages'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Languages'); ?>
        </div>
        <?php if (!is_array($CaMykS->get_configValue('Admin_Languages')) or count($CaMykS->get_configValue('Admin_Languages')) == 0) : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_LanguagesUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?php foreach ($CaMykS->get_configValue('Admin_Languages') as $language) : ?>
                    <?= language_getLocalisedTitle($language); ?> <br />
                <?php endforeach; ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_EditionLanguages'); ?>
        </div>
        <?php if (!is_array($CaMykS->get_configValue('Admin_EditionLanguages')) or count($CaMykS->get_configValue('Admin_EditionLanguages')) == 0) : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_LanguagesUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?php foreach ($CaMykS->get_configValue('Admin_EditionLanguages') as $language) : ?>
                    <?= language_getLocalisedTitle($language); ?> <br />
                <?php endforeach; ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_DefaultLanguage'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_DefaultLanguage') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_DefaultLanguageAutomatic'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= language_getLocalisedTitle($CaMykS->get_configValue('Admin_DefaultLanguage')); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_GraphicalAspect'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Theme'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_Theme') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_ThemeUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Admin_Theme'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_URLs'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_EntryPoint'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_EntryPoint') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_EntryPointUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Admin_EntryPoint'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SSLMode'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_DefaultLanguage') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_SSLUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Admin_SSLMode'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_AdvancedParameters'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_KeepAlive'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_DefaultLanguage') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_KeepAliveUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Admin_KeepAlive'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SaveMode'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Admin_DefaultLanguage') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_SaveModeUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Admin_SaveMode'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>
</div>
