<?php
/**
 * @brief CaMykSManager plugin, site list layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.3
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>

<div class="mainFrame">

    <?php $this->externalLinksForm->display_header(); ?>
    <?php $this->externalLinksForm->display_hiddens(); ?>
    <?php $this->externalLinksForm->display_footer(); ?>

    <div class="buttons rightFloated">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_ConfigureCaMykS2'),
            'Link'  => $this->get_actionLink('DisplayConfigurationPanel'),
        )); ?>
    </div>

    <h1 class="title1">
        <?= $this->get_locale('SiteList_ServerNameX', gethostname()); ?>
    </h1>

    <div class="siteList">
        <?php $this->display_folderContent($this->folderTree, 0); ?>
    </div>
</div>
