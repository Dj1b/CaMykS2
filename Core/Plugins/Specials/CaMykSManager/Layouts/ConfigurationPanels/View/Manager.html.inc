<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view manager section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Sep 2020
 * @date Modification: Nov 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_ManagerSettings'); ?>
</h3>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Secondary" href="<?= $this->get_actionLink('EditManagerPassword'); ?>">
            <?= $this->get_locale('Configuration_EditManagerPassword'); ?>
        </a>
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_Parameters'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Language'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Manager_DefaultLanguage') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_DefaultLanguageAutomatic'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= language_getLocalisedLongTitle($CaMykS->get_configValue('Manager_DefaultLanguage')); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Theme'); ?>
        </div>
        <div class="formValue">
            <?= $CaMykS->get_configValue('Manager_Theme'); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_SiteList'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListPath'); ?>
        </div>
        <div class="formValue">
            <?= $CaMykS->get_configValue('Manager_SiteListPath'); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListSearchLevel'); ?>
        </div>
        <div class="formValue">
            <?= $CaMykS->get_configValue('Manager_SiteListSearchLevel'); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListHiddenFiles'); ?>
        </div>
        <div class="formNoValue">
            <?= $this->get_locale($CaMykS->get_configValue('Manager_SiteListHideHiddenFiles') == 1 ? 'No' : 'Yes'); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SiteListHiddenFolders'); ?>
        </div>
        <div class="formNoValue">
            <?= $this->get_locale($CaMykS->get_configValue('Manager_SiteListHideHiddenFolders') == 1 ? 'No' : 'Yes'); ?>
        </div>
    </div>

    <div class="formTitle">
        <?= $this->get_locale('Configuration_Security'); ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_SSLEnabled'); ?>
        </div>
        <div class="formValue">
            <?= $this->get_locale($CaMykS->get_configValue('Manager_SSLEnabled') == 1 ? 'Yes' : 'No'); ?>
        </div>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_RestrainedIPs'); ?>
        </div>
        <?php $list = $CaMykS->get_configValue('Manager_RestrainedIPs'); ?>
        <?php if (!is_array($list) or count($list) == 0) : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_RestrainedIPsNone'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= implode('<br />', $list); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formButtons">
        <a class="formButton Secondary" href="<?= $this->get_actionLink('EditManagerPassword'); ?>">
            <?= $this->get_locale('Configuration_EditManagerPassword'); ?>
        </a>
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>
</div>
