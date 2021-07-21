<?php
/**
 * @brief SystemConfigurationManager admin module.
 * <br />Module to manage website configuration.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jun 2021
 * @date Modification: Jun 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemConfigurationManager class.
 */
final class SystemConfigurationManager extends AdminModule {
    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Module methods */

    /* Configuration data methods */

    /**
     * Return encoding values.
     * @return array
     */
    public function get_encodingValues() {
        return array('UTF-8'=>'UTF-8');
    }

    /**
     * Return CMS Signature values.
     * @return array
     */
    public function get_displayCMSSignatureValues() {
        return array(
            0 => $this->get_locale('DisplayCMSSignature_Hidden'),
            1 => $this->get_locale('DisplayCMSSignature_Display'),
        );
    }

    /**
     * Return available themes titles, compatible with admin mode.
     * @return array
     */
    public function get_availableAdminThemesTitles() {
        global $CaMykS;

        /* Initialise result */
        $result = array();

        /* Load available themes from CMS */
        $themes = $CaMykS->get_availableThemes();

        /* Check themes that support admin mode */
        foreach ($themes as $name => $theme)
            if ($theme->support_adminMode())
                $result[$name] = $name;

        /* Return result */
        return $result;
    }

    /**
     * Return available themes titles, compatible with visit mode.
     * @return array
     */
    public function get_availableVisitThemesTitles() {
        global $CaMykS;

        /* Initialise result */
        $result = array();

        /* Load available themes from CMS */
        $themes = $CaMykS->get_availableThemes();

        /* Check themes that support visit mode */
        foreach ($themes as $name => $theme)
            if ($theme->support_visitMode())
                $result[$name] = $name;

        /* Return result */
        return $result;
    }

    /**
     * Return available templates titles, compatible with visit mode.
     * @return array
     */
    public function get_availableVisitTemplatesTitles() {
        global $CaMykS;

        /* Initialise result */
        $result = array();

        /* Load available templates from CMS */
        $templates = $CaMykS->get_availableTemplates();

        /* Check themes that support visit mode */
        foreach ($templates as $name => $template)
            if ($template->libraryName !== 'AdminPage')
                $result[$name] = $name;

        /* Return result */
        return $result;
    }
}
?>
