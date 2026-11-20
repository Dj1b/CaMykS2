<?php
/**
 * @brief CaMykSManager plugin, configuration panel, view plugins section sublayout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Sep 2020
 * @date Modification: Nov 2026
 * @copyright 2020 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;

$plugins = $CaMykS->get_configValue('DefaultPlugins');
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
            <?php foreach ($plugins[$pluginType] as $pluginName => $pluginSource) : ?>
                <li>
                    <?= $pluginName; ?>
                </li>
            <?php endforeach; ?>
        </ul>
    </div>
    <?php endforeach; ?>
</div>
