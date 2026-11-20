<?php
/**
 * @brief CaMykSInstaller plugin, finalisation error layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Jun 2021
 * @date Modification: Nov 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<div class="mainFrame installer">
    <h1 class="title1">
        <?= $this->get_locale('TemplateHeaderTitle'); ?>
    </h1>

    <div class="clear">
        &nbsp;
    </div>

    <div class="centered title2">
        <?= $this->get_locale('Installation_Error_Title'); ?>
    </div>

    <div class="centered content1 large">
        <?= $this->get_locale('Installation_Error_Message'); ?>
    </div>

    <div class="clear">
        &nbsp;
    </div>

    <div class="centered buttons">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Back'),
            'Link'  => $this->get_actionLink('DisplayWebmasterConfiguration'),
            'Size'  => 'Large',
            'Color' => 'Alternate',
        )); ?>
        <?php if ($this->mediaPopupComponent !== false and !empty($this->installationManifest)) : ?>
            <?php $this->theme->display_button(array(
                'Title' => $this->get_locale('Action_DisplayDetails'),
                'Link'  => 'javascript:MediaPopup.open_popup(\'DOMElement:IntallationManifest\');',
                'Size'  => 'Large',
                'Color' => 'Alternate',
            )); ?>
        <?php endif; ?>
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_Retry'),
            'Link'  => $this->get_actionLink('FinaliseInstallation'),
            'Size'  => 'Large',
        )); ?>
    </div>

    <div class="clear">
        &nbsp;
    </div>
</div>

<?php if ($this->mediaPopupComponent !== false and !empty($this->installationManifest)) : ?>
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
<?php endif; ?>
