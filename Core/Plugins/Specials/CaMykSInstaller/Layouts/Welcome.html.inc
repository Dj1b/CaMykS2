<?php
/**
 * @brief CaMykSMInstaller plugin, Welcome layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jun 2021
 * @date Modification: Jun 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<div class="loginFrame">
     <div class="title">
        <?= $this->get_locale('Welcome_Title'); ?>
    </div>

    <div class="buttons">
        <img src="<?= $this->welcomePicture->get_base64Data(); ?>" alt="<?= CAMYKS_NAME; ?>" />
    </div>

    <div class="">
        &nbsp;
    </div>

    <div class="buttons">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_StartInstallation'),
            'Link'  => $this->get_actionLink('DisplayWebsiteConfiguration'),
            'Size'  => 'Large',
        )); ?>
    </div>

    <?php if (!client_isHTTPS()) : ?>
        <div class="warning">
            <?= $this->get_locale('Welcome_ConnectionNotSecured'); ?>
        </div>
    <?php endif; ?>
</div>
