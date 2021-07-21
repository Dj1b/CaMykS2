<?php
/**
 * @brief CaMykSMInstaller plugin, webmaster configuration sub layout.
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
    <?= $this->get_locale('WebmasterConfiguration'); ?>
</h2>

<div class="formBlock">
    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayDatabaseConfiguration'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Action_PerformInstallation'); ?>
        </a>
    </div>


    <div class="formButtons">
        <a class="formButton Back" href="<?= $this->get_actionLink('DisplayDatabaseConfiguration'); ?>">
            <?= $this->get_locale('Back'); ?>
        </a>
        <a class="formButton" href="<?= $this->configurationForm->get_HTMLSubmitLink(); ?>">
            <?= $this->get_locale('Action_PerformInstallation'); ?>
        </a>
    </div>
</div>

