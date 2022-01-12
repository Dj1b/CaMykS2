<?php
/**
 * @brief MediaPopup component.
 * <br />Component to create media popup.
 * @details Plugin / Component Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Nov 2021
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @details Accepted media :
 * <br />- Picture: .gif, .jpg, .png, .svg
 * <br />- Video: .mp4, .m4v
 * <br />- Youtube video: https://youtu.be/xxxxxxxxxxxx
 * <br />- Direct content (including HTML): content:xxxxxxxx
 * <br />- Web page (in a frame): https://www.xxxxx.xxx/xxxxx
 * <br />- DOM Element: DOMElement:DOMElementId
 */

namespace CaMykS2;

/**
 * MediaPopup class.
 */
final class MediaPopup extends Component {
    /**
     * @var boolean $hasConfiguration
     * @brief Plugin has a configuration.
     */
    protected $hasConfiguration = true;

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        global $CaMykS;

        /* Execute parent construct */
        parent::__construct($location);

        /* Check auto loading in visit mode */
        if ($CaMykS->mode === CAMYKS_MODE_VISIT) {
            /* Initialise component to load configuration */
            $this->initialise();

            /* Check auto loading */
            if ($this->get_configValue('Integration_Mode') == 1)
                $this->plug();
        }
    }

    /* Override Component methods */

    /**
     * Plug component to page.
     * @return void
     */
    public function plug($params=array()) {
        global $CaMykS;

        /* Perform parent plug action */
        parent::plug($params);

        /* Merge params in configuration */
        $this->configuration->load_fromArray($this->params, true);

        /* Adds styles */
        if ($this->get_configValue('Integration_StyleSource') === 'theme')
            $CaMykS->currentTheme->add_pluginStyleFile('MediaPopup');
        else
            $this->add_styleFile('MediaPopup.css?v=1.0pre0');

        /* Add scripts */
        $CaMykS->load_engineJSScript('dom.js', 'Tools');
        $this->add_JSFile('MediaPopup.js?v=1.0pre1', array('async'=>''));
        $this->add_JSScript('MediaPopup.set_param("EnableBackgroundAction", '.$this->get_configValue('Control_ClickBkgdToClose').');', true, 'Load');
        $this->add_JSScript('MediaPopup.set_param("EnableKeyboardActions", '.$this->get_configValue('Control_EnableKeyboard').');', true, 'Load');
        $this->add_JSScript('MediaPopup.set_param("EnableDiaporama", '.$this->get_configValue('Diaporama_Enable').');', true, 'Load');
        $this->add_JSScript('MediaPopup.initialise();', true, 'Load');
        $this->add_JSScript('MediaPopup.update_mediaSize();', true, 'Resize');
    }
}
?>
