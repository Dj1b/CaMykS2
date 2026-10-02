<?php
/**
 * @brief ConfigurationFile object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre7
 * @date Creation: Mar 2020
 * @date Modification: Oct 2026
 * @copyright 2020 - 2026 CaMykS Team
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
    private bool $status = false;

    /**
     * @var string $file
     * @brief Configuration file to use.
     */
    private string $file = '';

    /**
     * @var string $source
     * @brief Configuration source.
     */
    private string $source = '';

    /**
     * @var string $mode
     * @brief Configuration file opening mode.
     */
    private string $mode = '';

    /**
     * @var array $data
     * @brief Configuration data.
     */
    private array $data = array();

    /**
     * @var string $fileType
     * @brief Stores configuration file type
     */
    private string $fileType;

    /**
     * @var File $configurationFile.
     * @brief CaMykS File object to access file.
     */
    private File $configurationFile;

    /**
     * Class constructor.
     * @param string $file
     * @param string $source
     * @param string $mode
     * @return void
     */
    public function __construct(string $file, string $source, string $mode='Read') {
        $this->file = $file;
        $this->source = $source;
        $this->mode = $mode;
    }

    /* Mark:  Public methods */

    /**
     * Load configuration file.
     * @return boolean success
     */
    public function load(): bool {
        /* Load file */
        switch ($this->source) {
            case 'Engine': $this->load_engineFile(); break;
            case 'Website': $this->load_websiteFile(); break;
            case 'FullPath': $this->load_fullPathFile(); break;
        }

        /* Check file availability */
        if (isset($this->configurationFile) and $this->configurationFile->is_accessible('Read')) {
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
    public function save(): bool {
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
    public function export(): array {
        if (!$this->status)
            $this->load();
        return $this->data;
    }

    /**
     * Return data value.
     * @param string $name
     * @return mixed
     */
    public function value(string $name): mixed {
        if (!$this->status)
            $this->load();
        if (!isset($this->data[$name]))
            return null;
        return $this->data[$name];
    }

    /**
     * Define data value.
     * @param string $name
     * @param mixed $value
     * @return void
     */
    public function define(string $name, mixed $value): void {
        if (!$this->status)
            $this->load();
        $this->data[$name] = $value;
    }

    /**
     * Merge data.
     * @param array $values
     * @return void
     */
    public function merge(array $values): void {
        if (!$this->status)
            $this->load();
        $this->data = array_merge($this->data, $values);
    }

    /* Mark: Private load methods */

    /**
     * Load engine configuration file.
     * @return void
     */
    private function load_engineFile(): void {
        global $CaMykS;

        /* Load engine configuration file */
        $this->configurationFile = new File($CaMykS->get_engineFileFullPath('Core/Configuration'), $this->file.CAMYKS_ASSETDATAEXTENSION, $this->mode);

        /* Define file type */
        $this->fileType = 'JSON';
    }

    /**
     * Load website configuration file.
     * @return void
     */
    private function load_websiteFile(): void {
        global $CaMykS;

        /* Load website configuration file */
        if ($this->file === 'Website') {
            $this->configurationFile = new File($CaMykS->get_websiteFileFullPath('Configuration'), $this->file.CAMYKS_LIBRARYEXTENSION, $this->mode);
            $this->fileType = 'PHP';
        } else {
            $this->configurationFile = new File($CaMykS->get_websiteFileFullPath('Configuration'), $this->file.CAMYKS_ASSETDATAEXTENSION, $this->mode);
            $this->fileType = 'JSON';
        }
    }

    /**
     * Load full path configuration file.
     * @return void
     */
    private function load_fullPathFile(): void {
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
    private function load_JSONData(): void {
        /* Load configuration data */
        $this->data = $this->configurationFile->get_JSONData(array());
    }

    /**
     * Load PHP data.
     * @return void
     */
    private function load_PHPData(): void {
        include $this->configurationFile->get_filePath();

        if (isset($configuration))
            $this->data = $configuration;
    }

    /* Mark: Private save methods */

    /**
     * Save JSON data in configuration file.
     * @return boolean result
     */
    private function save_JSONData(): bool {
        global $CaMykS;

        /* Write configuration */
        return $this->configurationFile->write_JSONData($this->data, true);
    }

    /**
     * Save PHP data in configuration file.
     * @return boolean result
     */
    private function save_PHPData(): bool {
        $content = '<?php'."\n";
        $content .= '$configuration = ';
        $content .= var_export($this->data, true);
        $content .= ';'."\n";
        $content .= '?>';

        return $this->configurationFile->write($content);
    }
}
?>
