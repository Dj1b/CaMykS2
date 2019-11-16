<?php
/**
 * @brief CaMykS error library, initiated by ErrorHandler.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.0
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
     * Object constructor.
     * @param string $origin
     * @param string $type
     * @param integer $code
     * @param string $file
     * @param integer $line
     * @param string $message
     * @return void
     */
    public function __construct($origin, $type, $code, $file, $line, $message) {
        $this->origin = $origin;
        $this->type = $type;
        $this->code = $code;
        $this->file = $file;
        $this->line = $line;
        $this->message = $message;
    }

    /**
     * Export error as HTML.
     * @return string
     */
    public function export_asHTML() {
        return '<strong>'.$this->origin.' '.$this->type.' ['.$this->code.'] </strong>'.
            '<em>('.$this->file.' - L'.$this->line.')</em>'.$message;
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
