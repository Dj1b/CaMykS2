<?php
/**
 * @brief CaMykS error library, initiated by ErrorHandler.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre3.1
 * @date Creation: Nov 2019
 * @date Modification: Jan 2021
 * @copyright 2019 - 2021 CaMykS Team
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
    private $file = '';

    /**
     * @var integer $line
     * @brief Line where error was initiated.
     */
    private $line = 0;

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
     * var integer $count
     * @brief Count error occurences.
     */
    private $count = 1;

    /**
     * var array $backtrace
     * @brief Stored generated backtrace.
     */
    private $backtrace;

    /**
     * Object constructor.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param integer $line
     * @param string $message
     * @param string $details
     * @param array $backtrace
     * @return void
     */
    public function __construct($origin, $type, $code=0, $file='', $line=0, $message='', $details='', $backtrace=array()) {
        $this->origin = $origin;
        $this->type = $type;
        $this->code = $code;
        $this->file = $file;
        $this->line = $line;
        $this->message = trim($message);
        $this->details = trim($details);
        $this->backtrace = $backtrace;
    }

    /* Data related methods */

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
        if ($this->count > 1)
            $s .= ' x'.$this->count;
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
     * Check error has a backtrace.
     * @return boolean result
     */
    public function has_backtrace() {
        return (count($this->backtrace) > 0);
    }

    /**
     * Return error trace value.
     * @param boolean $HTMLFormat
     * @return string
     */
    public function get_backtrace($HTMLFormat=false) {

        $lines = array();

        $cnt = 0;
        foreach($this->backtrace as $trace) {
            $m = ++$cnt.'/ ';
            if (isset($trace['file'])) {
                $m .= $trace['file'].' ';
                if (isset($trace['line']))
                    $m .= '(Line '.$trace['line'].') ';
                $m .= '- ';
            }

            if (isset($trace['class']))
                $m .= $trace['class'].' '.$trace['type'];

            if (isset($trace['function']))
                $m .= $trace['function'];

            if (isset($trace['args']))
                $m .= '('.implode(', ', $trace['args']).')';

            $lines[] = $m;
        }

        if ($HTMLFormat)
            return '<br />&nbsp;&nbsp;&nbsp;&nbsp;'.implode('<br />&nbsp;&nbsp;&nbsp;&nbsp;', $lines);
        return "\n\t".implode("\n\t", $lines);
    }

    /**
     * Update error count.
     * return void
     */
    public function update_count() {
        $this->count ++;
    }

    /* Check methods */

    /**
     * Check error is similar.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param integer $line
     * @param string $message
     * @return boolean success
     */
    public function is_similar($origin, $type, $code=0, $file='', $line=0, $message='') {
        if ($this->origin != $origin)
            return false;

        if ($this->type != $type)
            return false;

        if ($this->code != $code)
            return false;

        if ($this->file != $file)
            return false;

        if ($this->line != $line)
            return false;

        if ($this->message != trim($message))
            return false;

        /* All test failed, return error is similar */
        return true;
    }

    /* Export methods */

    /**
     * Export error as HTML.
     * @return string
     */
    public function export_asHTML() {
        $s = '<strong style="color:inherit;">'.$this->get_header();
        if (!$this->has_backtrace() and $this->has_location())
            $s .= ' - <em style="color:inherit;">'.$this->get_location().'</em>';
        if ($this->has_message())
            $s .= ' - '.$this->get_message(true);
        $s .= '</strong>';
        if ($this->has_backtrace())
            $s .= $this->get_backtrace(true);

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
