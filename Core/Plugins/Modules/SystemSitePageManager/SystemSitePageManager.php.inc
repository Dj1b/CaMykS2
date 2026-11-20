<?php
/**
 * @brief SystemSitePageManager admin module.
 * <br />Module to manage website pages.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Jun 2021
 * @date Modification: Nov 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemSitePageManager;

use CaMykS2\AdminModule;

/**
 * SystemSitePageManager class.
 */
final class SystemSitePageManager extends AdminModule {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected bool $hasConfiguration = true;

    /**
     * @var boolean $hasWorkflow
     * @brief Module manages workflow on its content objects.
     */
    protected bool $hasWorkflow = true;

    /**
     * @var array $pluginExtensions
     * @brief List of plugins which it provides an extension.
     */
    protected array $pluginExtensions = array('SystemWorkflowManager');

    /**
     * @var array $libraries
     * @brief List of module internal libraries names.
     */
    protected array $libraries = array('Page', 'Page\\Block', 'Page\\Meta', 'Configuration');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Installation specific methods */

    /**
     * Create initial website page during website installation.
     * @param array $languages
     * @return boolean success
     */
    public function create_initialPage(array $languages=array()): bool {
        global $CaMykS;

        /* Check if a page already exists */
        $page = $this->get_libraryInstance('Page');
        if ($page === false or $page->get_count() > 0)
            return false;

        /* Define languages */
        if (empty($languages)) {
            $languages = isset($CaMykS) ? $CaMykS->get_siteLanguages() : array('fr');
        }

        /* Define base page attributes */
        $page->Status   = 2; // Enabled
        $page->ParentId = 0; // First level
        $page->Position = 1;

        /* Define localized attributes for each language */
        foreach ($languages as $language) {
            $this->load_locales('', $language);
            $name  = $this->get_locale('FirstPage_Name');
            $title = $this->get_locale('FirstPage_Title');

            $nameAttribute = $page->get_attribute('Name');
            if ($nameAttribute !== false)
                $nameAttribute->set_value($name, $language);

            $titleAttribute = $page->get_attribute('Title');
            if ($titleAttribute !== false)
                $titleAttribute->set_value($title, $language);
        }

        /* Save page */
        return $page->save();
    }
}
?>
