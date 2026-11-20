<?php
/**
 * @brief Module object.
 * Module abstract library, extends Plugin.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre12
 * @date Creation: Dec 2019
 * @date Modification: Nov 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * Module class.
 */
abstract class Module extends Plugin {
    /**
     * @var string|null $moduleType
     * @brief Module type, values are "Admin", "Content", "Tool".
     */
    protected ?string $moduleType = null;

    /**
     * @var array $libraries
     * @brief List of module internal libraries names.
     */
    protected array $libraries = array();

    /**
     * @var boolean $hasWorkflow
     * @brief Module manages workflow on its content objects.
     */
    protected bool $hasWorkflow = false;

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Override Plugin variables */
        $this->pluginType = 'Module';

        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Mark: Override Plugin methods */

    /**
     * Load module data.
     * @return void
     */
    protected function load_data(): void {
        /* Load module libraries */
        $this->load_internalLibraries();
    }

    /**
     * Install plugin.
     * @return boolean success
     */
    public function install(): bool {
        /* Check Plugin installation */
        if (!parent::install())
            return false;

        /* Do libraries installation */
        foreach ($this->libraries as $library) {
            $libraryInstance = $this->get_libraryInstance($library);
            if ($libraryInstance === false or !$libraryInstance->install()) {
                /* Add error */

                /* Return false */
                return false;
            }
        }

        /* Return success */
        return true;
    }

    /**
     * Uninstall plugin.
     * @return boolean result.
     */
    public function uninstall(): bool {
         /* Check Plugin installation */
        if (!parent::uninstall())
            return false;

        /* Do libraries installation */
        foreach ($this->libraries as $library) {
            $libraryInstance = $this->get_libraryInstance($library);
            if ($libraryInstance === false or !$libraryInstance->uninstall()) {
                /* Add error */

                /* Return false */
                return false;
            }
        }

        /* Return success */
        return true;
    }

    /**
     * Check plugin installation.
     * @return boolean result.
     */
    public function is_installed(): bool {
        global $CaMykS;

        /* Return plugin manager answer */
        foreach ($this->libraries as $library) {
            $libraryInstance = $this->get_libraryInstance($library);
            if ($libraryInstance === false or !$libraryInstance->is_installed())
                return false;
        }

        /* Return check success */
        return true;
    }

    /* Mark: Admin mode methods */

    /**
     * Initialise object in admin mode.
     * @param array $params
     * @return void
     */
    public function initialise_admin(array $params=array()): void {
        /* Do generic initialisation */
        $this->initialise($params);

        /* Check user access permission */
        /* @TODO */

        /* Load action to execute
        $action = ;
        */

        /* Execute action */
        $method = 'initialise_admin_'.$action;
        if (method_exists($this, $method))
            call_user_method($method, $this);
        else
            $this->initialise_admin_Main();
    }

    /**
     * Display object in admin mode.
     * @return void
     */
    public function display_admin(): void {
        /* Do generic display */
        parent::display();
    }

    /**
     * Initialise object admin/main mode.
     * @return void
     */
    protected function initialise_admin_Main(): void {

    }

    /* Mark: Configuration mode methods */

    /**
     * Return button for configuration panel.
     * @return false|array
     */
    public function get_adminConfigurationPanelButton(): false|array {
        return false;

        return array(
            /*
            array(
                'title'         => '',
                'icon'          => '',
                'category'      => '',
                'categoryTitle' => '', // Optional : to be defined only for non-generic categories
            ),
            */
        );
    }

    /**
     * Initialise object in view configuration mode.
     * @return void
     */
    protected function initialise_configuration_view(): void {

    }

    /**
     * Display object in view configuration mode.
     * @return void
     */
    protected function display_configuration_view(): void {

    }

    /**
     * Initialise object in edit configuration mode.
     * @return void
     */
    protected function initialise_configuration_edit(): void {

    }

    /**
     * Display object in edit configuration mode.
     * @return void
     */
    protected function display_configuration_edit(): void {

    }

    /* Mark: Library methods */

