<?php
/**
 * @brief CaMykSManager plugin, information panel layout.
 * @details Plugin / Special layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Sep 2020
 * @date Modification: Feb 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;
?>
<div class="mainFrame configurationPanel">
    <div class="buttons rightFloated">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Action_BackToSiteList'),
            'Link'  => $this->get_actionLink('SiteList'),
        )); ?>
    </div>

    <h1 class="title1">
        <?= $this->get_locale('SiteList_ServerNameX', gethostname()); ?>
    </h1>

    <div class="clear">
        &nbsp;
    </div>

    <div class="clear">
        &nbsp;
    </div>

    <div class="columns">
        <div class="column third">
            <h2 class="title2">
                <?= $this->get_locale('Information_Server'); ?>
            </h2>

            <?php $server = server_getSoftware(); ?>
            <?php $allModules = server_checkModuleRequirements(); ?>
            <?php if (count($allModules) > 0) : ?>
                <h3 class="title3">
                    <?= $server['Name'].' '.$server['Version']; ?>
                </h3>

                <?php foreach ($allModules as $level => $modules) : ?>
                <div class="">
                    &nbsp;
                </div>

                <h4 class="title4">
                    <?= $this->get_locale('Information_Modules_'.$level); ?>
                </h4>

                <ul class="noDot">
                    <?php foreach ($modules as $module => $status) : ?>
                        <li>
                            <?= ($status ? '<span class="valid">&check;</span>' : '<span class="invalid">&cross;</span>').' '.$module; ?>
                        </li>
                    <?php endforeach; ?>
                </ul>
                <?php endforeach; ?>
            <?php endif; ?>

        </div>
        <div class="column third">
            <h2 class="title2">
                &nbsp;
            </h2>

            <h3 class="title3">
                <?= $this->get_locale('Information_PHP'); ?> <?= phpversion(); ?>
            </h3>

            <div class="">
                &nbsp;
            </div>

            <?php if (($v = opcache_get_status()) !== false and $v['opcache_enabled'] === true) : ?>
                <?php $this->theme->display_button(array(
                    'Title' => $this->get_locale('Action_ResetOpcodeCache'),
                    'Link'  => $this->get_actionLink('ResetOpcodeCache'),
                    'Size'  => 'Small',
                )); ?>

                <div class="">
                    &nbsp;
                </div>
            <?php endif; ?>

            <h4 class="title4">
                <?= $this->get_locale('Information_AvailableDatabaseDrivers'); ?>
            </h4>

            <ul>
            <?php foreach ($this->dbDrivers as $driver) : ?>
                <li>
                    <?= $driver; ?>
                </li>
            <?php endforeach; ?>
            </ul>

            <?php $allModules = php_checkModuleRequirements(); ?>
            <?php foreach ($allModules as $level => $modules) : ?>
                <div class="">
                    &nbsp;
                </div>

                <h4 class="title4">
                    <?= $this->get_locale('Information_Modules_'.$level); ?>
                </h4>

                <ul class="noDot">
                <?php foreach ($modules as $module => $status) : ?>
                    <li>
                        <?= ($status ? '<span class="valid">&check;</span>' : '<span class="invalid">&cross;</span>').' '.$module; ?>
                    </li>
                <?php endforeach; ?>
                </ul>
            <?php endforeach; ?>
        </div>
        <div class="column third">

            <h2 class="title2">
                CaMykS
            </h2>

            <p class="content1">
                <?= $this->get_locale('Information_CurrentVersion'); ?> <?= CAMYKS_VERSION; ?> <br />
                <?= $this->get_locale('Information_LatestVersion'); ?> <?= CAMYKS_VERSION; ?> <br />
            </p>

            <div class="buttons">
                <?php $this->theme->display_button(array(
                    'Title' => $this->get_locale('Action_Update'),
                    'Link'  => '#',
                    'Size'  => 'Small',
                )); ?>

                <?php $this->theme->display_button(array(
                    'Title' => $this->get_locale('Action_CheckIntegrity'),
                    'Link'  => '#',
                    'Size'  => 'Small',
                )); ?>
            </div>
        </div>
    </div>
</div>
