<?php
/**
 * @brief SystemSitePageManager admin module, Page library.
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

use CaMykS2\Module;
use CaMykS2\WorkflowModuleLibrary;
use CaMykS2\DatabaseQuery;
use CaMykS2\DatabaseQueryWhereCondition;

/**
 * Page class.
 */
final class Page extends WorkflowModuleLibrary {
    /**
     * @var array $blocks
     * @brief Attached Page\Block objects list.
     */
    public array $blocks = array();

    /**
     * @var array $metas
     * @brief Attached Page\Meta objects list.
     */
    public array $metas = array();

    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Define structure & navigation database attributes */
        $this->define_databaseAttribute('ParentId',                 'ObjectId', 0);
        $this->define_databaseAttribute('Position',                 'Integer',  0);
        $this->define_databaseAttribute('Template',                 'Text',     '');
        $this->define_databaseAttribute('Theme',                    'Text',     '');

        /* Define localized database attributes */
        $this->define_databaseAttribute('Name',                     'Name',     '', array(), true);
        $this->define_databaseAttribute('Title',                    'Text',     '', array(), true);
        $this->define_databaseAttribute('MenuTitle',                'Text',     '', array(), true);
        $this->define_databaseAttribute('MetaTitle',                'Text',     '', array(), true);
        $this->define_databaseAttribute('MetaDescription',          'Text',     '', array(), true);
        $this->define_databaseAttribute('MetaKeywords',             'Text',     '', array(), true);

        /* Define meta robots database attributes */
        $this->define_databaseAttribute('MetaRobots_Index',         'Boolean',  true);
        $this->define_databaseAttribute('MetaRobots_Follow',        'Boolean',  true);
        $this->define_databaseAttribute('MetaRobots_Snippet',       'Boolean',  true);
        $this->define_databaseAttribute('MetaRobots_Translate',     'Boolean',  true);
        $this->define_databaseAttribute('MetaRobots_ImageIndex',    'Boolean',  true);

