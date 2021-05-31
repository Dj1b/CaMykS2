<?php
/**
 * @brief CaMykSInstaller special plugin.
 * @details Plugin / Plugin Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: May 2021
 * @date Modification: May 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * CaMykSInstaller class.
 */
final class CaMykSInstaller extends Plugin {
    /**
     * @var string $action
     * @brief Current action to execute in installer.
     */
    protected $action = 'Welcome';

    /**
     * @var Object $theme
     * @brief Link to current CaMykS theme.
     */
    protected $theme;

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Special';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /**
     * Initialise plugin (in manager mode).
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        global $CaMykS;
        parent::initialise($params);

        /* Load locales */
        $this->load_locales('', $CaMykS->currentLanguage);

        /* Apply title */
        $CaMykS->HTMLPage->set_pageInformation($this->get_locale('TemplateHeaderTitle'));

        /* Load theme */
        $this->theme = &$CaMykS->currentTheme;

        /* Load action to execute */
        $this->action = http_getStringRequest('InstallerAction', 'Welcome');

        /* Initialise action to execute */
        switch ($this->action) {
            case 'DisplayWebsiteConfiguration': /* Display website configuration */
                $this->initialise_DisplayWebsiteConfiguration();
                break;
            case 'SaveWebsiteConfiguration': /* Save website configuration */
                $this->initialise_SaveWebsiteConfiguration();
                break;

            case 'DisplayVisitConfiguration': /* Display visit configuration */
                $this->initialise_DisplayVisitConfiguration();
                break;
            case 'SaveVisitConfiguration': /* Save visit configuration */
                $this->initialise_SaveVisitConfiguration();
                break;

            case 'DisplayAdminConfiguration': /* Display admin configuration */
                $this->initialise_DisplayAdminConfiguration();
                break;
            case 'SaveAdminConfiguration': /* Save admin configuration */
                $this->initialise_SaveAdminConfiguration();
                break;

            case 'DisplayDatabaseConfiguration': /* Display database configuration */
                $this->initialise_DisplayDatabaseConfiguration();
                break;
            case 'SaveDatabaseConfiguration': /* Save database configuration */
                $this->initialise_SaveDatabaseConfiguration();
                break;

            case 'Welcome': /* Main, default */
            default:
                $this->initialise_Welcome();
        }
    }

    /* Installation action methods */

    /**
     * Initialise plugin in Welcome mode.
     * @return void
     */
    private function initialise_Welcome() {
        global $CaMykS;

        /* Update layout */
        $this->update_selectedLayout('Welcome');
    }
}
?>
