<?php
/**
 * @brief Theme object.
 * <br />Theme abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Dec 2019
 * @date Modification: Dec 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Theme class.
 */
abstract class Theme extends Plugin {
    /**
     * @var boolean $supportAdminMode
     * @brief Is theme capable to design admin mode.
     */
    private $supportAdminMode = false;

    /**
     * @var boolean $supportManagerMode
     * @brief Is theme capable to design manager mode.
     */
    private $supportManagerMode = false;

    /**
     * @var boolean $supportVisitMode
     * @brief Is theme capable to design visit mode.
     */
    private $supportVisitMode = true;

    /**
     * @var array $supportPluginList
     * @brief List of plugins supported by theme.
     */
    private $supportPluginList = array();

    /**
     * @var string $shortName
     * @brief Short name for graphical element classes.
     */
    protected $shortName;

    /**
     * @var array $layoutContext
     * @brief Temporary element to display in layouts.
     */
    protected $layoutContext;

    /**
     * @var array $pluginStyleFiles
     * @brief List all available plugins style files.
     */
    protected $pluginStyleFiles = array();

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Override Plugin variables */
        $this->pluginType = 'Theme';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Plugin methods */

    /**
     * Initialise theme.
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        parent::initialise($params);

        /* Build element base name */
        $this->shortName = preg_replace('/(Style|Design)$/', '', $this->libraryName);
    }

    /* Check methods */

    /**
     * Check if theme supports visit mode.
     * @return boolean result
     */
    public function support_visitMode() {
        return (count($this->styleFiles) > 0);
    }

    /**
     * Check if theme support admin mode.
     * @return boolan result
     */
    public function support_adminMode() {
        return (count($this->adminStyleFiles) > 0);
    }

    /* Graphical elements building */

    /**
     * Create and display a button.
     * @param array $params
     * @return boolean success.
     */
    public function display_button($params=array()) {
        global $CaMykS;

        /* Build default params */
        $defaultParams = array(
            'Title'     => '',
            'Link'      => '',
            'Id'        => '',
            'Target'    => '', // | _blank | _parent
            'Size'      => '', // | Small | Large
            'Color'     => '',
            'Classes'   => '',
            'Styles'    => '',
        );

        /* Merge params */
        $this->layoutContext = array_merge($defaultParams, $params);

        /* Build class */
        $classes = $this->shortName.'Button';
        if ($this->layoutContext['Size'] != '')
            $classes .= ' '.$this->layoutContext['Size'];
        unset($this->layoutContext['Size']);
        if ($this->layoutContext['Color'] != '')
            $classes .= ' '.$this->layoutContext['Color'];
        unset($this->layoutContext['Color']);
        if ($this->layoutContext['Classes'] != '')
            $classes .= ' '.$this->layoutContext['Classes'];
        $this->layoutContext['Classes'] = $classes;

        /* Try to load Theme layout */
        if ($this->load_layout('Button'))
            return true;

        /* Load generic layout */
        return $CaMykS->load_assetLayout('Theme/Button', $this->layoutContext);
    }

    /* Plugin related methods */

    /**
     * Attach a plugin specific style file to HTML page.
     * @param string $plugin
     * @param string $condition
     * @param string $URLMode
     * @return void
     */
    public function add_pluginStyleFile($plugin, $condition='', $URLMode=null) {
        if (isset($this->pluginStyleFiles[$plugin]))
            $this->add_styleFile('Plugins/'.$this->pluginStyleFiles[$plugin], $condition, $URLMode);
    }
}
?>
