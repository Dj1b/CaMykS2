<?php
/**
 * @brief CaMykSManager plugin, folder content sub layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Mar 2020
 * @date Modification: Dec 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;

$currentLevel = $this->currentLevel;

?>
<?php foreach($this->currentFolder['Files'] as $file => $fileInfo) : ?>
    <div class="listLine">
        <div class="cell opener">
            &nbsp;
        </div>
        <div class="cell icon">
            <img src="<?= $this->get_fileURL('Images/SiteList/File.png'); ?>" alt="<?= $this->get_locale('File'); ?>" title="<?= $this->get_locale('File'); ?>" />
        </div>
        <div class="cell name">
            <?= $file; ?>
        </div>
        <div class="cell description">
            <?= $this->get_fileDescription($fileInfo['Path']); ?>
        </div>
        <div class="cell action">
            &nbsp;
        </div>
        <div class="cell action">
            &nbsp;
        </div>
        <div class="cell action">
            &nbsp;
        </div>
    </div>
<?php endforeach; ?>

<?php foreach($this->currentFolder['Folders'] as $folder => $folderInfo) : ?>
    <div id="Folder_<?= $folderInfo['Id']; ?>" class="listLine<?= $folderInfo['Class'] != '' ? ' '.$folderInfo['Class'] : ''; ?>">
        <div class="cell opener" onclick="SiteList.toggle_folderContentDisplay('Folder_<?= $folderInfo['Id']; ?>');">
            <?php if (count($folderInfo['Folders']) > 0) : ?>
                <span class="openButton">
                +
                </span>
                <span class="closeButton">
                -
                </span>
            <?php endif; ?>
        </div>
        <div class="cell icon">
            <img src="<?= $this->get_fileURL('Images/SiteList/'.$folderInfo['Type'].'.png'); ?>" alt="<?= $this->get_locale($folderInfo['Type']); ?>" title="<?= $this->get_locale($folderInfo['Type']); ?>" />
        </div>
        <div class="cell name">
            <?= $folder; ?>
        </div>
        <div class="cell description">
            <?= $this->get_locale($folderInfo['Type']); ?>
        </div>
        <?php if ($folderInfo['Type'] == 'Folder' and $currentLevel < $CaMykS->get_configValue('Manager_SiteListSearchLevel')) : ?>
            <div class="cell action">
                <a href="javascript:SiteList.open_newCaMykS2SitePromptPopup('<?= $folder; ?>');" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/AddSite.png'); ?>" alt="<?= $this->get_locale('SiteList_NewCaMykSv2WebsiteInX', $folder); ?>" title="<?= $this->get_locale('SiteList_NewCaMykSv2WebsiteInX', $folder); ?>" />
                </a>
            </div>
            <div class="cell action">
                <a href="javascript:SiteList.open_newFolderPromptPopup('<?= $folder; ?>');" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/AddFolder.png'); ?>" alt="<?= $this->get_locale('SiteList_NewFolderInX', $folder); ?>" title="<?= $this->get_locale('SiteList_NewFolderInX', $folder); ?>" />
                </a>
            </div>
            <div class="cell action">
                &nbsp;
            </div>

        <?php elseif ($folderInfo['Type'] == 'CaMykSv2Site') : ?>
            <div class="cell action">
                <a href="#" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/View.png'); ?>" alt="<?= $this->get_locale('SiteList_VisitWebsite'); ?>" title="<?= $this->get_locale('SiteList_VisitWebsite'); ?>" />
                </a>
            </div>
            <div class="cell action">
                <a href="#" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/Admin.png'); ?>" alt="<?= $this->get_locale('SiteList_AccessWebsiteAdmin'); ?>" title="<?= $this->get_locale('SiteList_AccessWebsiteAdmin'); ?>" />
                </a>
            </div>
            <div class="cell action">
                <a href="#" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/Configuration.png'); ?>" alt="" title="" />
                </a>
            </div>
        <?php elseif ($folderInfo['Type'] == 'CaMykSv1Site') : ?>
            <div class="cell action">
                <a href="javascript:SiteList.open_externalLink('OpenSiteVisit', '<?= $folderInfo['Path']; ?>');" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/View.png'); ?>" alt="<?= $this->get_locale('SiteList_VisitWebsite'); ?>" title="<?= $this->get_locale('SiteList_VisitWebsite'); ?>" />
                </a>
            </div>
            <div class="cell action">
                <a href="javascript:SiteList.open_externalLink('OpenSiteAdmin', '<?= $folderInfo['Path']; ?>');" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/Admin.png'); ?>" alt="<?= $this->get_locale('SiteList_AccessWebsiteAdmin'); ?>" title="<?= $this->get_locale('SiteList_AccessWebsiteAdmin'); ?>" />
                </a>
            </div>
            <div class="cell action">
                <a href="javascript:SiteList.toggle_websiteInfoPanelDisplay();" class="img">
                    <img src="<?= $this->get_fileURL('Images/SiteList/Info.png'); ?>" alt="<?= $this->get_locale('SiteList_ViewWebsiteInfo'); ?>" title="<?= $this->get_locale('SiteList_ViewWebsiteInfo'); ?>" />
                </a>
            </div>
        <?php else : ?>
            <div class="cell action">
                &nbsp;
            </div>
            <div class="cell action">
                &nbsp;
            </div>
            <div class="cell action">
                &nbsp;
            </div>
        <?php endif; ?>
    </div>
    <?php if (count($folderInfo['Folders']) > 0) : ?>
        <div class="children">
            <?php $this->display_folderContent($folderInfo, $currentLevel + 1); ?>
        </div>
    <?php endif; ?>
<?php endforeach; ?>
