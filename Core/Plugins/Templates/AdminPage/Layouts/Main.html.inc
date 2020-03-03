<?php
/**
 * @brief AdminPage Template main layout.
 * @details Plugin / Template layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Feb 2020
 * @date Modification: Feb 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<div id="PageMain">
    <div id="PageHeader">
        <div class="background">
            <div class="innerContent">
                <div class="infos">
                    &nbsp;
                </div>
                <div class="title">
                    <?= $this->get_headerTitle(); ?>
                </div>
                <?php if ($this->has_logo()) : ?>
                <div class="logo">
                    &nbsp;
                </div>
                <?php endif; ?>
            </div>
        </div>
    </div>
    <div id="PageContent">
        <div class="innerContent">
            <?= $this->display_slotContent(1); ?>
        </div>
    </div>
    <div id="PageFooter">
        <div class="innerContent">
            CaMykS version <?= $CaMykS->version; ?>
        </div>
    </div>
</div>
