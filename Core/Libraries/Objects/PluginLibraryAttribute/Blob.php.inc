<?php
/**
 * @brief BlobPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2026
 * @date Modification: Feb 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * BlobPluginLibraryAttribute class.
 */
class BlobPluginLibraryAttribute extends PluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array(
        'Format'        => 'Free',      // HTML, MD, JSON, Free
        'Size'          => 'Small',     // Small, Medium, Large
        'StripPHPTags'  => true,
        'StripJSTags'   => true,
        'AutoTrim'      => false,
    );

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Return read value from save.
     * @param mixed $value
     * @return mixed
     */
    protected function read_valueFromSave(mixed $value) {
        return stripSlashes($value);
    }

    /**
     * Return optimise value for save.
     * @param mixed $value
     * @return mixed
     */
    protected function optimise_valueToSave(mixed $value) {
        return addSlashes($value);
    }

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        /* Check format-specific validation */
        switch ($this->params['Format']) {
            case 'HTML':
                /* Strip dangerous tags */
                if ($this->params['StripPHPTags'])
                    $this->value = preg_replace(array('/\<\?[^\?\>]*\?\>/', '/\<\?/', '/\<qss[^\>]*\>/', '/\<qss/'), '', $this->value);
                if ($this->params['StripJSTags'])
                    $this->value = preg_replace(array('/\<script[^\>]*\>[^\<]\<\/script\>/', '/\<script/'), '', $this->value);
                break;

            case 'JSON':
                /* Validate JSON syntax */
                if ($this->value !== '') {
                    json_decode($this->value);
                    if (json_last_error() !== JSON_ERROR_NONE) {
                        return false;
                    }
                }
                break;

            case 'MD':
            case 'Free':
                /* Minimal validation - strip PHP tags only */
                if ($this->params['StripPHPTags'])
                    $this->value = preg_replace(array('/\<\?[^\?\>]*\?\>/', '/\<\?/', '/\<qss[^\>]*\>/', '/\<qss/'), '', $this->value);
                break;
        }

        /* Check autotrim */
        if ($this->params['AutoTrim'])
            $this->value = trim($this->value);

        /* Check size limit */
        $maxSize = $this->get_maxSize();
        if (strlen($this->value) > $maxSize) {
            $this->value = substr($this->value, 0, $maxSize);
        }

        /* Return value is conform */
        return true;
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters() {
        /* Get database type based on size */
        $dbType = $this->get_databaseType();

        return array(
            /* Generic values */
            'Type'      => $dbType,
            'Default'   => $this->defaultValue,
            'IsUnique'  => $this->params['IsUnique'],

            /* Primary key and index settings */
            'IndexName'     => $this->params['IndexName'],
            'IndexLength'   => $this->params['IndexLength'],
        );
    }

    /* Mark: Specific tool methods */

    /**
     * Return maximum size in bytes based on Size parameter.
     * @return integer
     */
    private function get_maxSize() {
        switch ($this->params['Size']) {
            case 'Small':
                return 65535;       // ~64 KB
            case 'Medium':
                return 16777215;    // ~16 MB
            case 'Large':
                return 4294967295;  // ~4 GB
            default:
                return 65535;       // Default to Small
        }
    }

    /**
     * Return database type based on Size parameter.
     * @return string
     */
    private function get_databaseType() {
        switch ($this->params['Size']) {
            case 'Small':
                return 'text';
            case 'Medium':
                return 'mediumtext';
            case 'Large':
                return 'longtext';
            default:
                return 'text';      // Default to Small
        }
    }
}
?>
