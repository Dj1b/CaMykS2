<?php
/**
 * @brief CaMykS message page header.
 * @details Engine / Layout
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Dec 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
global $CaMykS;

try {
    /* Try to load CaMykS logo for header */
    $img = new CaMykS2\File($CaMykS->get_assetFilePath('Images/Logo/logo400.png'));
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
            * {font-family : Arial, Verdana, Helvetica; font-size : 14px; font-weight : normal; font-style : normal; color : #333333; padding : 0; margin : 0; box-sizing:border-box;}
            body {background: #DFDEEE;}
            p {padding:4px; word-wrap : break-word;}
            a {font-weight:bold; color:#666686;}
            img {max-width:100%;}
            #PageMain {margin: 0 auto; width:1000px; max-width:100%; padding:0 40px;}
            #PageHeader {width:400px; max-width:100%; margin: 100px auto 60px;}
            #PageContent {padding: 12px 8px;}
            #PageContent * {text-align:center;}
            #PageFooter {position:fixed; bottom:0; right:0; padding: 4px;}
            h1 {font-size:36px; text-transform:uppercase; color:#666686; margin-bottom:20px;}
            .purple {color:#666686;}
            .button {background:#666686; padding:8px 40px; text-transform:uppercase; color:#DFDEEE; border-radius:20px; text-decoration:none; transition:all linear 0.3s;}
            .button:hover {opacity:0.8;}

            @media screen and (max-width: 700px) {
                * {font-size:12px;}
                h1 {font-size:24px;}
                #PageHeader {margin: 40px auto 30px;}—
            }

        </style>
    </head>
    <body>
        <div id="PageMain">
            <div id="PageHeader">
                <?= $header; ?>
            </div>
            <div id="PageContent">
