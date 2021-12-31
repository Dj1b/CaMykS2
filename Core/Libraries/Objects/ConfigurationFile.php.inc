<?php
/**
 * @brief ConfigurationFile object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4
 * @date Creation: Mar 2020
 * @date Modification: Dec 2021
 * @copyright 2020 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ConfigurationSaver class.
 */
final class ConfigurationFile {
    /**
     * @var boolean $status
     * @brief Stores data is loaded status.
     */
    private $status = false;

    /**
     * @var string $file
     * @brief Configuration file to use.
     */
    private $file = '';

    /**
     * @var string $source
     * @brief Configuration source.
     */
    private $source = '';

    /**
     * @var array $data
     * @brief Configuration data.
     */
    private $data = array();

    /**
     * @var string $fileType
     * @brief Stores configuration file type
     */
    private $fileType;

    /**
     * @var object $configurationFile.
     * @brief CaMykS File object to access file.
     */
    private $configurationFile;

    /**
     * Class constructor.
     * @param string $file
     * @param string $source
     * @return void
     */
    public function __construct($file, $source) {
        $this->file = $file;
        $this->source = $source;
    }

    /*  Public methods */

    /**
     * Load configuration file.
     * @return boolean success
     */
    public function load() {
        /* Load file */
        switch ($this->source) {
            case 'Engine': $this->load_engineFile(); break;
            case 'Website': $this->load_websiteFile(); break;
            case 'FullPath': $this->load_fullPathFile(); break;
        }

        /* Check file availability */
        if ($this->configurationFile->is_accessible()) {
            /* Load configuration data */
            switch($this->fileType) {
                case 'JSON': $this->load_JSONData(); break;
                case 'PHP': $this->load_PHPData(); break;
            }
        }

        /* Update status */
        $this->status = true;

        /* Return success */
        return $this->status;
    }

    /**
     * Save configuration file.
     * @return boolean success
     */
    public function save() {
        /* Check file is loaded */
        if (!$this->status)
            return false;

        /* Update file mode */
        $this->configurationFile->change_mode('Write');

        /* Check file type to save */
        switch ($this->fileType) {
            case 'JSON': return $this->save_JSONData();
            case 'PHP': return $this->save_PHPData();
        }

        /* Return default result */
        return false;
    }

    /**
     * Export data.
     * @return array
     */
    public function export() {
        if (!$this->status)
            $this->load();
        return $this->data;
    }

    /**
     * Define data value.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function define($name, $value) {
        if (!$this->status)
            $this->load();
        $this->data[$name] = $value;
    }

    /**
     * Merge data.
     * @param array $values
     * @return void
     */
    public function merge($values) {
        if (!$this->status)
            $this->load();
        $this->data = array_merge($this->data, $values);
    }

    /* Private load methods */

    /**
     * Load engine configuration file.
     * @return b
     */
    private function load_engineFile() {
        global $CaMykS;

        /* Load engine configuration file */
        $this->configurationFile = new File($CaMykS->get_engineFileFullPath('Core/Configuration/'.$this->file.CAMYKS_ASSETDATAEXTENSION));

        /* Define file type */
        $this->fileType = 'JSON';
    }

    /**
     * Load website configuration file.
     * @return void
     */
    private function load_websiteFile() {
        global $CaMykS;

        /* Load website configuration file */
        if ($this->file === 'Website') {
            $this->configurationFile = new File($CaMykS->get_websiteFileFullPath('Configuration/'.$this->file.CAMYKS_LIBRARYEXTENSION));
            $this->fileType = 'PHP';
        } else {
            $this->configurationFile = new File($CaMykS->get_websiteFileFullPath('Configuration/'.$this->file.CAMYKS_ASSETDATAEXTENSION));
            $this->fileType = 'JSON';
        }
    }

    /**
     * Load full path configuration file.
     * @return void
     */
    private function load_fullPathFile() {
        global $CaMykS;

        /* Load engine configuration file */
        $this->configurationFile = new File($this->file);

        /* Check configuration is a JSON file */
        if (preg_match('#'.string_escape4Regex(CAMYKS_ASSETDATAEXTENSION).'$#', $this->file))
            $this->fileType = 'JSON';

        /* Check configuration is a PHP file */
        elseif (preg_match('#'.string_escape4Regex(CAMYKS_LIBRARYEXTENSION).'$#', $this->file))
            $this->fileType = 'PHP';
    }

    /**
     * Load JSON data.
     * @return void
      */
    private function load_JSONData() {
        /* Load configuration data */
        $this->data = $this->configurationFile->get_JSONData(array());
    }

    /**
     * Load PHP data.
     * @return void
     */
    private function load_PHPData() {
        include $this->file;

        if (isset($configuration))
            $this->data = $configuration;
    }

    /* Private save methods */

    /**
     * Save JSON data in configuration file.
     * @return boolean result
     */
    private function save_JSONData() {
        global $CaMykS;

        /* Write configuration */
        return $this->configurationFile->write_JSONData($this->data, true);
    }

    /**
     * Save PHP data in configuration file.
     * @return boolean result
     */
    private function save_PHPData() {
        $content = '<?php'."\n";
        $content .= '$configuration = ';
        $content .= var_export($this->data, true);
        $content .= ';'."\n";
        $content .= '?>';

        return $this->configurationFile->write($content);
    }
}
?>
