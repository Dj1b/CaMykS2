<?php
/**
 * @brief AdminPage template.
 * @details Plugin / Template engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Feb 2020
 * @date Modification: Feb 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * AdminPage class.
 */
final class AdminPage extends Template {

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Specific methods */

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
     * Return header title.
     * @return string
     */
    protected function get_headerTitle() {
        global $CaMykS;

        switch ($CaMykS->mode) {
            case 2: /* Admin mode */
                return $CaMykS->get_locale('HeaderTitle', 'Admin', array($CaMykS->get_configValue('Website_Title')));
            case 3: /* Manager mode */
                return $CaMykS->get_locale('TemplateHeaderTitle', 'CaMykSManager');
            case 4: /* Install mode */

                break;
        }
    }
}
?>
