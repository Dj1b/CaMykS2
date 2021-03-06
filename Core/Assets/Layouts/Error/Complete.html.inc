<?php
/**
 * @brief CaMykS complete error display.
 * @details Engine / Layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Dec 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
$email = $this->get_configValue('Engine_WebmasterEmail');

$emailLink = 'website administrator';
if ($email == false or $email != '')
    $emailLink = '<a href="mailto:'.$email.'">'.$emailLink.'</a>';

switch ($this->mode) {
    case CAMYKS_MODE_VISIT:
        $backLink = '';
        break;
    case CAMYKS_MODE_ADMIN:
        $backLink = $this->get_adminLink();
        break;
    case CAMYKS_MODE_REQUEST:
        $backLink = '';
        break;
    case CAMYKS_MODE_MANAGER:
        $backLink = 'manager.php';
        break;
    case CAMYKS_MODE_INSTALL:
        $backLink = '';
        break;
}

?>
<h1>
    An error has occured
</h1>
<p>
    <strong>
        <?= $context->get_header(); ?>
    </strong>
</p>

<?php if ($context->has_location()) : ?>
<p>
    <em>
        Location : <?= $context->get_location(); ?>
    </em>
</p>
<?php endif; ?>

<?php if ($context->has_message()) : ?>
<p>
    Message : <?= $context->get_message(true); ?>
</p>
<?php endif; ?>

<p>
    &nbsp;
</p>

<p class="purple">
    For any questions, or if the problem persists, please contact the <?= $emailLink; ?>.
</p>

