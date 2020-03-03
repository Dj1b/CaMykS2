<?php
/**
 * @brief CaMykS message page header.
 * @details Engine / Layout
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.2
 * @date Creation: Nov 2019
 * @date Modification: Mar 2020
 * @copyright 2019 - 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
try {
    /* Try to load CaMykS logo for header */
    $img = new CaMykS2\File($this->get_assetFilePath('Images/Logo/logo400.png'));
    if ($img->is_accessible()) {
        $header = html_getImg(array(
            'src' => $img->get_base64Data(),
            'alt' => $this->name,
        ));
    } else
        $header = '';
} catch (Exception $e) {
    $header = '';
}

?>
<div id="PageMain">
    <div id="PageHeader">
        <?= $header; ?>
    </div>
    <div id="PageContent">
