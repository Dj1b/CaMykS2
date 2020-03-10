<?php
/**
 * @brief CaMykSManager plugin, message layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
?>
<div class="loginFrame">
    <div class="title">
        <?= $this->message['Title']; ?>
    </div>

    <div class="content">
        <?= $this->message['Content']; ?>
    </div>

    <div class="buttons">
        <?php $this->theme->display_button(array(
            'Title' => $this->message['ButtonTitle'],
            'Link'  => $this->message['ButtonLink'],
            'Size'  => 'Large',
        )); ?>
    </div>
</div>
