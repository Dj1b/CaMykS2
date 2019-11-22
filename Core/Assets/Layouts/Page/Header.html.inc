<?php
/**
 * @brief CaMykS message page header.
 * @details Engine / Layout
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;

try {
    /* Try to load CaMykS logo for header */
    $img = new CaMykS2\File($CaMykS->get_assetFilePath('Images/Logo/logo500.png'));
    if ($img->is_accessible()) {
        $header = html_getImg(array(
            'src' => $img->get_base64Data(),
            'alt' => $CaMykS->name,
        ));
    } else
        $header = '';
} catch (Exception $e) {
    $header = '';
}

?>
<!DOCTYPE html>
<html>
    <head>
        <title> </title>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="author" content="CaMykS Engine" />
        <style type="text/css">
            * {font-family : Arial, Verdana, Helvetica; font-size : 11px; font-weight : normal; font-style : normal; color : #333333; padding : 0; margin : 0;}
            body {background: #DEDEEE;}
            p {padding:4px;}
            a {font-weight:bold; color:#666686;}
            #PageMain {margin: 100px auto; width:500px; max-width:100%;}
            #PageHeader {background : #666686; padding : 6px 12px; font-weight:bold; font-size:14px; color:white; }
            #PageContent {background : white; padding: 12px 8px;}
            #PageFooter {background : #666686; padding : 6px 12px; font-size:10px; color:white; text-align:right;}
            #description {color:#CC3333; font-weight:bold; font-size:12px;}
        </style>
    </head>
    <body>
        <div id="PageMain">
            <?= $header; ?>
        </div>
        <div class="PageContent">
