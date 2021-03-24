<?php
/**
 * @brief Image manipulating object
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Dec 2019
 * @date Modification: Mar 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @todo Adds ImageMagick (and/or other manipulation engines) methods.
 * @todo Finalise crop calculation in _gd_rotate method.
 * @todo Complete methods documentation with params details.
 */

namespace CaMykS2;

/**
 * Image class.
 */
final class Image extends CaMykSLibrary {
    /**
     * @var constant array DEFAULTPARAMS
     * @brief Known image manipulation engines.
     */
    private const KNOWNENGINES = array('gd');

    /**
     * @var string $source
     * @brief Image source path.
     */
    private $source;

    /**
     * @var Ressource $resource
     * @brief Image ressource to manipulate.
     */
    private $resource;

    /**
     * @var boolean $loaded
     * @brief Is image loaded.
     */
    private $loaded = false;

    /**
     * @var mixed $engine
     * @brief Image editing engine to use.
     */
    private $engine = false;

    /**
     * @var string $type
     * @brief Image type.
     */
    private $type;

    /**
     * @var integer $width
     * @brief Image current width.
     */
    private $width;

    /**
     * @var integer $height
     * @brief Image current height.
     */
    private $height;

    /**
     * @var integer $jpgQuality
     * @brief JPG Quality value for compression while saving.
     */
    private $jpgQuality = 75;

    /**
     * @var string $transparentReplacement
     * @brief Replacement color for transparency.
     */
    private $transparentReplacement = '0xFFFFFF';

    /**
     * @var boolean $hasTransparency
     * @brief Indicate if picture has transparency
     */
    private $hasTransparency = false;