        /* Define sitemap database attributes */
        $this->define_databaseAttribute('Sitemap_Include',          'Boolean',  true);
        $this->define_databaseAttribute('Sitemap_Priority',         'Float',    1.0);
    }

    /* Mark: Data checking & saving methods */

    /**
     * Check object data before saving and update sitemap priority.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        /* Perform parent check_data */
        if (!parent::check_data($loadPreviousVersion))
            return false;

        /* Calculate SitemapPriority from parent page if parent exists *
        if ($this->ParentId > 0) {
            $parentPage = new Page($this->libraryParent, $this->ParentId);
            if ($parentPage->get()) {
                $this->SitemapPriority = max(0.1, round($parentPage->SitemapPriority - 0.1, 2));
            } else {
                $this->SitemapPriority = 1.0;
            }
        } else {
            $this->SitemapPriority = 1.0;
        }
        */

        return true;
    }

    /**
     * Create a new draft version of the page and duplicate its attached blocks and metas.
     * @param boolean $save
     * @return WorkflowModuleLibrary|false
     */
    public function create_newVersion(bool $save=true): WorkflowModuleLibrary|false {
        global $CaMykS;

        /* Create new Page version */
        $newPage = parent::create_newVersion($save);
        if (!($newPage instanceof Page))
            return false;

        /* If not saved yet, sub-objects cannot be attached to a database ParentId */
        if (!$save || $newPage->objectId <= 0)
            return $newPage;

        /* Load current blocks and metas */
        $this->load_blocks();
        $this->load_metas();

        /* Duplicate Page\Meta items */
        $metaLib = $this->libraryParent->get_libraryInstance('Page\\Meta');
        if ($metaLib !== false && !empty($this->metas)) {
            foreach ($this->metas as $metaData) {
                $newMeta = $metaLib->get_newInstance(0);
                $newMeta->initialise();
                if (is_array($metaData))
                    $newMeta->load_fromArray($metaData);
                $newMeta->ParentId = $newPage->objectId;
                $newMeta->save();
            }
        }

        /* Duplicate Page\Block items and trigger content new version if module manages workflow */
        $blockLib = $this->libraryParent->get_libraryInstance('Page\\Block');
        if ($blockLib !== false && !empty($this->blocks)) {
            foreach ($this->blocks as $blockData) {
                $newBlock = $blockLib->get_newInstance(0);
                $newBlock->initialise();
                if (is_array($blockData))
                    $newBlock->load_fromArray($blockData);
                $newBlock->ParentId = $newPage->objectId;

                /* Check if referenced content module manages workflow */
                $moduleName = (string)$newBlock->Module;
                $contentId  = (int)$newBlock->ContentId;
                if ($moduleName !== '' && $contentId > 0 && isset($CaMykS)) {
                    $contentModule = $CaMykS->get_module($moduleName, true);
                    if ($contentModule !== false && $contentModule->has_workflow()) {
                        $newContent = $contentModule->create_contentNewVersion($contentId);
                        if ($newContent !== false) {
                            $newBlock->ContentId = $newContent->objectId;
                            $newBlock->WFVersion = $newContent->WFVersion;
                        }
                    }
                }

                $newBlock->save();
            }
        }

        return $newPage;
    }

    /* Mark: Workflow transition methods */

    /**
     * Mark current draft version as ready (WFStatus = 2) and propagate to attached blocks.
     * @return boolean success
     */
    public function mark_asReady(): bool {
        if (!parent::mark_asReady())
            return false;

        return $this->propagate_wfStatusToBlocks($this->WFStatus);
    }

    /**
     * Validate given workflow step (1 to 3) and propagate to attached blocks.
     * @param integer $step
     * @param integer $userId
     * @param string|null $comment
     * @return boolean success
     */
    public function validate_step(int $step=1, int $userId=0, ?string $comment=null): bool {
        if (!parent::validate_step($step, $userId, $comment))
            return false;

        return $this->propagate_wfStatusToBlocks($this->WFStatus);
    }

    /**
     * Reject given workflow validation step (1 to 3) and propagate to attached blocks.
     * @param integer $step
     * @param integer $userId
     * @param string $comment
     * @return boolean success
     */
    public function reject_step(int $step=1, int $userId=0, string $comment=''): bool {
        if (!parent::reject_step($step, $userId, $comment))
            return false;

        return $this->propagate_wfStatusToBlocks($this->WFStatus);
    }

    /**
     * Cancel current version (11 or 12) and propagate to attached blocks.
     * @param integer $userId
     * @param string $comment
     * @return boolean success
     */
    public function cancel_version(int $userId=0, string $comment=''): bool {
        if (!parent::cancel_version($userId, $comment))
            return false;

        return $this->propagate_wfStatusToBlocks($this->WFStatus);
    }

    /**
     * Archive current version (WFStatus = 21) and propagate to attached blocks.
     * @param integer $userId
     * @param string|null $comment
     * @return boolean success
     */
    public function archive_version(int $userId=0, ?string $comment=null): bool {
        if (!parent::archive_version($userId, $comment))
            return false;

        return $this->propagate_wfStatusToBlocks($this->WFStatus);
    }

    /**
     * Publish current version as Live (WFStatus = 6) and propagate to attached blocks.
     * @param integer $userId
     * @return boolean success
     */
    public function publish_version(int $userId=0): bool {
        if (!parent::publish_version($userId))
            return false;

        return $this->propagate_wfStatusToBlocks($this->WFStatus);
    }

    /**
     * Propagate workflow status to all attached Page\Block content objects whose module manages workflow.
     * @param integer $wfStatus
     * @return boolean success
     */
    protected function propagate_wfStatusToBlocks(int $wfStatus): bool {
        global $CaMykS;

        if ($this->objectId <= 0 || !isset($CaMykS))
            return true;

        if (empty($this->blocks))
            $this->load_blocks();

        if (empty($this->blocks))
            return true;

        foreach ($this->blocks as $blockData) {
            $moduleName = '';
            $contentId  = 0;

            if (is_array($blockData)) {
                $moduleName = isset($blockData['Module']) ? (string)$blockData['Module'] : '';
                $contentId  = isset($blockData['ContentId']) ? (int)$blockData['ContentId'] : 0;
            } elseif (is_object($blockData)) {
                $moduleName = (string)$blockData->Module;
                $contentId  = (int)$blockData->ContentId;
            }

            if ($moduleName !== '' && $contentId > 0) {
                $contentModule = $CaMykS->get_module($moduleName, true);
                if ($contentModule !== false && $contentModule->has_workflow())
                    $contentModule->update_contentWFStatus($contentId, $wfStatus);
            }
        }

        return true;
    }

    /* Mark: Sub-libraries loading methods */

    /**
     * Load attached Page\Block objects into $this->blocks.
     * @return void
     */
    public function load_blocks(): void {
        if ($this->objectId <= 0)
            return;
        $this->blocks = $this->libraryParent->get_libraryInstance('Page\\Block')->get_list(array('ParentId'=>$this->objectId, 'OrderBy'=>array('Position'=>'asc')));
    }

    /**
     * Load attached Page\Meta objects into $this->metas.
     * @return void
     */
    public function load_metas(): void {
        if ($this->objectId <= 0)
            return;
        $this->metas = $this->libraryParent->get_libraryInstance('Page\\Meta')->get_list(array('ParentId'=>$this->objectId));
    }

    /* Mark: Helper methods */

    /**
     * Return formatted Meta Robots string.
     * @return string
     */
    public function get_metaRobotsValue(): string {
        $directives = array();

        $directives[] = $this->MetaRobots_Index ? 'index' : 'noindex';
        $directives[] = $this->MetaRobots_Follow ? 'follow' : 'nofollow';

        if (!$this->MetaRobots_Snippet)
            $directives[] = 'nosnippet';

        if (!$this->MetaRobots_Translate)
            $directives[] = 'notranslate';

        if (!$this->MetaRobots_ImageIndex)
            $directives[] = 'noimageindex';

        return implode(', ', $directives);
    }
}
?>
