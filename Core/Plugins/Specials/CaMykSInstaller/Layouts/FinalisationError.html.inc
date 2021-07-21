<?php
/**
 * @brief CaMykSMInstaller plugin, finalisation error layout.
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

     <div class="buttons">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_Retry'),
            'Link'  => $this->get_actionLink('FinaliseInstallation'),
            'Size'  => 'Large',
        )); ?>
    </div>
</div>
