<?php
/**
 * @brief CaMykSMInstaller plugin, configuration layout.
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
<div class="mainFrame configurationPanel">
    <h1 class="title1">
        <?= $this->get_locale('TemplateHeaderTitle'); ?>
    </h1>

    <div class="clear">
        &nbsp;
    </div>

    <?php $this->configurationForm->display_header(); ?>
    <?php $this->configurationForm->display_hiddens(); ?>

    <?php $this->load_layout('ConfigurationPanels/'.$this->section); ?>

    <?php $this->configurationForm->display_footer(); ?>
</div>
