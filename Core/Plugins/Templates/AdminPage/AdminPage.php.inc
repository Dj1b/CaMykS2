<?php
/**
 * @brief AdminPage template.
 * @details Plugin / Template engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre5
 * @date Creation: Feb 2020
 * @date Modification: Jan 2022
 * @copyright 2020 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * AdminPage class.
 */
final class AdminPage extends Template {

    /**
     * @var boolean $actionBarStatus
     * @brief Is the action bar displayed.
     */
    protected $actionBarStatus = false;

    /**
     * @var array $actionBarButtons
     * @brief List of buttons to be displayed in action bar
     */
    protected $actionBarButtons = array();

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Plugin methods */

    /**
     * Initialise object.
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        global $CaMykS;
        parent::initialise($params);

        /* Load locales */
        $this->load_locales();

        /* Set up title on HTML page */
        $CaMykS->HTMLPage->set_pageInformation($this->get_headerTitle());

        if ($CaMykS->mode !== CAMYKS_MODE_INSTALL) {
            /* Load master scripts */
            $CaMykS->load_CaMykSJSScript();

            /* Load plugin scripts */
            $this->add_JSFile('AdminPage.js?v=1.0pre1', array('async'=>''));
            $this->add_JSFile('Locales/AdminPage.'.$CaMykS->get_currentLanguageValue().'.js?v=1.0pre0', array('async'=>''));
            $this->add_JSScript('AdminPage.set_param("LogoutURL", "'.$this->get_logoutLink().'");', true, 'Load');
            $this->add_JSScript('AdminPage.initialise();', true, 'Load');
        }
    }

    /* Public methods */

    /**
     * Enable action bar display.
     * @return void
     */
    public function enable_actionBar() {
        $this->actionBarStatus = true;
    }

    /**
     * Add a button to action bar
     * @param array $button
     * @return void
     */
    public function add_actionBarButton($button) {
        $this->actionBarButtons[] = $button;
    }

    /* Protected methods */

    /**
     * Check has a logo to display in header.
     * @return boolean result
     */
    protected function has_logo() {
        global $CaMykS;
        $logo = $CaMykS->get_configValue('Admin_HeaderLogoFile');
        return (!is_null($logo) and $logo !== '');
    }

    /**
     * Return header logo url.
     * @return string
     */
    protected function get_headerLogo() {
        global $CaMykS;
        return $CaMykS->get_configValue('Admin_HeaderLogoFile');
    }

    /**
     * Return header title.
     * @return string
     */
    protected function get_headerTitle() {
        global $CaMykS;

        switch ($CaMykS->mode) {
            case 2: /* Admin mode */
                return $this->get_locale('HeaderTitle', array($CaMykS->get_configValue('Website_Title')));
            case 3: /* Manager mode */
                return $CaMykS->get_locale('TemplateHeaderTitle', 'CaMykSManager');
            case 4: /* Install mode */

                break;
        }
    }

    /**
     * Return home link.
     * @return string
     */
    protected function get_homeLink() {
        global $CaMykS;

        switch ($CaMykS->mode) {
            case 2: /* Admin mode */

            case 3: /* Manager mode */
                return $CaMykS->manager->get_homeLink();
            case 4: /* Install mode */

                break;
        }
    }

    /**
     * Return logout link.
     * @return string
     */
    protected function get_logoutLink() {
        global $CaMykS;

        switch ($CaMykS->mode) {
            case 2: /* Admin mode */

            case 3: /* Manager mode */
                return $CaMykS->manager->get_logoutLink();
            case 4: /* Install mode */

                break;
        }
    }
}
?>