    /**
     * Object constructor.
     * @param string $source
     * @param string $engine
     * @return void
     */
    public function __construct($source='', $engine='') {
        /* Check engine */
        $this->check_imageManipulationEngine($engine);

        /* Load image */
        $this->load_image($source);
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            /* Engine errors */
            1 => 'UnknownEngine',
            2 => 'EngineNotAvailable',

            /* File error */
            3 => 'UnableToLoadFile',
        );
    }

    /* Image related public methods */

    /**
     * Load image resource.
     * @param string $source
     * @return boolean result
     */
    public function load_image($source='') {
        /* Check engine */
        if ($this->engine === false)
            return false;

        /* Check source value */
        if ($source == '')
            return $this->add_error(3, $source, __FILE__, __LINE__);

        if (string_isURL($source) === false) {
            /* Check local file */
            if (!file_exists($source))
                return $this->add_error(3, $source, __FILE__, __LINE__);
        } else {
            /* Check remote file */
            if (!fopen($source, "r"))
                return $this->add_error(3, $source, __FILE__, __LINE__);
        }

        /* Load file with engine */
        switch ($this->engine) {
            case 'gd':
                return ($this->loaded = $this->_gd_load_image($source));
        }
        return false;
    }

    /**
     * Check if image is loaded.
     * @return boolean result
     */
    public function is_loaded() {
        return $this->loaded;
    }

    /**
     * Update JPEG Quality value.
     * @param integer $quality
     * @return void
     */
    public function update_JPGQuality($quality=75) {
        $this->jpgQuality = max(0, min(100, (int)$quality));
    }

    /**
     * Return picture width.
     * @return integer
     */
    public function get_width() {
        if (!$this->is_loaded()) return false;
        return $this->width;
    }

    /**
     * Return picture height.
     * @return integer
     */
    public function get_height() {
        if (!$this->is_loaded()) return false;
        return $this->height;
    }

    /**
     * Save resource as file.
     * @param string $filename
     * @param string $type
     * @return boolean success
     */
    public function save_image($filename, $type='') {
        if (!$this->is_loaded()) return false;
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_save_image($filename, $type);
        }
        return false;
    }

    /* Public manipulation methods */

    /**
     * Crop image.
     * @param integer $width
     * @param integer $height
     * @param mixed $position
     * @param string $bgColor (\#transp for transparent background)
     * @return boolean success
     */
    public function crop($width, $height, $position='', $bgColor='#FFFFFF') {
        if (!$this->is_loaded()) return false;
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_crop($width, $height, $position, $bgColor);
        }
        return false;
    }

    /**
     * Resize image.
     * @param integer $width
     * @param integer $height
     * @param integer $homothetic (0 : no / 1 : yes, use min value / 2 : yes, use max value)
     * @return boolean success
     */
    public function resize($width, $height=0, $homothetic=0) {
        if (!$this->is_loaded()) return false;
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_resize($width, $height, $homothetic);
        }
        return false;
    }

    /**
     * Sharp image.
     * @param integer $gain
     * @return boolean success
     */
    public function sharp($gain=40) {
        if (!$this->is_loaded()) return false;

        /* Check value */
        $gain = min(100, max(0, (int)$gain));

        /* Execute action is selected engine */
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_sharp($gain);
        }
        return false;
    }

    /**
     * Blur image.
     * @param integer $loss
     * @return boolean success
     */
    public function blur($loss=50) {
        if (!$this->is_loaded()) return false;

        /* Check value */
        $loss = min(100, max(0, (int)$loss));

        /* Execute action is selected engine */
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_blur($loss);
        }
        return false;
    }

    /**
     * Darken image.
     * @param integer $factor
     * @return boolean success
     */
    public function darken($factor=0) {
        if (!$this->is_loaded()) return false;

        /* Check value */
        $factor = min(100, max(0, (int)$factor));

        /* Execute action is selected engine */
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_darken($factor);
        }
        return false;
    }

    /**
     * Lighten image.
     * @param integer $factor
     * @return boolean success
     */
    public function lighten($factor=0) {
        if (!$this->is_loaded()) return false;

        /* Check value */
        $factor = min(100, max(0, (int)$factor));

        /* Execute action is selected engine */
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_lighten($factor);
        }
        return false;
    }

    /**
     * Trim image.
     * @param array $params
     * @return boolean success
     */
    public function trim($params=array()) {
        if (!$this->is_loaded()) return false;

        /* Check params */
        $dParams = array('sides'=>'tblr', 'color'=>'auto');
        $params = array_merge($dParams, $params);

        /* Execute action is selected engine */
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_trim($params);
        }
        return false;
    }

    /**
     * Rotate image.
     * @param array $params
     * @return boolean success
     */
    public function rotate($params=array()) {
        if (!$this->is_loaded()) return false;

        /* Check params */
        $dParams = array('angle'=>0, 'bkgdColor'=>0, 'ignoreTransparency'=>0, 'cropMode'=>0); // cropMode 0 : no resize, 1 : extends with bkgdColor, 2 : crop inside to avoid dead zones
        $params = array_merge($dParams, $params);

        /* Execute action is selected engine */
        switch ($this->engine) {
            case 'gd':
                return $this->_gd_rotate($params);
        }
        return false;
    }

    /* Private methods */

    /**
     * Check which image manipulation to use.
     * @param string $engine
     * @return boolean result
     */
    private function check_imageManipulationEngine($engine) {
        /* Normalise engine */
        $engine = strToLower($engine);

        /* Check provided engine */
        if (in_array($engine, self::KNOWNENGINES)) {
            /* Check favorite engine from configuration */
            $engine = $this->get_configValue('Engine_PictureEditor');
            if (!in_array($engine, self::KNOWNENGINES))
                return $this->add_error(1, $engine, __FILE__, __LINE__);
        }

        /* Check engine extension */
        if ($engine == 'gd' and !extension_loaded('gd'))
            return $this->add_error(2, $engine, __FILE__, __LINE__);

        /* Update engine */
        $this->engine = $engine;

        /* Return success */
        return true;
    }

    /* GD methods */

    /**
     * Load image with GD.
     * @return boolean success
     */
    private function _gd_load_image($source) {
        /* Save source */
        $this->source = $source;

        /* Check file type */
        $ext = file_getImageType($this->source);

        /* Check image while loading sizes */
        if (!list($this->width, $this->height) = getimagesize($this->source)) return false;

        /* Load image */
        switch($ext) {
            case 'jpg': case 'jpeg': $this->type = 'jpg'; $this->resource = @imagecreatefromjpeg($this->source); return true;
            case 'gif': $this->type = 'gif'; $this->resource = @imagecreatefromgif($this->source); return true;
            case 'png': $this->type = 'png'; $this->resource = @imagecreatefrompng($this->source); $this->_gd_add_transparency(); return true;
            case 'bmp': $this->type = 'bmp'; $this->resource = @imagecreatefromwbmp($this->source); return ($this->resource !== false);
        }
        return false;
    }

    /**
     * Save image with GD.
     * @param string $filename
     * @param string $type
     * @return boolean success
     */
    private function _gd_save_image($filename, $type) {
        /* Check resource */
        if ($this->resource == null) return false;

        /* Check type */
        if ($type == '' or !in_array($type, array('jpg', 'gif', 'png', 'bmp')))
            if (!in_array($this->type, array('jpg', 'gif', 'png', 'bmp')))
                $type = 'jpg';
            else
                $type = $this->type;

        /* Check file extension */
        if (!(strtolower(substr($filename, -4, 4)) == '.'.$type))
            $filename .= '.'.$type;

        /* Complete file path */
        folder_completePath(dirname($filename));

        /* Save image */
        switch($type) {
            case 'jpg': imagejpeg($this->resource, $filename, $this->jpgQuality); return true;
            case 'gif': imagegif($this->resource, $filename); return true;
            case 'png': imagepng($this->resource, $filename); return true;
            case 'bmp': imagewbmp($this->resource, $filename); return true;
        }
        return false;
    }

    /**
     * Crop image with GD.
     * @param integer $width
     * @param integer $height
     * @param mixed $position
     * @param string $bgColor
     * @return boolean success
     */
    private function _gd_crop($width, $height, $position, $bgColor) {
        /* Check resource */
        if ($this->resource == null) return false;

        /* Create new image */
        $newSrc = imageCreateTrueColor($width, $height);

        /* Add background color */
        if ($bgColor == '#transp' or $this->_gd_is_transparencyEnabled()) {
            imageAlphaBlending($newSrc, false);
            imageSaveAlpha($newSrc, true);
            imageFilledRectangle($newSrc, 0, 0, $width, $height, imageColorAllocateAlpha($newSrc, 255, 255, 255, 127));
        } else {
            imageFill($newSrc, 0, 0, $this->_gd_getAllocatedColor($bgColor, $newSrc));
        }

        /* Set default values */
        $dest_x = 0;
        $dest_y = 0;
        $src_x = 0;
        $src_y = 0;
        $src_w = 0;
        $src_h = 0;

        /* Determine crop position */
        if (!is_array($position)) {
            switch ($position) {
                /* Crop top left part of the image */
                case 'TL': case 1:
                    if ($width < $this->width) {
                        /* Crop inside */
                        $src_w = $width;
                    } else {
                        /* Extend image */
                        $src_w = $this->width;
                    }
                    if ($height < $this->height) {
                        /* Crop inside */
                        $src_h = $height;
                    } else {
                        /* Extend image */
                        $src_h = $this->height;
                    }
                    break;

            /* Crop top center part of the image */
            case 'T': case 2:
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                    $src_x = ($this->width - $width)/2;
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                    $dest_x = ($width - $this->width)/2;
                }
                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                }
                break;

            /* Crop top right part of the image */
            case 'TR': case 3 :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                    $src_x = ($this->width - $width);
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                    $dest_x = ($width - $this->width);
                }
                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                }
                break;

            /* Crop middle left part of the image */
            case 'L': case 4 :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                }
                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                    $src_y = ($this->height - $height)/2;
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                    $dest_y = ($height - $this->height)/2;
                }
                break;

            /* Crop middle right part of the image */
            case 'R': case 6 :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                    $src_x = ($this->width - $width);
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                    $dest_x = ($width - $this->width);
                }
                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                    $src_y = ($this->height - $height)/2;
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                    $dest_y = ($height - $this->height)/2;
                }
                break;

            /* Crop bottom left part of the image */
            case 'BL': case 7 :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                }
                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                    $src_y = ($this->height - $height);
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                    $dest_y = ($height - $this->height);
                }
                break;

            /* Crop bottom center part of the image */
            case 'B': case 8 :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                    $src_x = ($this->width - $width)/2;
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                    $dest_x = ($width - $this->width)/2;
                }
                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                    $src_y = ($this->height - $height);
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                    $dest_y = ($height - $this->height);
                }
                break;

            /* Crop bottom right part of the image */
            case 'BR': case 9 :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                    $src_x = ($this->width - $width);
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                    $dest_x = ($width - $this->width);
                }

                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                    $src_y = ($this->height - $height);
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                    $dest_y = ($height - $this->height);
                }
                break;

            /* Crop center part of the image */
            case '':  case 'C': case 0 : case 5: default :
                if ($width < $this->width) {
                    /* Crop inside */
                    $src_w = $width;
                    $src_x = ($this->width - $width)/2;
                } else {
                    /* Extend image */
                    $src_w = $this->width;
                    $dest_x = ($width - $this->width)/2;
                }

                if ($height < $this->height) {
                    /* Crop inside */
                    $src_h = $height;
                    $src_y = ($this->height - $height)/2;
                } else {
                    /* Extend image */
                    $src_h = $this->height;
                    $dest_y = ($height - $this->height)/2;
                }
                break;
            }
        }

        /* Crop */
        imagecopy($newSrc, $this->resource, $dest_x, $dest_y, $src_x, $src_y, $src_w, $src_h);
        imagedestroy($this->resource);
        $this->resource = $newSrc;
        $this->width = $width;
        $this->height = $height;
        return true;
    }

    /**
     * Resize image with GD.
     * @param integer $width
     * @param integer $height
     * @param integer $homothetic
     * @return result success
     */
    private function _gd_resize($width, $height, $homothetic) {
        /* Check sizes */
        if ($width == 0 and $height == 0) {
            return false;
        } elseif ($width == 0 or $height == 0) {
            $homothetic = true;
        }

        /* Check for homothetic resize */
        if ($homothetic > 0) {
            if (($homothetic == 1 and $width/$this->width > $height/$this->height) or ($homothetic == 2 and $width/$this->width < $height/$this->height))
                $height = (int)($this->height*$width/$this->width);
            else
                $width = (int)($this->width*$height/$this->height);
        }

        /* Create new image */
        $newSrc = imagecreatetruecolor($width, $height);
        if ($this->_gd_is_transparencyEnabled()) {
            imageAlphaBlending($newSrc, false);
            imageSaveAlpha($newSrc, true);
        }
        imagecopyresampled($newSrc, $this->resource, 0, 0, 0, 0, $width, $height, $this->width, $this->height);
        imagedestroy($this->resource);
        $this->resource = $newSrc;
        $this->width = $width;
        $this->height = $height;
        return true;
    }

    /**
     * Sharp image with GD.
     * @param integer $gain
     * @return boolean success
     */
    private function _gd_sharp($gain) {
        if ($gain >= 80) {
            $gain = 4 + ((100-$gain)*5)/100;
        } else {
            $gain = 25 - 20*($gain/80);
        }
        return $this->_gd_convolution(array(array(0, -1, 0), array(-1, $gain, -1), array(0, -1, 0)));
    }

    /**
     * Blur image with GD.
     * @param integer $loss
     * @return boolean success
     */
    private function _gd_blur($loss) {
        $loss = 17 - ($loss/5);

        return $this->_gd_convolution(array(array(1, 1, 1), array(1, $loss, 1), array(1, 1, 1)));
    }

    /**
     * Darken image with GD.
     * @param integer $factor
     * @return boolean success
     */
    private function _gd_darken($factor) {
        $factor = 100 - min(100, max(0, $factor));

        return $this->_gd_convolution(array(array(0, 0, 0), array(0, $factor, 0), array(0, 0, 0)), 100);
    }

    /**
     * Lighten image with GD.
     * @param integer $factor
     * @return boolean success
     */
    private function _gd_lighten($factor) {
        $factor = 100 - min(100, max(0, $factor));

        return $this->_gd_convolution(array(array(0, 0, 0), array(0, 100, 0), array(0, 0, 0)), $factor);
    }

    /**
     * Trim image with GD.
     * @param array $params
     * @return boolean success
     */
    private function _gd_trim($params) {
        /* Auto detect color */
        if ($params['color'] == 'auto') {
            $cc = array();
            $cc[imagecolorat($this->resource, 0, 0)] = 1;
            $c = imagecolorat($this->resource, imagesx($this->resource)-1, 0);
            if (isset($cc[$c]))
                $cc[$c] += 1;
            else
                $cc[$c] = 1;
            $c = imagecolorat($this->resource, 0, imagesy($this->resource)-1);
            if (isset($cc[$c]))
                $cc[$c] += 1;
            else
                $cc[$c] = 1;
            $c = imagecolorat($this->resource, imagesx($this->resource)-1, imagesy($this->resource)-1);
            if (isset($cc[$c]))
                $cc[$c] += 1;
            else
                $cc[$c] = 1;

            arsort($cc);
            foreach (array_keys($cc) as $color) {
                $params['color'] = $color;
                break;
            }
        }

        /* Initialise vars */
        $top = 0;
        $bottom = 0;
        $left = 0;
        $right = 0;

        /* Find top border */
        if ($params['sides'] == '' or stripos($params['sides'], 't') !== false) {
            for (; $top < imagesy($this->resource); ++$top) {
                for ($x = 0; $x < imagesx($this->resource); ++$x) {
                    if (imagecolorat($this->resource, $x, $top) != $params['color']) {
                        break 2;
                    }
                }
            }
        }

        /* Find bottom border */
        if ($params['sides'] == '' or stripos($params['sides'], 'b') !== false) {
            for (; $bottom < imagesy($this->resource); ++$bottom) {
                for ($x = 0; $x < imagesx($this->resource); ++$x) {
                    if (imagecolorat($this->resource, $x, imagesy($this->resource) - $bottom-1) != $params['color']) {
                        break 2;
                    }
                }
            }
        }

        /* Find left border */
        if ($params['sides'] == '' or stripos($params['sides'], 'l') !== false) {
            for (; $left < imagesx($this->resource); ++$left) {
                for ($y = 0; $y < imagesy($this->resource); ++$y) {
                    if (imagecolorat($this->resource, $left, $y) != $params['color']) {
                        break 2;
                    }
                }
            }
        }

        /* Find right border */
        if ($params['sides'] == '' or stripos($params['sides'], 'r') !== false) {
            for (; $right < imagesx($this->resource); ++$right) {
                for ($y = 0; $y < imagesy($this->resource); ++$y) {
                    if (imagecolorat($this->resource, imagesx($this->resource) - $right-1, $y) != $params['color']) {
                        break 2;
                    }
                }
            }
        }

        /* Compute new picture size, excluding the border */
        $width = imagesx($this->resource)-($left+$right);
        $height = imagesy($this->resource)-($top+$bottom);

        /* Define the new picture  */
        $newSrc = imagecreatetruecolor($width, $height);
        imagecopy($newSrc, $this->resource, 0, 0, $left, $top, imagesx($newSrc), imagesy($newSrc));
        imagedestroy($this->resource);
        $this->resource = $newSrc;

        /* Update sizes */
        $this->width = imagesx($this->resource);
        $this->height = imagesy($this->resource);

        return true;
    }

    /**
     * Trim rotate with GD.
     * @param array $params
     * @return boolean success
     */
    private function _gd_rotate($params) {
        if ($params['angle'] == 0)
            return true;
        if ($params['angle'] == 180) {
            $width = $this->width;
            $height = $this->height;
        } elseif (in_array($params['angle'], array(-90, 90, 270))) {
            $width = $this->height;
            $height = $this->width;
        } else if ($params['cropMode'] == 1) { // 1 : extends with bkgdColor
            $width = abs((int)($this->width*cos(deg2rad($params['angle'])) + $this->height*sin(deg2rad($params['angle']))));
            $height = abs((int)($this->width*sin(deg2rad($params['angle'])) + $this->height*cos(deg2rad($params['angle']))));
        } else if ($params['cropMode'] == 2) { // 2 : crop inside to avoid dead zones
            $temp = $this->height / ($this->width*sin(deg2rad($params['angle'])) + $this->height*cos(deg2rad($params['angle'])));
            $width = abs((int)($this->width*$temp));
            $height = abs((int)($this->height*$temp));
        } else { // cropMode 0 : no resize
            $width = $this->width;
            $height = $this->height;
        }

        /* Rotate picture */
        $this->resource = imagerotate($this->resource, $params['angle'], $this->_gd_getAllocatedColor($params['bkgdColor']), $params['ignoreTransparency']);

        /* Update sizes */
        $this->width = imagesx($this->resource);
        $this->height = imagesy($this->resource);

        /* Crop picture to follow expected cropmode */
        if ($width != $this->width)
            $this->crop($width, $height, 'C');

        /* Update sizes */
        $this->width = imagesx($this->resource);
        $this->height = imagesy($this->resource);

        /* Return success */
        return true;
    }

    /* GD sub methods */

    /**
     * Return allocated color for GD.
     * @param string $color
     * @param resource $resource
     * @return resource
     */
    private function _gd_getAllocatedColor($color, $resource=false) {

        /* Check color */
        if (preg_match('/#?([0-9A-Fa-f]{6})/', $color, $matches)) {
            $color = $matches[1];
            $r = hexdec(substr($color,0,2));
            $g = hexdec(substr($color,2,2));
            $b = hexdec(substr($color,4,2));
        } else {
            $r = 255; $g = 255; $b = 255;
        }

        if ($resource == false)
            return imagecolorallocate($this->resource, $r, $g, $b);
        return imagecolorallocate($resource, $r, $g, $b);
    }

    /**
     * Execute convolution for GD.
     * @param array $matrix
     * @param float $divisor
     * @param float $offset
     * @return boolean success
     */
    private function _gd_convolution($matrix, $divisor=false, $offset=0) {
        /* Check for convolution method (PHP > 5.1) */
        if (!function_exists("imageconvolution"))
            return false;

        /* Check matrix */
        if (count($matrix) != 3)
            return false;

        /* Check divisor, if required set neutral divisor */
        if ($divisor === false)
            $divisor = array_sum(array_map('array_sum', $matrix));

        /* Return result */
        return imageconvolution($this->resource, $matrix, $divisor, $offset);
    }

    /**
     * Check if picture has transparency enabled
     * @return void
     */
    private function _gd_is_transparencyEnabled() {
        return $this->hasTransparency;
    }

    /**
     * Add transparency support for current picture
     * @return void
     */
    private function _gd_add_transparency() {
        imageAlphaBlending($this->resource, true);
        imageSaveAlpha($this->resource, true);
        $this->hasTransparency = true;
    }

    /* Static public methods */

    /**
     * Return available engine list.
     * @return array
     */
    public static function get_availableEngines() {
        $list = array();

        foreach (self::KNOWNENGINES as $engine)
            if (extension_loaded($engine))
                $list[] = $engine;

        return $list;
    }
}
?>
