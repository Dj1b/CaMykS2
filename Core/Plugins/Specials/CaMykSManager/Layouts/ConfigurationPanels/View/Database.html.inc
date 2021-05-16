<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view database section sublayout.
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
    <?= $this->get_locale('Configuration_DatabaseSettings'); ?>
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
            <?= $this->get_locale('Configuration_Driver'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Database_Driver') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_DriverUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Database_Driver'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Host'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Database_Host') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_HostUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Database_Host'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Port'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Database_Port') == 0) : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_PortAutomatic'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Database_Port'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_Secure'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Database_Secure') == 0) : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_SecureUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Database_Secure'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formElement half">
        <div class="formLabel">
            <?= $this->get_locale('Configuration_TablePrefix'); ?>
        </div>
        <?php if ($CaMykS->get_configValue('Database_TablePrefix') == '') : ?>
            <div class="formNoValue">
                <?= $this->get_locale('Configuration_TablePrefixUndefined'); ?>
            </div>
        <?php else : ?>
            <div class="formValue">
                <?= $CaMykS->get_configValue('Database_TablePrefix'); ?>
            </div>
        <?php endif; ?>
    </div>

    <div class="formButtons">
        <a class="formButton" href="<?= $this->get_actionLink('EditConfigurationPanel', array('Section'=>$this->section)); ?>">
            <?= $this->get_locale('Configuration_Edit'); ?>
        </a>
    </div>
</div>
