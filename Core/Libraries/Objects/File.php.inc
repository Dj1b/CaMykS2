<?php
/**
 * @brief CaMykS file object library.
 * <br /> Reads an existing file to do some actions on it.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * File class.
 */
final class File {
    /**
     * @var string $file
     * @brief File name.
     */
    private $file;

    /**
     * @var string $path
     * @brief File path.
     */
    private $path;

    /**
     * @var string $filePath
     * @brief File full path.
     */
    private $filePath;

    /**
     * @var string $queryString
     * @brief File query string.
     */
    private $queryString = '';

    /**
     * @var string $ext
     * @brief File extension.
     */
    private $ext;

    /**
     * @var array $mimes
     * @brief List of known mimes.
     */
    private $mimes = null;

    /**
     * @var array $stats
     * @brief File statistics.
     */
    private $stats = array(
        'mime'  => null,
        'type'  => null,
        'size'  => null,
        'atime' => null,
        'mtime' => null,
        'ctime' => null,
        'inode' => null,
    );

    /**
     * Class constructor.
     * @param string $path
     * @param string $file
     * @return void
     */
    public function __construct($path='', $file=null) {
        global $CaMykS;

        /* Define full path */
        if (is_null($file))
            $this->filePath = $path;
        else
            $this->filePath = $path.'/'.$file;

        /* Check for querystring */
        $f = explode('?', $this->filePath);
        $this->filePath = $f[0];
        if (isset($f[1]))
            $this->queryString = $f[1];

        /* Check if file is accessible */
        if (($this->isAccessible = file_exists($this->filePath)) == false)
            return;
        if (($this->isAccessible = $CaMykS->check_pathAccessibility($this->filePath, 'read')) == false)
            return;

        /* Split path and file */
        $this->path = dirname($this->filePath);
        $this->file = basename($this->filePath);

        /* Get file extension */
        $this->ext = pathinfo($this->file, PATHINFO_EXTENSION);
    }

    /* Check methods */

    /**
     * Is file readable.
     * @return boolean result
     */
    public function is_readable() {
        if (!$this->is_accessible())
            return false;
        return is_readable($this->filePath);
    }

    /**
     * Is file writable.
     * @return boolean result
     */
    public function is_writable() {
        if (!$this->is_accessible())
            return false;
        return is_writable($this->filePath);
    }

    /**
     * Is file accessible.
     * @return boolean result
     */
    public function is_accessible() {
        return $this->isAccessible;
    }

    /* Information request methods */

    /**
     * Return file name.
     * @return string
     */
    public function get_name() {
        if (!$this->is_accessible())
            return false;
        return $this->file;
    }

    /**
     * Return file path.
     * @return string
     */
    public function get_path() {
        if (!$this->is_accessible())
            return false;
        return $this->path;
    }

    /**
     * Return full file path.
     * @return string
     */
    public function get_filePath() {
        if (!$this->is_accessible())
            return false;
        return $this->filePath;
    }

    /**
     * Return mime.
     * @return string
     */
    public function get_mime() {
        if (!$this->is_accessible())
            return false;

        /* Load information if not already loaded */
        $this->load_mimeInformation();

        /* Return mime */
        return $this->stats['mime'];
    }

    /**
     * Return generic type (image, audio, video, font, script, archive, document).
     * @return string
     */
    public function get_genericType() {
        if (!$this->is_accessible())
            return false;

        /* Load information if not already loaded */
        $this->load_mimeInformation();

        /* Parse mime to get type */
        $mime = $this->stats['mime'];

        /* Check type from first part of mime */
        $type = strtok('/', $mime);
        if ($type !== false and !in_array($type, array('application', 'text')))
            return $type;

        /* Clean type in second part of mime */
        $type = preg_replace(array('/^x-/', '/^x-script\./', '/^vnd\./', '/-compressed$/', '/-source$/'), '', $mime);

        /* Check type is a font */
        if ($type == 'ms-fontobject')
            return 'font';

        /* Check type is a script */
        if (in_array($type, array('cocoa', 'csh', 'java', 'java-archive', 'javascript', 'json', 'pascal', 'php', 'perl', 'python', 'sh', 'xml')))
            return 'script';

        /* check type is an archive */
        if (in_array($type, array('7z', 'ace', 'bzip', 'bzip2', 'compactpro', 'gzip', 'mac-binhex40', 'rar', 'stuffit', 'tar', 'gnutar', 'zip')))
            return 'archive';

        /* Return default type: document */
        return 'document';
    }

    /**
     * Return type.
     * @return string
     */
    public function get_type() {
        if (!$this->is_accessible())
            return false;

        /* Load information if not already loaded */
        $this->load_mimeInformation();

        /* Parse mime to get type */
        $mime = $this->stats['mime'];

        /* Remove first part of mime */
        $type = strtok('/', $mime);

        /* Clean type in second part of mime */
        $type = preg_replace(array('/^x-/', '/^x-script\./', '/^vnd\./', '/-compressed$/', '/-source$/'), '', $mime);

        /* Return default type: document */
        return $type;
    }

