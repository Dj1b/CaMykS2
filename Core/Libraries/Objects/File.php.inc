<?php
/**
 * @brief CaMykS file object library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.4
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @todo File icon collections.
 */

namespace CaMykS2;

/**
 * File class.
 */
final class File extends CaMykSLibrary{
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
     * @var string $mode
     * @brief File opening mode.
     */
    private $mode;

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
        'mime'      => null,
        'type'      => null,
        'size'      => null,
        'atime'     => null,
        'mtime'     => null,
        'ctime'     => null,
        'inode'     => null,
        'width'     => null,
        'height'    => null,
    );

    /**
     * Class constructor.
     * @param string $path
     * @param string $file
     * @param string $mode
     * @return void
     */
    public function __construct($path='', $file=null, $mode='Read') {
        parent::__construct(array());

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

        /* Check mode */
        $this->mode = ucwords(strtolower($mode));
        if (!in_array($this->mode, array('Read', 'Write', 'Detete')))
            $this->mode = 'Read';

        /* Check if file exists in read/delete mode */
        if ($this->mode != 'Write' and ($this->isAccessible = file_exists($this->filePath)) == false)
            return $this->add_error(1, '', __FILE__, __LINE__);

        /* Check if file is accessible in given mode */
        if (($this->isAccessible = folder_checkPathAccessibility($this->filePath, $this->mode)) == false)
            return $this->add_error(2, '', __FILE__, __LINE__);

        /* Split path and file */
        $this->path = dirname($this->filePath);
        $this->file = basename($this->filePath);

        /* Get file extension */
        $this->ext = pathinfo($this->file, PATHINFO_EXTENSION);
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            1 => 'FileDoesntExists',
            2 => 'FileIsntAccessible',
        );
    }

    /* Check methods */

    /**
     * Is file readable.
     * @return boolean result
     */
    public function is_readable() {
        if (!$this->is_accessible('Read'))
            return false;
        return is_readable($this->filePath);
    }

    /**
     * Is file writable.
     * @return boolean result
     */
    public function is_writable() {
        if (!$this->is_accessible('Write'))
            return false;
        return is_writable($this->filePath);
    }

    /**
     * Is file accessible.
     * @param string $mode
     * @return boolean result
     */
    public function is_accessible($mode='All') {
        if ($mode !== 'All' and $this->mode !== $mode)
            return false;
        return $this->isAccessible;
    }

    /**
     * Is file a picture.
     * @return boolean result
     */
    public function is_picture() {
        return ($this->get_genericType() === 'image');
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
     * Return querystring.
     * @return string
     */
    public function get_queryString() {
        if (!$this->is_accessible())
            return false;
        return $this->queryString;
    }

    /* Read mode methods */

    /**
     * Return mime.
     * @return string
     */
    public function get_mime() {
        if (!$this->is_accessible('Read'))
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
        if (!$this->is_accessible('Read'))
            return false;

        /* Load information if not already loaded */
        $this->load_mimeInformation();

        /* Parse mime to get type */
        $mime = explode('/', $this->stats['mime']);

        /* Check type from first part of mime */
        if ($mime[0] !== false and !in_array($mime[0], array('application', 'text')))
            return $mime[0];

        /* Clean type in second part of mime */
        $type = preg_replace(array('/^x-/', '/^x-script\./', '/^vnd\./', '/-compressed$/', '/-source$/'), '', $mime[1]);

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
        if (!$this->is_accessible('Read'))
            return false;

        /* Load information if not already loaded */
        $this->load_mimeInformation();

        /* Parse mime to get type */
        $mime = explode('/', $this->stats['mime']);

        /* Remove first part of mime */
        if ($mime[0] === false or !isset($mime[1]))
            return '';

        /* Clean type in second part of mime */
        $type = preg_replace(array('/^x-/', '/^x-script\./', '/^vnd\./', '/-compressed$/', '/-source$/'), '', $mime[1]);

        /* Return default type: document */
        return $type;
    }

    /**
     * Return description.
     * @param array $params
     * @return string
     */
    public function get_description($params=array()) {
        if (!$this->is_accessible('Read'))
            return false;

        /* Default parameters list */
        $defaultParams = array('Mode' => 'Long', 'AddSize' => false, 'AddExt' => false, 'SizeFormat' => 'Binary');

        /* Merge parameters */
        $params = array_merge($defaultParams, $params);

        /* Check mode */
        $params['Mode'] = ucwords(strtolower($params['Mode']));
        if (!in_array($params['Mode'], array('Long', 'Short')))
            $params['Mode'] = $defaultParams['Mode'];

        /* Load type */
        $type = $this->get_genericType();

        /* Load locale from type */
        $s = $this->get_locale($params['Mode'].'Desc_'.ucwords($type));

        /* Initialise additionnal elements */
        $add = array();

        /* Check for extension */
        if ($params['AddExt'])
            if ($type == 'Document')
                $add[] = strtoupper($this->ext);
            else
                $add[] = strtoupper($this->get_type());

        /* Check for size */
        if ($params['AddSize'])
            $add[] = $this->get_humanReadableSize($params['SizeFormat']);

        /* Check for additionnal elements */
        if (count($add) > 0)
            $s .= ' ('.implode(', ', $add).')';

        /* Return result */
        return $s;
    }

    /**
     * Return icon URL.
     * @param string $collection
     * @param string $size
     * @param string $URLMode
     * @return string
     */
    public function get_iconURL($collection='', $size=16, $URLMode=false) {
        if (!$this->is_accessible('Read'))
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
        if (!$this->is_accessible('Read'))
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
    public function get_humanReadableSize($format='Binary') {
        if (!$this->is_accessible('Read'))
            return false;

        /* Load statistics if not already loaded */
        $this->load_statistics();

        /* Check format */
        if ($format == 'Decimal')
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

    /* Checksum methods */

    /**
     * Return MD5 checksum for the file.
     * @param boolean $rawOutput
     * @return string
     */
    public function md5($rawOutput=false) {
        if (!$this->is_accessible('Read'))
            return false;
        return md5_file($this->filePath, $rawOutput);
    }

    /**
     * Return CRC32 checksum for the file.
     * @param boolean $rawOutput
     * @return string
     */
    public function crc32($rawOutput=false) {
        if (!$this->is_accessible('Read'))
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
        global $CaMykS;

        /* Check file is accessible */
        if (!$this->is_accessible('Read'))
            return false;

        /* Write headers */
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

        /* Write file content */
        echo $this->get_contents();

        /* Close CaMykS */
        $CaMykS->close();
    }

    /**
     * Send file to client browser.
     * @param string $title
     * @return void
     */
    public function send($title=null) {
        global $CaMykS;

        /* Check file is accessible */
        if (!$this->is_accessible('Read'))
            return false;

        /* Write headers */
        header('Content-Type: '.$this->get_mime());
        if (isset($title)) {
            if (pathinfo($title, PATHINFO_EXTENSION) === '')
                $title .= '.'.$this->ext;
        } else {
            $title  = $this->file;
        }
        header('Content-Disposition: inline; filename="'.$title.'"');

        /* Write file content */
        echo $this->get_contents();

        /* Close CaMykS */
        $CaMykS->close();
    }

    /* Write mode methods */

    /**
     * Write content to file.
     * @param string $content
     * @param boolean $deleteOPCache
     * @return boolean success
     */
    public function write($content, $deleteOPCache=false) {
       if (!$this->is_accessible('Write'))
            return false;

        /* Write file */
        if (@file_put_contents($this->filePath, $content, LOCK_EX) !== false) {
            /* Remove cache from OPCache */
            if ($deleteOPCache and function_exists('opcache_invalidate')) opcache_invalidate($this->filePath);

            /* Return sucess */
            return true;
        }

        /* Return failure */
        return false;
    }

    /**
     * Append content to file.
     * @param mixed $content
     * @return boolean success
     */
    public function append($content) {
       if (!$this->is_accessible('Write'))
            return false;

        if (is_string($content))
            return @file_put_contents($this->filePath, $content."\n", FILE_APPEND|LOCK_EX);
        elseif (is_array($content)) {
            foreach ($content as $line) {
                if (!is_string($line))
                    return false;
                if (@file_put_contents($this->filePath, $line."\n", FILE_APPEND|LOCK_EX) === false)
                    return false;
            }
            return true;
        }
        return false;
    }

    /* Delete mode methods */

    /**
     * Delete file.
     * @return boolean success
     */
    public function delete() {
        if (!$this->is_accessible('Delete'))
            return false;
        return @unlink($this->filePath);
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
        return 'data:'.$this->get_mime().';base64,'.base64_encode($this->get_contents());
    }

    /* Picture related methods */

    /**
     * Return picture size as an array.
     * @return array
     */
    public function get_pictureSize() {
        /* Check file is an picture */
        if (!$this->is_picture())
            return false;

        /* Load picture size */
        $this->load_pictureInformation();

        /* Return result */
        return array('width'=>$this->stats['width'], 'height'=>$this->stats['height']);
    }

    /**
     * Return picture width.
     * @return array
     */
    public function get_pictureWidth() {
        /* Check file is an picture */
        if (!$this->is_picture())
            return false;

        /* Load picture size */
        $this->load_pictureInformation();

        /* Return result */
        return $this->stats['width'];
    }

    /**
     * Return picture height.
     * @return array
     */
    public function get_pictureHeight() {
        /* Check file is an picture */
        if (!$this->is_picture())
            return false;

        /* Load picture size */
        $this->load_pictureInformation();

        /* Return result */
        return $this->stats['height'];
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

    /**
     * Load picture information.
     * @return void
     */
    private function load_pictureInformation() {
        /* Check information are already loaded */
        if ($this->stats['width'] != null)
            return true;

        /* Load data */
        $data = getImageSize($this->filePath);

        /* Check data */
        if ($data == '')
            return;

        /* Add sizes to statistics */
        $this->stats['width'] = $data[0];
        $this->stats['height'] = $data[1];
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
        $finfo = @new \finfo(FILEINFO_MIME);
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
     * @return boolean success
     */
    private function load_mimeInformationFromMimeContentType() {
        /* Check php method is available */
        if (!function_exists('mime_content_type'))
            return false;

        /* Load file mime */
        $mime = mime_content_type($this->filePath);

        /* Check result */
        if ($mime == '')
            return false;

        /* Update value */
        $this->stats['mime'] = $mime;

        /* Return success */
        return true;
    }

    /**
     * Load file mime from file extension.
     * @return boolean success
     */
    private function load_mimeInformationFromExtension() {
        global $CaMykS, $cMimes;

        /* Check for mimes list */
        if (is_null($cMimes))
            $CaMykS->load_library('fileMimes', 'Data');

        /* Check ext in mimes */
        if (isset($cMimes[$this->ext]))
            $this->stats['mime'] = $cMimes[$this->ext];
        else
            $this->stats['mime'] = 'application/octet-stream';

        /* Return success */
        return true;
    }
}
?>