    /**
     * Return library instance.
     * @param string $library
     * @param mixed $objectParam
     * @return PluginLibrary|false
     */
    public function get_libraryInstance(string $library, mixed $objectParam=null): PluginLibrary|false {
        /* Check library is declared in module */
        if (!in_array($library, $this->libraries))
            return false;

        /* Compute library class name */
        $libraryFullName = $this->libraryNamespace.'\\'.$library;

        /* Check fallback to legacy prefixed class name */
        if (!class_exists($libraryFullName))
            $libraryFullName = $this->libraryNamespace.'\\'.$this->libraryName.$library;

        /* Check class exists, or attempt to load library file */
        if (!class_exists($libraryFullName)) {
            if (!$this->load_library($library))
                return false;

            $libraryFullName = $this->libraryNamespace.'\\'.$library;
            if (!class_exists($libraryFullName))
                $libraryFullName = $this->libraryNamespace.'\\'.$this->libraryName.$library;
        }

        /* Check class exists */
        if (!class_exists($libraryFullName))
            return false;

        /* Return new instance */
        if (is_null($objectParam) || $objectParam === 0)
            return new $libraryFullName($this);
        return new $libraryFullName($this, $objectParam);
    }

    /* Mark: Workflow methods */

    /**
     * Check if module manages workflow.
     * @return boolean result
     */
    public function has_workflow(): bool {
        return $this->hasWorkflow;
    }

    /**
     * Create a new workflow version for given content object.
     * @param integer $contentId
     * @param string|null $library
     * @return WorkflowModuleLibrary|WorkflowContentModuleLibrary|false
     */
    public function create_contentNewVersion(int $contentId=0, ?string $library=null): WorkflowModuleLibrary|WorkflowContentModuleLibrary|false {
        if (!$this->has_workflow() || $contentId <= 0)
            return false;

        /* Determine target content library */
        if (is_null($library)) {
            foreach ($this->libraries as $libName) {
                if ($libName !== 'Configuration' && strpos($libName, '\\') === false) {
                    $library = $libName;
                    break;
                }
            }
        }
        if (is_null($library))
            return false;

        /* Load content instance and create new version */
        $instance = $this->get_libraryInstance($library, $contentId);
        if (!($instance instanceof WorkflowModuleLibrary) && !($instance instanceof WorkflowContentModuleLibrary))
            return false;

        if (!$instance->load(true))
            return false;

        return $instance->create_newVersion(true);
    }

    /**
     * Update workflow status for given content object.
     * @param integer $contentId
     * @param integer $wfStatus
     * @param string|null $library
     * @return boolean success
     */
    public function update_contentWFStatus(int $contentId, int $wfStatus, ?string $library=null): bool {
        if (!$this->has_workflow() || $contentId <= 0)
            return false;

        /* Determine target content library */
        if (is_null($library)) {
            foreach ($this->libraries as $libName) {
                if ($libName !== 'Configuration' && strpos($libName, '\\') === false) {
                    $library = $libName;
                    break;
                }
            }
        }
        if (is_null($library))
            return false;

        /* Load content instance and update workflow status */
        $instance = $this->get_libraryInstance($library, $contentId);
        if ($instance instanceof WorkflowContentModuleLibrary) {
            if (!$instance->load(true))
                return false;
            return $instance->update_wfStatus($wfStatus);
        }

        if ($instance instanceof WorkflowModuleLibrary) {
            if (!$instance->load(true))
                return false;
            if ($wfStatus === 6)
                return $instance->publish_version();
            if ($wfStatus === 21)
                return $instance->archive_version();

            $instance->WFStatus = $wfStatus;
            $instance->Status   = 1;
            return $instance->save();
        }

        return false;
    }

    /* Mark: Private methods */

    /**
     * Load libraries.
     * @return boolean success
     */
    private function load_internalLibraries(): bool {
        foreach ($this->libraries as $library) {
            /* Load library */
            if (!$this->load_library($library)) {
                /* Log error */
                /* @TODO */

                /* Return failure */
                return false;
            }
        }
        return true;
    }
}
?>