    /**
     * Return icon URL.
     * @param string $collection
     * @param string $size
     * @param string $URLMode
     * @return string
     */
    public function get_iconURL($collection='', $size=16, $URLMode=false) {
        if (!$this->is_accessible())
            return false;

        /* Define collections parameters */
        $collections = array(
            /*
            'Name' => array(
                'FileExtension' => 'png',
                'Sizes'         => array(16, 32, 64, 128, 256),
            ),
            */
        );

        /* Check collection */
        if (!isset($collections[$collection]))
            return '';

        /* Check size */
        if (!in_array($size, $collections[$collection]['Sizes']))
            return '';

        /* Load file type */
        $type = $this->get_type();

        /* Load file URL from CaMykS */
        $url = $CaMykS->get_assetFileURL('Images/Mimes/'.$collection.'/'.$size.'/'.$type.'.'.$collections[$collection]['FileExtension'], $URLMode);

        /* Check URL */
        if ($url != '')
            return $url;

        /* Type icon not found, try generic type */
        $type = $this->get_genericType();

        /* Load file URL from CaMykS */
        $url = $CaMykS->get_assetFileURL('Images/Flags/'.$collection.'/'.$size.'/'.$type.'.'.$collections[$collection]['FileExtension'], $URLMode);

        /* Check URL */
        if ($url != '')
            return $url;

        /* Load default file URL from CaMykS */
        return $CaMykS->get_assetFileURL('Images/Flags/'.$collection.'/'.$size.'/default.'.$collections[$collection]['FileExtension'], $URLMode);
    }

    /**
     * return icon tag.
     * @param string $collection
     * @param string $size
     * @param string $URLMode
     * @param string $imgParams
     * @return string
     */
    public function get_iconTag($collection='', $size=16, $URLMode=false, $imgParams=array()) {
        /* Add source to image parameters */
        $imgParams['src'] = $this->get_iconURL($collection, $size, $URLMode);

        /* Add file description to image parameters */
        if (!isset($imgParams['alt']) or $imgParams['alt'] == '') {
            $imgParams['alt'] = $this->get_description();
            if (!isset($imgParams['title']) or $imgParams['title'] == '')
                $imgParams['title'] = $imgParams['alt'];
        }

        /* Return built tag */
        return html_getImg($imgParams);
    }

    /**
     * Return size.
     * @param mixed $humanReadable
     * @return string
     */
    public function get_size($humanReadable=false) {
        if (!$this->is_accessible())
            return false;

        /* Check for human readable result */
        if ($humanReadable !== false)
            return $this->get_humanReadableSize($humanReadable);

        /* Load statistics if not already loaded */
        $this->load_statistics();

        /* Return result */
        return $this->stats['size'];
    }

