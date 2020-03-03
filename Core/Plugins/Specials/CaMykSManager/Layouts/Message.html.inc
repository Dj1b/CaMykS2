<?php
/**
 * @brief CaMykSManager plugin, message layout.
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
<div class="shortFrame">
    <div class="interfaceTitle">
        <?= $this->message['Title']; ?>
    </div>

    <div class="largeContent">
        <?= $this->message['Content']; ?>
    </div>

    <div class="interfaceButtons">
        <?php $this->theme->display_button(array(
            'Title' => $this->message['ButtonTitle'],
            'Link'  => $this->message['ButtonLink'],
        )); ?>
    </div>
</div>