<?php
/**
 * @brief CaMykSMInstaller plugin, installation done layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<div class="mainFrame installer">
    <h1 class="title1">
        <?= $this->get_locale('TemplateHeaderTitle'); ?>
    </h1>

    <div class="clear">
        &nbsp;
    </div>

    <div class="centered">
        <img src="<?= $this->get_fileURL('Images/InstallComplete.svg'); ?>" alt="<?= $this->get_locale('Installation_Done_Title'); ?>" title="<?= $this->get_locale('Installation_Done_Title'); ?>" />
    </div>

    <div class="clear">
        &nbsp;
    </div>

    <div class="centered title2">
        <?= $this->get_locale('Installation_Done_Title'); ?>
    </div>

    <div class="centered content1 large">
        <?= $this->get_locale('Installation_Done_Message'); ?>
    </div>

    <div class="clear">
        &nbsp;
    </div>

    <div class="centered buttons">
        <?php if ($this->mediaPopupComponent !== false) : ?>
            <?php $this->theme->display_button(array(
                'Title' => $this->get_locale('Action_DisplayDetails'),
                'Link'  => 'javascript:MediaPopup.open_popup(\'DOMElement:IntallationManifest\');',
                'Size'  => 'Large',
                'Color' => 'Alternate'
            )); ?>
        <?php endif; ?>
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_AccessAdmin'),
            'Link'  => $this->adminLink,
            'Size'  => 'Large',
        )); ?>
    </div>

    <div class="clear">
        &nbsp;
    </div>
</div>

<div id="IntallationManifest" style="display:none;">
    <div class="title1 manifest">
        <img src="<?= $this->get_fileURL('Images/ManifestFile.svg'); ?>" alt="<?= $this->get_locale('Manifest_Title'); ?>" title="<?= $this->get_locale('Manifest_Title'); ?>" />
        <?= $this->get_locale('Manifest_Title'); ?>
    </div>
    <div class="clear">
        &nbsp;
    </div>
    <?php foreach($this->installationManifest as $manifestLine) : ?>
        <p class="content1 large manifest">
            <?= $manifestLine; ?>
        </p>
    <?php endforeach; ?>
</div>