    /**
     * Return human readable size.
     * @param mixed $format
     * @return string
     */
    public function get_humanReadableSize($format='binary') {
        if (!$this->is_accessible())
            return false;

        /* Load statistics if not already loaded */
        $this->load_statistics();

        /* Check format */
        if ($format == 'decimal')
            $divider = 1000.00;
        else
            $divider = 1024.00;

        /* Load unit to display */
        $t = language_getFileUnit();

        /* List of multiples */
        $l = array('', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y');

        /* Reduce to correct multiple */
        $s = 0;
        $size = $this->stats['size'];
        while ($size > $divider) {
            $s++;
            $size /= $divider;
        }
        $size = ((int)($size*10)/10);

        if ($s == 0)
            return $size.' '.($t=='o' ? 'octets' : 'bytes');
        return $size.' '.$l[$s].$t;
    }

    /* Content related methods */

    /**
     * Return file content.
     * @return string
     */
    public function get_contents() {
        if (!$this->is_accessible())
            return false;
        return file_get_contents($this->filePath);
    }

    /**
     * Return file content as base64 data.
     * @return string
     */
    public function get_base64Data() {
        if (!$this->is_accessible())
            return false;
        return 'data:'.$this->mime.';base64,'.base64_encode(file_get_contents($this->filePath));
    }

    /* Checksum methods */

    /**
     * Return MD5 checksum for the file.
     * @param boolean $rawOutput
     * @return string
     */
    public function md5($rawOutput=false) {
        if (!$this->is_accessible())
            return false;
        return md5_file($this->filePath, $raw_output);
    }

    /**
     * Return CRC32 checksum for the file.
     * @param boolean $rawOutput
     * @return string
     */
    public function crc32($rawOutput=false) {
        if (!$this->is_accessible())
            return false;
        return hash_file('CRC32', $this->filePath, $rawOutput);
    }

    /* File download methods */

    /**
     * Download file to client browser.
     * @param string $title
     * @return void
     */
    public function download($title=null) {
        if (!$this->is_accessible())
            return false;
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Content-Type: application/octet-stream');
        header('Content-Description: File Transfer');
        header('Content-Length: '.$this->get_size());
        if (isset($title)) {
            if (pathinfo($title, PATHINFO_EXTENSION) === '')
                $title .= '.'.$this->ext;
        } else {
            $title  = $this->file;
        }
        header('Content-Disposition: attachment; filename="'.$title.'";');
        echo $this->get_contents();
        die();
    }

    /**
     * Send file to client browser.
     * @param string $title
     * @return void
     */
    public function send($title=null) {
        if (!$this->is_accessible())
            return false;

        header('Content-Type: '.$this->get_mime());
        if (isset($title)) {
            if (pathinfo($title, PATHINFO_EXTENSION) === '')
                $title .= '.'.$this->ext;
        } else {
            $title  = $this->file;
        }
        header('Content-Disposition: inline; filename="'.$title.'"');
        echo $this->get_contents();
        die();
    }

    /* Information loading methods */

    /**
     * Load mime information.
     * @return boolean success
     */
    private function load_mimeInformation() {
        /* Check information are already loaded */
        if ($this->stats['mime'] != null)
            return true;

        /* Try to get file info from finfo methods */
        if ($this->load_mimeInformationFromFinfo())
            return true;

        /* Try to get file info from mime content type method */
        if ($this->load_mimeInformationFromMimeContentType())
            return true;

        /* Try to get file info from internal method */
        return $this->load_mimeInformationFromExtension();
    }

    /**
     * Load file statistics.
     * @return void
     */
    private function load_statistics() {
        /* Check statistics are already loaded */
        if ($this->stats['size'] != null)
            return true;

        $stat = stat($this->filePath);
        $this->stats['size'] = $stat['size'];
        $this->stats['atime'] = $stat['atime'];
        $this->stats['mtime'] = $stat['mtime'];
        $this->stats['ctime'] = $stat['ctime'];
        $this->stats['inode'] = $stat['ino'];
    }

    /* MIME loading sub methods */

    /**
     * Load file mime from finfo method.
     * @return boolean success
     */
    private function load_mimeInformationFromFinfo() {
        /* Check finfo is available */
        if (!class_exists('finfo'))
            return false;

        /* Load finfo */
        $finfo = @new finfo(FILEINFO_MIME);
        if (!$finfo) return false;

        /* Load file mime */
        $mime = $finfo->file($this->filePath);
        $mime = explode(';', $mime);

        /* Check result */
        if ($mime[0] == '')
            return false;

        /* Update value */
        $this->stats['mime'] = $mime[0];

        /* Return success */
        return true;
    }

    /**
     * Load file mime from PHP internal method.
     * @param string $file
     * @return string
     */
    private function load_mimeInformationFromMimeContentType() {
        /* Check php method is available */
        if (!function_exists('mime_content_type'))
            return false;

        /* Load file mime */
        $mime = mime_content_type($this->filePath);

        /* Chech result */
        if ($mime == '')
            return false;

        /* Update value */
        $this->stats['mime'] = $mime;

        /* Return success */
        return true;
    }

    /**
     * Load file mime from file extension.
     * @return string
     */
    private function load_mimeInformationFromExtension() {
        global $cMimes;

        /* Check for mimes list */
        if (is_null($cMimes))
            $CaMykS->load_library('fileMimes', 'Data');

        /* Check ext in mimes */
        if (isset($cMimes[$this->ext]))
            $this->stats['mime'] = $mime;
        else
            $this->stats['mime'] = 'application/octet-stream';

        /* Return success */
        return true;
    }









    /**
     * Load file mime.
     * @return void
     *
    private function get_mime() {
        $this->mime = file_getMime($this->filePath, $this->ext);
        $t = explode('/', $this->mime);
        $this->type = $t[0];
    }

    /**
     * Load file description.
     * @return void
     *
    private function get_descriptions() {
        $this->typedesc = file_getTypeDescription($this->mime, $this->ext);
        $this->mimedesc = file_getMimeDescription($this->mime);
        $this->hsize = file_getFileSize($this->size);
    }

    /**
     * Load more information from file.
     * @return void
     *
    private function get_moreInfos() {
        if ($this->type == 'image') {
            $this->get_imageInfos();
        }
    }

    /**
     * Load more information from image file.
     * @return void
     *
    private function get_imageInfos() {
        $this->moreinfos = array_merge($this->moreinfos, file_getImageSize($this->filePath));
    }

    /**
     * Return mime relative icon.
     * @param int $size
     * @param string $styles
     * @return string
     *
    public function get_mimeIcon($size=32, $styles='') {
        return file_getMimeIcon($this->mime, $size, $styles);
    }

    /**
     * Return mime relative icon URL.
     * @param int $size
     * @return string
     *
    public function get_mimeIconURL($size=32) {
        return file_getMimeIconURL($this->mime, $size);
    }

    */
}
?>
