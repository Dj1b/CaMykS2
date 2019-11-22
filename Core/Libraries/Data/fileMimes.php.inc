<?php
/**
 * @brief CaMykS file MIME definitions from extensions.
 * @details Engine / Data Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */
global $cMimes;

$cMimes = array(
    /* Images */
    'bmp'   => 'image/bmp',
    'gif'   => 'image/gif',
    'ico'   => 'image/x-icon',
    'jpg'   => 'image/jpeg',
    'jpeg'  => 'image/jpeg',
    'jpe'   => 'image/jpeg',
    'jp2'   => 'image/jp2',
    'pct'   => 'image/pict',
    'pic'   => 'image/pict',
    'pict'  => 'image/pict',
    'tiff'  => 'image/tiff',
    'png'   => 'image/png',
    'svg'   => 'image/svg+xml',
    'tif'   => 'image/tiff',
    'webp'  => 'image/webp',

    /* Audios */
    'aac'   => 'audio/aac',
    'aif'   => 'audio/aiff',
    'aiff'  => 'audio/aiff',
    'aifc'  => 'audio/aiff',
    'm2a'   => 'audio/aiff',
    'mp3'   => 'audio/mpeg',
    'm4a'   => 'audio/mp4',
    'mpa'   => 'audio/mpeg',
    'mid'   => 'audio/midi',
    'midi'  => 'audio/midi',
    'oga'   => 'audio/ogg',
    'ra'    => 'audio/x-realaudio',
    'ram'   => 'audio/x-realaudio',
    'rm'    => 'audio/x-realaudio',
    'rmm'   => 'audio/x-realaudio',
    'rmp'   => 'audio/x-realaudio',
    'wav'   => 'audio/wav',
    'weba'  => 'audio/webm',

    /* Videos */
    '3gp'   => 'video/3gpp',
    '3g2'   => 'video/3gpp2',
    'asf'   => 'video/x-ms-asf',
    'avi'   => 'video/avi',
    'dv'    => 'video/x-dv',
    'm1v'   => 'video/mpeg',
    'm2v'   => 'video/mpeg',
    'm4v'   => 'video/mpeg',
    'mp2'   => 'video/mpeg',
    'mp4'   => 'video/mpeg',
    'mpg'   => 'video/mpeg',
    'mpeg'  => 'video/mpeg',
    'mpe'   => 'video/mpeg',
    'mjpg'  => 'video/x-motion-jpeg',
    'mkv'   => 'video/x-matroska',
    'moov'  => 'video/quicktime',
    'mov'   => 'video/quicktime',
    'ogv'   => 'video/ogg',
    'qt'    => 'video/quicktime',
    'webm'  => 'video/webm',

    /* Texts */
    'css'   => 'text/css',
    'csv'   => 'text/csv',
    'htm'   => 'text/html',
    'html'  => 'text/html',
    'htmls' => 'text/html',
    'shtml' => 'text/html',
    'xhtml' => 'application/xhtml+xml',
    'ics'   => 'text/calendar',
    'markdown' => 'text/x-markdown',
    'md'    => 'text/x-markdown',
    'rt'    => 'text/richtext',
    'rtf'   => 'text/richtext',
    'rtx'   => 'text/richtext',
    'sgm'   => 'text/sgml',
    'sgml'  => 'text/sgml',
    'txt'   => 'text/plain',
    'text'  => 'text/plain',
    'vcard' => 'text/vcard',
    'vcf'   => 'text/vcard',

    /* Archives */
    '7z'    => 'application/x-7z-compressed',
    'ace'   => 'application/x-ace-compressed',
    'bz'    => 'application/x-bzip',
    'bz2'   => 'application/x-bzip2',
    'cpt'   => 'application/x-compactpro',
    'gz'    => 'application/x-gzip',
    'gzip'  => 'application/x-gzip',
    'hqx'   => 'application/mac-binhex40',
    'rar'   => 'application/x-rar-compressed',
    'sit'   => 'application/x-stuffit',
    'sitx'  => 'application/x-stuffit',
    'tar'   => 'application/x-tar',
    'tgz'   => 'application/gnutar',
    'z'     => 'application/x-zip-compressed',
    'zip'   => 'application/x-zip-compressed',

    /* Source & scripts */
    'cco'   => 'application/x-cocoa',
    'csh'   => 'application/x-csh',
    'jav'   => 'text/x-java-source',
    'java'  => 'text/x-java-source',
    'jar'   => 'application/java-archive',
    'js'    => 'application/javascript',
    'json'  => 'application/json',
    'p'     => 'text/x-pascal',
    'pas'   => 'text/pascal',
    'php'   => 'text/php',
    'pl'    => 'text/x-script.perl',
    'pm'    => 'text/x-script.perl',
    'py'    => 'text/x-script.python',
    'pyc'   => 'application/x-bytecode.python',
    'sh'    => 'application/x-sh',
    'xml'   => 'text/xml',

    /* Fonts */
    'eot'   => 'application/vnd.ms-fontobject',
    'otf'   => 'font/otf',
    'ttf'   => 'font/ttf',
    'woff'  => 'font/woff',
    'woff2' => 'font/woff2',

    /* Disk images */
    'dmg'   => 'application/x-apple-diskimage',
    'iso'   => 'application/x-iso9660-image',

    /* Pcakages */
    'apk'   => 'application/vnd.android.package-archive',
    'deb'   => 'application/x-debian-package',
    'mpkg'  => 'application/vnd.apple.installer+xml',
    'rpm'   => 'application/x-redhat-package-manager',

    /* Signatures */
    'crt'   => 'application/x-x509-ca-cert',
    'der'   => 'application/x-x509-ca-cert',
    'pem'   => 'application/x-x509-ca-cert',
    'p10'   => 'application/pkcs10',
    'p10'   => 'application/x-pkcs10',
    'p12'   => 'application/pkcs-12',
    'p12'   => 'application/x-pkcs12',
    'p7a'   => 'application/x-pkcs7-signature',
    'p7c'   => 'application/pkcs7-mime',
    'p7c'   => 'application/x-pkcs7-mime',
    'p7m'   => 'application/pkcs7-mime',
    'p7m'   => 'application/x-pkcs7-mime',
    'p7r'   => 'application/x-pkcs7-certreqresp',
    'p7s'   => 'application/pkcs7-signature',

    /* Generic documents */
    'eps'   => 'application/postscript',
    'ps'    => 'application/postscript',
    'pdf'   => 'application/pdf',
    'epub'  => 'application/epub+zip',
    'ts'    => 'application/typescript',

    /* Internet documents */
    'kml'   => 'application/vnd.google-earth.kml+xml',
    'kmz'   => 'application/vnd.google-earth.kmz',
    'rss'   => 'application/rss+xml',
    'torrent' => 'application/x-bittorrent',

    /* Adobe documents */
    'ai'    => 'application/postscript',
    'dcr'   => 'application/x-director',
    'dxr'   => 'application/x-director',
    'psd'   => 'image/vnd.adobe.photoshop',
    'swf'   => 'application/x-shockwave-flash',

    /* Microsoft documents */
    'doc'   => 'application/msword',
    'dot'   => 'application/msword',
    'word'  => 'application/msword',
    'docx'  => 'application/msword',
    'mdb'   => 'application/x-msaccess',
    'pot'   => 'application/mspowerpoint',
    'ppa'   => 'application/mspowerpoint',
    'ppt'   => 'application/mspowerpoint',
    'pps'   => 'application/mspowerpoint',
    'ppz'   => 'application/mspowerpoint',
    'pwz'   => 'application/mspowerpoint',
    'pptx'  => 'application/mspowerpoint',
    'xl'    => 'application/msexcel',
    'xla'   => 'application/msexcel',
    'xlb'   => 'application/msexcel',
    'xlc'   => 'application/msexcel',
    'xld'   => 'application/msexcel',
    'xlk'   => 'application/msexcel',
    'xll'   => 'application/msexcel',
    'xlm'   => 'application/msexcel',
    'xls'   => 'application/msexcel',
    'xlt'   => 'application/msexcel',
    'xlv'   => 'application/msexcel',
    'xlw'   => 'application/msexcel',
    'xlwx'  => 'application/msexcel',
    'vsd'   => 'application/vnd.visio',

    /* Amazon documents */
    'azw'   => 'application/vnd.amazon.ebook',

    /* OpenOffice documents */
    'odp'   => 'application/vnd.oasis.opendocument.presentation',
    'ods'   => 'application/vnd.oasis.opendocument.spreadsheet',
    'odt'   => 'application/vnd.oasis.opendocument.text',

    /* Other documents */
    'atom'  => 'application/atom+xml',
    'latext' => 'application/x-latex',

    /* Extensions */
    'crx'   => 'application/x-chrome-extension',
    'oex'   => 'application/x-opera-extension',
);
?>
