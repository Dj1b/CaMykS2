<?php
/**
 * @brief CaMykSManager plugin, login layout.
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
<div class="loginFrame">
    <div class="title">
        <?= $this->get_locale('Login_Title'); ?>
    </div>

    <div class="content">
        <?= $this->get_locale('Login_Content'); ?>
    </div>

    <?php $this->managerForm->display_header(); ?>
    <?php $this->managerForm->display_hiddens(); ?>

    <div class="input">
        <?php $this->managerForm->display_passwordInput(array('name'=>'ManagerPassword', 'placeholder'=>$this->get_locale('Password'))); ?>
    </div>

    <?php $this->managerForm->display_footer(); ?>

    <div class="buttons">
        <?php $this->theme->display_button(array(
            'Title' => $this->get_locale('Login_Access'),
            'Link'  => $this->managerForm->get_HTMLSubmitLink(),
        )); ?>
    </div>

    <?php if (!client_isHTTPS()) : ?>
        <div class="warning">
            <?= $this->get_locale('Login_ConnectionNotScured'); ?>
        </div>
    <?php endif; ?>
</div>
