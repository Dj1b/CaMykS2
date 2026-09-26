<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view plugins section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Sep 2020
 * @date Modification: Feb 2026
 * @copyright 2020 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<h3 class="title3">
    <?= $this->get_locale('Configuration_DefaultPluginsSettings'); ?>
</h3>
<div class="columns">
    <?php foreach ($CaMykS->get_pluginTypes() as $pluginType) : ?>
    <div class="column half">
        <h2 class="title2">
            <?= $this->get_locale($pluginType.'s'); ?>
        </h2>
        <ul>
            <?php foreach ($CaMykS->get_configValue('Default'.$pluginType.'s') as $pluginName) : ?>
                <li>
                    <?= $pluginName; ?>
                </li>
            <?php endforeach; ?>
        </ul>
    </div>
    <?php endforeach; ?>
</div>
