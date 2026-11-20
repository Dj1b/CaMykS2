<?php
/**
 * @brief SystemSitePageManager admin module, Configuration library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemSitePageManager;

use CaMykS2\PluginConfiguration;

/**
 * Configuration class.
 */
final class Configuration extends PluginConfiguration {
    /**
     * Class constructor.
     * @param mixed $parent
     * @param string $name
     * @return void
     */
    public function __construct(&$parent, string $name='config') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* Navigation Attributes */
        $this->define_attribute('Navigation_MainRootPage',          'Integer', 1);
        $this->define_attribute('Navigation_AltRootPage',           'Integer', 0);
        $this->define_attribute('Navigation_AdditionalRootPages',   'List',    array());
        $this->define_attribute('Navigation_KeyPages',              'List',    array());

        /* 404 Page handling Attributes */
        $this->define_attribute('Page404_Fallback',                 'Text',    'DisplayHome');
        $this->define_attribute('Page404_ErrorFile',                'Text',    '');
        $this->define_attribute('Page404_ErrorPage',                'Integer', 0);
        $this->define_attribute('Page404_SendCodeHeader',           'Boolean', false);

        /* Social media Attributes */
        $this->define_attribute('Social_ExportOGMeta',              'Boolean', false);
        $this->define_attribute('Social_ExportTwitterCards',        'Boolean', false);
        $this->define_attribute('Social_ExportPWAManifest',         'Boolean', false);
        $this->define_attribute('Social_ExportLD+JSON',             'Boolean', false);
        $this->define_attribute('Social_ExportHreflang',            'Boolean', true);
    }
}
?>
