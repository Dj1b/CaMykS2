<?php
/**
 * @brief CaMykS error library, initiated by ErrorHandler.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Nov 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Error class.
 */
final class Error {
    /**
     * @var string $origin
     * @brief Error origin.
     */
    private $origin;

    /**
     * @var string $type
     * @brief Error type.
     */
    private $type;

    /**
     * @var integer $code
     * @brief Error code.
     */
    private $code;

    /**
     * @var string $file
     * @brief File where error was initiated.
     */
    private $file;

    /**
     * @var integer $line
     * @brief Line where error was initiated.
     */
    private $line;

    /**
     * @var string $message
     * @brief Message associated with error.
     */
    private $message;

    /**
     * var string $details
     * @brief Details attached to message.
     */
    private $details;

    /**
     * Object constructor.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param integer $line
     * @param string $message
     * @return void
     */
    public function __construct($origin, $type, $code=0, $file='', $line=0, $message='', $details='') {
        $this->origin = $origin;
        $this->type = $type;
        $this->code = $code;
        $this->file = $file;
        $this->line = $line;
        $this->message = trim($message);
        $this->details = trim($details);
    }

    /**
     * Return error header value.
     * @return string
     */
    public function get_header() {
        $s = '';
        if ($this->origin != '')
            $s .= $this->origin.' ';
        if ($this->type != '') {
            $s .= $this->type;
            if ($this->code > 0)
                $s .= ' ['.$this->code.']';
        }
        return $s;
    }

    /**
     * Check error has location values.
     * @return boolean result
     */
    public function has_location() {
        return ($this->file != '');
    }

    /**
     * Return error location value.
     * @return string
     */
    public function get_location() {
        if (is_null($this->file))
            return '';
        $s = $this->file;
        if (!$this->line > 0)
            return $s;
        $s .= ' (L'.$this->line.')';
        return $s;
    }

    /**
     * Check error has a message.
     * @return boolean result
     */
    public function has_message() {
        return ($this->message != '');
    }

    /**
     * Return error message value.
     * @param boolean $HTMLFormat
     * @return string
     */
    public function get_message($HTMLFormat=false) {
        /* Build full message with details */
        $message = $this->message;
        if ($this->details !== '')
            $message .= ' ('.$this->details.')';

        /* Check format */
        if ($HTMLFormat)
            return nl2br($message);
        return $message;
    }

    /**
     * Export error as HTML.
     * @return string
     */
    public function export_asHTML() {
        $s = '<strong>'.$this->get_header().'</strong>';
        if ($this->has_location())
            $s .= ' - <em>'.$this->get_location().'</em>';
        if ($this->has_message())
            $s .= ' - '.$this->get_message(true);
        return $s;
    }

    /**
     * Export error as array.
     * @return array
     */
    public function export_asArray() {
        return get_object_vars($this);
    }
}
?>
