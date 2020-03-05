<?php
/**
 * @brief ConfigurationFile object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ConfigurationSaver class.
 */
final class ConfigurationFile {

    /*  Public methods */

    /**
     * Load configuration file.
     * @param string $file
     * @param string $source
     * @return array
     */
    public function load($file, $source) {
        switch ($source) {
            case 'Engine': return $this->load_engineFile($file);
            case 'Website': return $this->load_websiteFile($file);
        }

        /* Return default value */
        return array();
    }

    /**
     * Save configuration file.
     * @param string $file
     * @param string $source
     * @param array $params
     * @return boolean success
     */
    public function save($file, $source, $params) {
        switch ($source) {
            case 'Engine': return $this->save_engineFile($file, $params);
            case 'Website': return $this->save_websiteFile($file, $params);
        }

        /* Return default result */
        return false;
    }

    /* Private load methods */

    /**
     * Load engine configuration file.
     * @param string $file
     * @return array
     */
    private function load_engineFile($file) {
        global $CaMykS;

        /* Load engine configuration file */
        $file = 'Core/Configuration/'.$file.$CaMykS->libraryExtension;
        if ($CaMykS->check_engineFileExists($file)) {
            require $CaMykS->get_engineFileFullPath($file);
            if (isset($configuration) and is_array($configuration))
                return $configuration;
        }

        /* Return default value */
        return array();
    }

    /**
     * Load website configuration file.
     * @param string $file
     * @return array
     */
    private function load_websiteFile($file) {
        global $CaMykS;

        /* Load website configuration file */
        $file = 'Configuration/'.$file.$CaMykS->libraryExtension;
        if ($CaMykS->check_websiteFileExists($file)) {
            require $CaMykS->get_websiteFileFullPath($file);
            if (isset($configuration) and is_array($configuration))
                return $configuration;
        }

        /* Return default value */
        return array();
    }

    /* Private save methods */

    /**
     * Save engine configuration file.
     * @param string $file
     * @param array $params
     * @return boolean result
     */
    private function save_engineFile($file, $params) {
        global $CaMykS;

        /* Check file is available in engine */
        if ($file == 'Website') return false;

        /* Load current configuration and merge it with new params */
        $params = array_merge($this->load_engineFile($file), $params);

        /* Build content */
        $content = "<?php \n";
        $content .= '$configuration = '.var_export($params, true).";\n";
        $content .= "?>\n";

        /* Build file full path */
        $file = new File($CaMykS->get_engineFileFullPath('Core/Configuration/'.$file.$CaMykS->libraryExtension), '', 'Write');

        /* Check file is writable */
        if ($file->is_writable() === false)
            return false;

        /* Write configuration */
        return $file->write($content, true);
    }

    /**
     * Save website configuration file.
     * @param string $file
     * @param array $params
     * @return boolean result
     */
    private function save_websiteFile($file, $params) {

    }
}
?>
