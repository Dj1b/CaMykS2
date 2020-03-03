<?php
/**
 * @brief CaMykS theme button generic layout.
 * @details Engine / Layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @cond Exclude
 */
?>
<a href="<?= $this->layoutContext['Link']; ?>" class="<?= $this->layoutContext['Classes']; ?>"<?php if ($this->layoutContext['Id'] != '') : ?>
    id="<?= $this->layoutContext['Id']; ?>"
<?php endif; ?>
<?php if ($this->layoutContext['Target'] != '') : ?>
    target="<?= $this->layoutContext['Target']; ?>"
<?php endif; ?>
<?php if ($this->layoutContext['Styles'] != '') : ?>
    style="<?= $this->layoutContext['Styles']; ?>"
<?php endif; ?>>
<?= $this->layoutContext['Title']; ?></a>
<?php
/**
 * @endcond Exclude
 */
?>
