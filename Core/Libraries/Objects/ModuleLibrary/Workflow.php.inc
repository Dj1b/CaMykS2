<?php
/**
 * @brief WorkflowModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Nov 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * WorkflowModuleLibrary class.
 */
class WorkflowModuleLibrary extends ManagedIdentifiableModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Workflow module library attributes */
        $this->define_databaseAttribute('WFStatus',          'Integer',  1,  array('Min' => 0, 'Max' => 127, 'Unsigned' => true));
        $this->define_databaseAttribute('WFVersion',         'Integer',  1,  array('Min' => 1, 'Unsigned' => true));
        $this->define_databaseAttribute('WFComment',         'Text',     '', array('Length' => 1024, 'AutoTrim' => true, 'StripHTMLTags' => true));
        $this->define_databaseAttribute('WFValidation1User', 'ObjectId', 0);
        $this->define_databaseAttribute('WFValidation1Date', 'Date',     0);
        $this->define_databaseAttribute('WFValidation2User', 'ObjectId', 0);
        $this->define_databaseAttribute('WFValidation2Date', 'Date',     0);
        $this->define_databaseAttribute('WFValidation3User', 'ObjectId', 0);
        $this->define_databaseAttribute('WFValidation3Date', 'Date',     0);
        $this->define_databaseAttribute('WFLiveUser',        'ObjectId', 0);
        $this->define_databaseAttribute('WFLiveDate',        'Date',     0);
        $this->define_databaseAttribute('WFRejectUser',      'ObjectId', 0);
        $this->define_databaseAttribute('WFRejectDate',      'Date',     0);
        $this->define_databaseAttribute('WFArchiveUser',     'ObjectId', 0);
        $this->define_databaseAttribute('WFArchiveDate',     'Date',     0);
    }

    /* Mark: Override ModuleLibrary methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        global $CaMykS;

        /* Perform parent check data loading previous version */
        if (!parent::check_data(true))
            return false;

        /* Check workflow version attribute */
        if ($this->objectId > 0) {
            /* Object is updated: preserve its version number */
            if (isset($this->previousVersion->WFVersion) && $this->previousVersion->WFVersion > 0)
                $this->WFVersion = $this->previousVersion->WFVersion;
        } else {
            /* Object is created: compute next version if UniqueId already exists */
            if ($this->WFVersion <= 0) {
                $this->WFVersion = $this->get_newVersion();
            }
        }

        /* Automatically set WFStatus to 6 (Live) if SystemWorkflowManager is unavailable or disabled */
        $workflowManager = isset($CaMykS) ? $CaMykS->get_module('SystemWorkflowManager', true) : false;
        if ($workflowManager === false or !$workflowManager->is_workflowEnabled()) {
            $this->WFStatus = 6;
        }

        /* Return data is conform */
        return true;
    }

    /* Mark: Workflow status related methods */

    /**
     * Return workflow status values.
     * @return array
     */
    public function get_wfStatusValueList(): array {
        return array(
            1  => $this->get_locale('ObjectWFStatus_Draft'),
            2  => $this->get_locale('ObjectWFStatus_Ready'),
            3  => $this->get_locale('ObjectWFStatus_Validated1'),
            4  => $this->get_locale('ObjectWFStatus_Validated2'),
            5  => $this->get_locale('ObjectWFStatus_Validated3'),
            6  => $this->get_locale('ObjectWFStatus_Live'),
            11 => $this->get_locale('ObjectWFStatus_CancelledDraft'),
            12 => $this->get_locale('ObjectWFStatus_CancelledReady'),
            13 => $this->get_locale('ObjectWFStatus_Rejected1'),
            14 => $this->get_locale('ObjectWFStatus_Rejected2'),
            15 => $this->get_locale('ObjectWFStatus_Rejected3'),
            21 => $this->get_locale('ObjectWFStatus_Archived'),
        );
    }

    /**
     * Return multi object workflow status values.
     * @param array $params
     * @return array
     */
    public function get_multiWFStatusValueList(array $params=array()): array {
        /* Check params */
        $params = array_merge(array('AddAll' => false), $params);

        /* Initialise list */
        $list = array();

        /* Check for all value */
        if ($params['AddAll'])
            $list[0] = $this->get_locale('ObjectsStatus_All');

        /* Insert values */
        $list[1]  = $this->get_locale('ObjectsWFStatus_Draft');
        $list[2]  = $this->get_locale('ObjectsWFStatus_Ready');
        $list[3]  = $this->get_locale('ObjectsWFStatus_Validated1');
        $list[4]  = $this->get_locale('ObjectsWFStatus_Validated2');
        $list[5]  = $this->get_locale('ObjectsWFStatus_Validated3');
        $list[6]  = $this->get_locale('ObjectsWFStatus_Live');
        $list[11] = $this->get_locale('ObjectsWFStatus_CancelledDraft');
        $list[12] = $this->get_locale('ObjectsWFStatus_CancelledReady');
        $list[13] = $this->get_locale('ObjectsWFStatus_Rejected1');
        $list[14] = $this->get_locale('ObjectsWFStatus_Rejected2');
        $list[15] = $this->get_locale('ObjectsWFStatus_Rejected3');
        $list[21] = $this->get_locale('ObjectsWFStatus_Archived');

        /* Return list */
        return $list;
    }

    /**
     * Return workflow status title.
     * @param integer|null $status
     * @return string
     */
    public function get_wfStatusTitle(?int $status=null): string {
        if (is_null($status))
            $status = $this->WFStatus;

        $list = $this->get_wfStatusValueList();

        if (isset($list[$status]))
            return $list[$status];
        return '<span class="undefined">'.$this->get_locale('ObjectStatus_Undefined').'</span>';
    }

    /* Mark: Version management methods */

    /**
     * Return maximum existing version for given UniqueId.
     * @param string|null $uniqueId
     * @return integer
     */
    public function get_maxVersion(?string $uniqueId=null): int {
        if (is_null($uniqueId))
            $uniqueId = (string)$this->UniqueId;

        if ($uniqueId === '' || $this->databaseConnector === false || empty($this->dbTableName))
            return 0;

        /* Build query to select max WFVersion */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('WFVersion', 'maxVersion', '', 'max');
        $query->add_whereCondition(new DatabaseQueryWhereCondition('UniqueId', $uniqueId));

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return 0;

        /* Fetch data line */
        $data = $this->get_DBQueryDataLine();
        if ($data === false || is_null($data) || !isset($data['maxVersion']))
            return 0;

        return (int)$data['maxVersion'];
    }

    /**
     * Return next available version number for given UniqueId.
     * @param string|null $uniqueId
     * @return integer
     */
    public function get_newVersion(?string $uniqueId=null): int {
        return $this->get_maxVersion($uniqueId) + 1;
    }

    /**
     * Return list of all versions for given UniqueId.
     * @param string|null $uniqueId
     * @param mixed $language
     * @param array $orderBy
     * @return array
     */
    public function get_versionList(?string $uniqueId=null, mixed $language=true, array $orderBy=array('WFVersion' => 'desc')): array {
        if (is_null($uniqueId))
            $uniqueId = (string)$this->UniqueId;

        if ($uniqueId === '')
            return array();

        $conditions = array(
            new DatabaseQueryWhereCondition('UniqueId', $uniqueId),
        );

        return $this->get_objectList($language, $conditions, -1, -1, $orderBy);
    }

    /**
     * Load live version (WFStatus = 6) for given UniqueId.
     * @param string $uniqueId
     * @param mixed $language
     * @return boolean success
     */
    public function load_liveVersion(string $uniqueId='', mixed $language=true): bool {
        if ($uniqueId === '')
            $uniqueId = (string)$this->UniqueId;
        if ($uniqueId === '')
            return false;

        $conditions = array(
            new DatabaseQueryWhereCondition('UniqueId', $uniqueId),
            new DatabaseQueryWhereCondition('WFStatus', 6),
        );

        $idList = $this->get_objectIdList($language, $conditions, 1, -1, array('WFVersion' => 'desc'));
        if (empty($idList))
            return false;

        $this->objectId = (int)reset($idList);
        return $this->load($language);
    }

    /**
     * Create a new draft version cloned from current object.
     * @param boolean $save
     * @return WorkflowModuleLibrary|false
     */
    public function create_newVersion(bool $save=true): WorkflowModuleLibrary|false {
        /* Ensure current object is loaded if it has an ID */
        if ($this->objectId > 0 && empty($this->UniqueId)) {
            if (!$this->load(true))
                return false;
        }

        /* Create new instance */
        $newInstance = $this->get_newInstance(0);
        if (!($newInstance instanceof WorkflowModuleLibrary))
            return false;

        $newInstance->initialise();

        /* Copy non-localised attributes */
        foreach ($this->attributesList['db'] as $attrName) {
            if (isset($newInstance->libraryAttributes[$attrName]))
                $newInstance->libraryAttributes[$attrName]->set_value($this->libraryAttributes[$attrName]->get_value());
        }

        /* Copy localised attributes */
        foreach ($this->attributesList['dbl'] as $attrName) {
            if (isset($newInstance->libraryAttributes[$attrName])) {
                $values = $this->libraryAttributes[$attrName]->get_values();
                if (is_array($values)) {
                    foreach ($values as $lang => $val)
                        $newInstance->libraryAttributes[$attrName]->set_value($val, $lang);
                }
            }
        }

        /* Reset workflow metadata for the new draft version */
        $newInstance->UniqueId          = $this->UniqueId;
        $newInstance->Status            = 1;
        $newInstance->WFStatus          = 1;
        $newInstance->WFVersion         = $this->get_newVersion((string)$this->UniqueId);
        $newInstance->WFComment         = '';
        $newInstance->WFValidation1User = 0;
        $newInstance->WFValidation1Date = 0;
        $newInstance->WFValidation2User = 0;
        $newInstance->WFValidation2Date = 0;
        $newInstance->WFValidation3User = 0;
        $newInstance->WFValidation3Date = 0;
        $newInstance->WFLiveUser        = 0;
        $newInstance->WFLiveDate        = 0;
        $newInstance->WFRejectUser      = 0;
        $newInstance->WFRejectDate      = 0;
        $newInstance->WFArchiveUser     = 0;
        $newInstance->WFArchiveDate     = 0;

        /* Save new version if requested */
        if ($save && !$newInstance->save())
            return false;

        return $newInstance;
    }

    /* Mark: Workflow transition methods */

    /**
     * Mark current draft version as ready (WFStatus = 2).
     * @return boolean success
     */
    public function mark_asReady(): bool {
        $this->WFStatus = 2;
        return $this->save();
    }

    /**
     * Validate given workflow step (1 to 3).
     * @param integer $step
     * @param integer $userId
     * @param string|null $comment
     * @return boolean success
     */
    public function validate_step(int $step=1, int $userId=0, ?string $comment=null): bool {
        if ($step < 1 || $step > 3)
            return false;

        $userAttr = 'WFValidation'.$step.'User';
        $dateAttr = 'WFValidation'.$step.'Date';

        $this->$userAttr = $userId;
        $this->$dateAttr = time();
        $this->WFStatus  = 2 + $step;

        if (!is_null($comment))
            $this->WFComment = $comment;

        return $this->save();
    }

    /**
     * Reject given workflow validation step (1 to 3).
     * @param integer $step
     * @param integer $userId
     * @param string $comment
     * @return boolean success
     */
    public function reject_step(int $step=1, int $userId=0, string $comment=''): bool {
        if ($step < 1 || $step > 3)
            return false;

        $this->WFRejectUser = $userId;
        $this->WFRejectDate = time();
        $this->WFStatus     = 12 + $step;
        $this->Status       = 1;
        $this->WFComment    = $comment;

        return $this->save();
    }

    /**
     * Cancel current version (after draft: 11, or after ready: 12).
     * @param integer $userId
     * @param string $comment
     * @return boolean success
     */
    public function cancel_version(int $userId=0, string $comment=''): bool {
        if ($this->WFStatus === 2)
            $this->WFStatus = 12;
        else
            $this->WFStatus = 11;

        $this->Status       = 1;
        $this->WFRejectUser = $userId;
        $this->WFRejectDate = time();
        $this->WFComment    = $comment;

        return $this->save();
    }

    /**
     * Archive current version (WFStatus = 21).
     * @param integer $userId
     * @param string|null $comment
     * @return boolean success
     */
    public function archive_version(int $userId=0, ?string $comment=null): bool {
        $this->Status        = 1;
        $this->WFStatus      = 21;
        $this->WFArchiveUser = $userId;
        $this->WFArchiveDate = time();

        if (!is_null($comment))
            $this->WFComment = $comment;

        return $this->save();
    }

    /**
     * Publish current version as Live (WFStatus = 6) and archive obsolete versions.
     * Lower active versions (< WFVersion and WFStatus < 11) and higher live versions (> WFVersion and WFStatus == 6) are archived.
     * @param integer $userId
     * @return boolean success
     */
    public function publish_version(int $userId=0): bool {
        /* Save current object as Live */
        $this->Status     = 2;
        $this->WFStatus   = 6;
        $this->WFLiveUser = $userId;
        $this->WFLiveDate = time();

        if (!$this->save())
            return false;

        /* Check UniqueId to archive obsolete versions */
        $uniqueId = (string)$this->UniqueId;
        if ($uniqueId === '')
            return true;

        $archiveTime = time();

        /* 1. Find lower versions (< current WFVersion) that are still active (WFStatus < 11) */
        $lowerConditions = array(
            new DatabaseQueryWhereCondition('UniqueId', $uniqueId),
            new DatabaseQueryWhereCondition('id', $this->objectId, '!='),
            new DatabaseQueryWhereCondition('WFVersion', $this->WFVersion, '<'),
            new DatabaseQueryWhereCondition('WFStatus', 11, '<'),
        );
        $lowerIds = $this->get_objectIdList(false, $lowerConditions);

        /* 2. Find higher versions (> current WFVersion) that are currently Live (WFStatus == 6) */
        $higherLiveConditions = array(
            new DatabaseQueryWhereCondition('UniqueId', $uniqueId),
            new DatabaseQueryWhereCondition('id', $this->objectId, '!='),
            new DatabaseQueryWhereCondition('WFVersion', $this->WFVersion, '>'),
            new DatabaseQueryWhereCondition('WFStatus', 6),
        );
        $higherLiveIds = $this->get_objectIdList(false, $higherLiveConditions);

        /* Archive all matching obsolete versions */
        $obsoleteIds = array_unique(array_merge($lowerIds, $higherLiveIds));
        foreach ($obsoleteIds as $obsoleteId) {
            $this->update_databaseAttributeList(array(
                'Status'        => 1,
                'WFStatus'      => 21,
                'WFArchiveUser' => $userId,
                'WFArchiveDate' => $archiveTime,
            ), (int)$obsoleteId);
        }

        return true;
    }
}
?>
