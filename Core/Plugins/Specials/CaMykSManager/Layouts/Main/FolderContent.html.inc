<?php
/**
 * @brief CaMykSManager plugin, folder content sub layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<?php foreach($this->currentFolder['Files'] as $file => $fileInfo) : ?>
    <div class="listLine">
        <div class="cell level<?= $this->currentLevel; ?>">
            <img src="<?= $this->get_fileURL('Images/Main/File.png'); ?>" alt="<?= $this->get_locale('File'); ?>" title="<?= $this->get_locale('File'); ?>" />
            <?= $file; ?>
        </div>
        <div class="cell description">
            &nbsp;
        </div>
    </div>
<?php endforeach; ?>

<?php foreach($this->currentFolder['Folders'] as $folder => $folderInfo) : ?>
    <div class="listLine">
        <div class="cell name level<?= $this->currentLevel; ?>">
            <img src="<?= $this->get_fileURL('Images/Main/'.$folderInfo['Type'].'.png'); ?>" alt="<?= $this->get_locale($folderInfo['Type']); ?>" title="<?= $this->get_locale($folderInfo['Type']); ?>" />
            <?= $folder; ?>
        </div>
        <div class="cell description">
            <?= $this->get_locale($folderInfo['Type']); ?>
        </div>
    </div>
<?php endforeach; ?>
