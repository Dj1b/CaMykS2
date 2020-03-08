<?php
/**
 * @brief CaMykSManager plugin, folder content sub layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<?php foreach($this->currentFolder['Files'] as $file => $fileInfo) : ?>
    <div class="listLine">
        <div class="cell icon">
            &nbsp;
        </div>
        <div class="cell icon">
            <img src="<?= $this->get_fileURL('Images/Main/File.png'); ?>" alt="<?= $this->get_locale('File'); ?>" title="<?= $this->get_locale('File'); ?>" />
        </div>
        <div class="cell name">
            <?= $file; ?>
        </div>
        <div class="cell description">
            &nbsp;
        </div>
    </div>
<?php endforeach; ?>

<?php foreach($this->currentFolder['Folders'] as $folder => $folderInfo) : ?>
    <div id="" class="listLine">
        <div class="cell icon">
            <?php if (count($folderInfo['Folders']) > 0) : ?>
                +
            <?php endif; ?>
        </div>
        <div class="cell icon">
            <img src="<?= $this->get_fileURL('Images/Main/'.$folderInfo['Type'].'.png'); ?>" alt="<?= $this->get_locale($folderInfo['Type']); ?>" title="<?= $this->get_locale($folderInfo['Type']); ?>" />
        </div>
        <div class="cell name">
            <?= $folder; ?>
        </div>
        <div class="cell description">
            <?= $this->get_locale($folderInfo['Type']); ?>
        </div>
    </div>
    <?php if (count($folderInfo['Folders']) > 0) : ?>
        <div class="children">
            <?= $this->display_folderContent($folderInfo, $this->currentLevel+1); ?>
        </div>
    <?php endif; ?>
<?php endforeach; ?>
