<?php
/**
 * @brief CaMykSManager plugin, home layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<div class="homeFrame">
    <div class="buttonsPanel">
        <?php foreach ($this->panelButtons as $button) : ?>
            <a href="<?= $button['Link']; ?>" class="button">
                <img src="<?= $button['Icon']; ?>" alt="<?= $button['Title']; ?>" title="<?= $button['Title']; ?>" />
                <br />
                <?= $button['Title']; ?>
            </a>
        <?php endforeach; ?>
    </div>
</div>
