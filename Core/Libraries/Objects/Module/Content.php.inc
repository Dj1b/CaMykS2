<?php
/**
 * @brief ContentModule object.
 * <br />Sub module abstract library for content modules.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Dec 2020
 * @date Modification: Nov 2026
 * @copyright 2020 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ContentModule class.
 */
abstract class ContentModule extends Module {
    /**
     * @var string|null $contentLibrary
     * @brief Name of the main content library managed by this module, or null if none.
     */
    protected ?string $contentLibrary = null;

    /**
     * @var ModuleLibrary|null $currentContent
     * @brief Currently active content object instance (for single-instance modules or active block in multi-instance).
     */
    protected ?ModuleLibrary $currentContent = null;

    /**
     * @var array $blockContents
     * @brief Loaded content object instances indexed by blockIndex.
     */
    protected array $blockContents = array();

    /**
     * @var array $blockLayouts
     * @brief Selected layout names indexed by blockIndex.
     */
    protected array $blockLayouts = array();

    /**
     * Class constructor.
     * @param string|null $location
     * @return void
     */
    public function __construct(?string $location=null) {
        /* Execute parent construct */
        parent::__construct($location);

        /* Override Module variables */
        $this->moduleType = 'Content';
    }

    /* Mark: Visit mode methods */

    /**
     * Initialise content module in visit mode for a given content and block index.
     * @param integer $contentId
     * @param integer $blockIndex
     * @return boolean success
     */
    public function initialise_visit(int $contentId=0, int $blockIndex=0): bool {
        /* Load plugin */
        $this->load();

        /* Reset current content for this block */
        $this->currentContent = null;

        /* Load content library instance if module defines one */
        if (!is_null($this->contentLibrary)) {
            $content = $this->get_contentInstance($contentId);
            if ($content === false)
                return false;

            if ($contentId > 0) {
                if (!$content->load())
                    return false;
            } else {
                $content->initialise();
            }

            $this->currentContent = $content;
            $this->blockContents[$blockIndex] = $content;
        }

        /* Store layout selected for this block */
        $this->blockLayouts[$blockIndex] = $this->selectedLayout;

        return true;
    }

    /**
     * Display content module in visit mode for a given block index.
     * @param integer $blockIndex
     * @return boolean success
     */
    public function display_visit(int $blockIndex=0): bool {
        /* Restore current content for this block if available */
        if (array_key_exists($blockIndex, $this->blockContents))
            $this->currentContent = $this->blockContents[$blockIndex];

        /* Restore layout selected for this block if defined */
        if (isset($this->blockLayouts[$blockIndex]) && $this->blockLayouts[$blockIndex] !== '')
            $this->selectedLayout = $this->blockLayouts[$blockIndex];

        /* Display layout */
        return parent::display();
    }

    /* Mark: Content management methods */

    /**
     * Check if module has editable content via a content library.
     * @return boolean result
     */
    public function has_editableContent(): bool {
        return !is_null($this->contentLibrary);
    }

    /**
     * Return list of available contents (Id => Name) for page block assignment.
     * Modules without a $contentLibrary can override this method to return external contents.
     * @return array
     */
    public function get_availableContentList(): array {
        if (is_null($this->contentLibrary))
            return array();

        $content = $this->get_contentInstance(0);
        if (!($content instanceof ContentModuleLibrary))
            return array();

        return $content->get_nameList();
    }

    /**
     * Return an instance of the module's main content library.
     * @param integer $contentId
     * @return ModuleLibrary|false
     */
    public function get_contentInstance(int $contentId=0): ModuleLibrary|false {
        if (is_null($this->contentLibrary))
            return false;

        $instance = $this->get_libraryInstance($this->contentLibrary, $contentId);
        if (!($instance instanceof ModuleLibrary))
            return false;

        return $instance;
    }

    /* Mark: Workflow methods */

    /**
     * Create a new workflow version for given content object.
     * @param integer $contentId
     * @param string|null $library
     * @return WorkflowModuleLibrary|WorkflowContentModuleLibrary|false
     */
    public function create_contentNewVersion(int $contentId=0, ?string $library=null): WorkflowModuleLibrary|WorkflowContentModuleLibrary|false {
        if (is_null($library))
            $library = $this->contentLibrary;

        return parent::create_contentNewVersion($contentId, $library);
    }

    /**
     * Update workflow status on given content object.
     * @param integer $contentId
     * @param integer $wfStatus
     * @param string|null $library
     * @return boolean success
     */
    public function update_contentWFStatus(int $contentId, int $wfStatus, ?string $library=null): bool {
        if (is_null($library))
            $library = $this->contentLibrary;

        return parent::update_contentWFStatus($contentId, $wfStatus, $library);
    }

    /* Mark: Page parameters helper methods */

    /**
     * Return all current page parameters from CaMykS.
     * @return array
     */
    protected function get_pageParams(): array {
        global $CaMykS;

        return $CaMykS->get_pageParams();
    }

    /**
     * Return a single page parameter value from CaMykS.
     * @param string $name
     * @param mixed $default
     * @return mixed
     */
    protected function get_pageParam(string $name, mixed $default=null): mixed {
        global $CaMykS;

        return $CaMykS->get_pageParam($name, $default);
    }
}
?>
