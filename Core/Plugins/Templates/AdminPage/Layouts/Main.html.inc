<?php
/**
 * @brief AdminPage Template main layout.
 * @details Plugin / Template layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Feb 2020
 * @date Modification: Jan 2022
 * @copyright 2020 - 2022 CaMykS Team
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
                <?php if ($this->has_logo()) : ?>
                <div class="logo">
                    <img src="<?= $this->get_headerLogo(); ?>" alt="<?= $this->get_headerTitle(); ?>" title="<?= $this->get_headerTitle(); ?>" />
                </div>
                <?php else : ?>
                <div class="title">
                    <?= $this->get_headerTitle(); ?>
                </div>
                <?php endif; ?>
                <div class="infos">
                    &nbsp;
                </div>
            </div>
        </div>
    </div>
    <div id="PageContent">
        <?php if (($v = $CaMykS->get_sessionValue('AdminInterface-Message', '')) !== '') : ?>
            <div class="interfaceMessage">
                <?= $v; ?>
            </div>
            <?php $CaMykS->set_sessionValue('AdminInterface-Message', ''); ?>
        <?php endif; ?>

        <?php if (($v = $CaMykS->get_sessionValue('AdminInterface-Error', '')) !== '') : ?>
            <div class="interfaceError">
                <?= $v; ?>
            </div>
            <?php $CaMykS->set_sessionValue('AdminInterface-Error', ''); ?>
        <?php endif; ?>

        <?php if (($v = $CaMykS->get_sessionValue('AdminInterface-Warning', '')) !== '') : ?>
            <div class="interfaceWarning">
                <?= $v; ?>
            </div>
            <?php $CaMykS->set_sessionValue('AdminInterface-Warning', ''); ?>
        <?php endif; ?>

        <div class="innerContent">
            <?= $this->display_slotContent(1); ?>
        </div>
    </div>
</div>

<div id="ActionBar">
    <?php if ($this->actionBarStatus) : ?>
        <div class="button menu">
            <a href="javascript:AdminPage.toggle_adminMenu();" class="noSize">
                <img src="<?= $this->get_fileURL('Images/ActionBarButtons/Menu.png'); ?>" alt="<?= $this->get_locale('Menu'); ?>" title="<?= $this->get_locale('Menu'); ?>" />
            </a>
        </div>
        <div class="button home">
            <a href="<?= $this->get_homeLink(); ?>" class="noSize">
                <img src="<?= $this->get_fileURL('Images/ActionBarButtons/Home.svg'); ?>" alt="<?= $this->get_locale('Home'); ?>" title="<?= $this->get_locale('Home'); ?>" />
            </a>
        </div>
        <?php foreach ($this->actionBarButtons as $button) : ?>
        <div class="button">
            <a href="<?= $button['Link']; ?>" class="noSize">
                <img src="<?= $button['Icon']; ?>" alt="<?= $button['Title']; ?>" title="<?= $button['Title']; ?>" />
            </a>
        </div>
        <?php endforeach; ?>
        <div class="button logout">
            <a href="javascript:AdminPage.open_logoutPopup();" class="noSize">
                <img src="<?= $this->get_fileURL('Images/ActionBarButtons/Logout.svg'); ?>" alt="<?= $this->get_locale('LogOut'); ?>" title="<?= $this->get_locale('LogOut'); ?>" />
            </a>
        </div>
    <?php endif; ?>
    <div class="version">
        <?= CAMYKS_NAME; ?> v. <?= CAMYKS_VERSION; ?>
    </div>
</div>

<?php if ($this->actionBarStatus) : ?>
    <div id="AdminMenu">
        &nbsp;
    </div>
<?php endif; ?>
