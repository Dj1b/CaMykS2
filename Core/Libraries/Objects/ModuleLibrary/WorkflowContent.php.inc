<?php
/**
 * @brief WorkflowContentModuleLibrary object.
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
 * WorkflowContentModuleLibrary class.
 */
class WorkflowContentModuleLibrary extends ContentModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Workflow content module library attributes */
        $this->define_databaseAttribute('WFUniqueId', 'UniqueId', '', array());
        $this->define_databaseAttribute('WFStatus',   'Integer',  1,  array('Min' => 0, 'Max' => 127, 'Unsigned' => true));
        $this->define_databaseAttribute('WFVersion',  'Integer',  1,  array('Min' => 1, 'Unsigned' => true));
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

        /* Check workflow unique id and version attributes */
        if ($this->objectId > 0) {
            /* Object is updated: preserve its WFUniqueId and WFVersion */
            if (isset($this->previousVersion->WFUniqueId) && $this->previousVersion->WFUniqueId !== '')
                $this->WFUniqueId = $this->previousVersion->WFUniqueId;
            if (isset($this->previousVersion->WFVersion) && $this->previousVersion->WFVersion > 0)
                $this->WFVersion = $this->previousVersion->WFVersion;
        } else {
            /* Object is created */
            if ($this->WFUniqueId === '')
                $this->WFUniqueId = (string)$this->get_newWFUniqueId();
            if ($this->WFVersion <= 0)
                $this->WFVersion = $this->get_newVersion();
        }

        /* Automatically set WFStatus to 6 (Live) if SystemWorkflowManager is unavailable or disabled */
        $workflowManager = isset($CaMykS) ? $CaMykS->get_module('SystemWorkflowManager', true) : false;
        if ($workflowManager === false or !$workflowManager->is_workflowEnabled()) {
            $this->WFStatus = 6;
        }

        /* Return data is conform */
        return true;
    }

    /* Mark: Content list methods */

    /**
     * Return associative list of live content names indexed by object ID.
     * @param array $whereParams
     * @param array $orderBy
     * @return array
     */
    public function get_nameList(array $whereParams=array(), array $orderBy=array('Name' => 'asc')): array {
        if (empty($whereParams)) {
            $whereParams = array(
                new DatabaseQueryWhereCondition('WFStatus', 6),
            );
        }
        return parent::get_nameList($whereParams, $orderBy);
    }

    /* Mark: Unique id related methods */

    /**
     * Return a new workflow unique id from SystemUniqueIdManager.
     * @param array $params
     * @return string|false
     */
    public function get_newWFUniqueId(array $params=array()): string|false {
        global $CaMykS;

        /* Load SystemUniqueIdManager module */
        $uniqueIdManager = $CaMykS->get_module('SystemUniqueIdManager', true);
        if ($uniqueIdManager === false)
            return false;

        return $uniqueIdManager->get_newUniqueId($this->libraryParent->libraryName, $this->libraryShortName, $this->objectId, $params);
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
     * Return maximum existing version for given WFUniqueId.
     * @param string|null $wfUniqueId
     * @return integer
     */
    public function get_maxVersion(?string $wfUniqueId=null): int {
        if (is_null($wfUniqueId))
            $wfUniqueId = (string)$this->WFUniqueId;

        if ($wfUniqueId === '' || $this->databaseConnector === false || empty($this->dbTableName))
            return 0;

        /* Build query to select max WFVersion */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('WFVersion', 'maxVersion', '', 'max');
        $query->add_whereCondition(new DatabaseQueryWhereCondition('WFUniqueId', $wfUniqueId));

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
     * Return next available version number for given WFUniqueId.
     * @param string|null $wfUniqueId
     * @return integer
     */
    public function get_newVersion(?string $wfUniqueId=null): int {
        return $this->get_maxVersion($wfUniqueId) + 1;
    }

    /**
     * Return list of all versions for given WFUniqueId.
     * @param string|null $wfUniqueId
     * @param mixed $language
     * @param array $orderBy
     * @return array
     */
    public function get_versionList(?string $wfUniqueId=null, mixed $language=true, array $orderBy=array('WFVersion' => 'desc')): array {
        if (is_null($wfUniqueId))
            $wfUniqueId = (string)$this->WFUniqueId;

        if ($wfUniqueId === '')
            return array();

        $conditions = array(
            new DatabaseQueryWhereCondition('WFUniqueId', $wfUniqueId),
        );

        return $this->get_objectList($language, $conditions, -1, -1, $orderBy);
    }

    /**
     * Create a new draft version cloned from current content object.
     * @param boolean $save
     * @return WorkflowContentModuleLibrary|false
     */
    public function create_newVersion(bool $save=true): WorkflowContentModuleLibrary|false {
        /* Ensure current object is loaded if it has an ID */
        if ($this->objectId > 0 && empty($this->WFUniqueId)) {
            if (!$this->load(true))
                return false;
        }

        /* Create new instance */
        $newInstance = $this->get_newInstance(0);
        if (!($newInstance instanceof WorkflowContentModuleLibrary))
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
        $newInstance->WFUniqueId = $this->WFUniqueId;
        $newInstance->Status     = 1;
        $newInstance->WFStatus   = 1;
        $newInstance->WFVersion  = $this->get_newVersion((string)$this->WFUniqueId);

        /* Save new version if requested */
        if ($save && !$newInstance->save())
            return false;

        return $newInstance;
    }

    /**
     * Update workflow status of current content version (and archive obsolete versions when going Live).
     * @param integer $wfStatus
     * @return boolean success
     */
    public function update_wfStatus(int $wfStatus): bool {
        $this->WFStatus = $wfStatus;
        $this->Status   = ($wfStatus === 6) ? 2 : 1;

        if (!$this->save())
            return false;

        /* When published as Live (6), archive lower active versions and higher Live versions */
        if ($wfStatus === 6) {
            $wfUniqueId = (string)$this->WFUniqueId;
            if ($wfUniqueId !== '') {
                /* 1. Lower active versions (< WFVersion and WFStatus < 11) */
                $lowerConditions = array(
                    new DatabaseQueryWhereCondition('WFUniqueId', $wfUniqueId),
                    new DatabaseQueryWhereCondition('id', $this->objectId, '!='),
                    new DatabaseQueryWhereCondition('WFVersion', $this->WFVersion, '<'),
                    new DatabaseQueryWhereCondition('WFStatus', 11, '<'),
                );
                $lowerIds = $this->get_objectIdList(false, $lowerConditions);

                /* 2. Higher Live versions (> WFVersion and WFStatus == 6) */
                $higherLiveConditions = array(
                    new DatabaseQueryWhereCondition('WFUniqueId', $wfUniqueId),
                    new DatabaseQueryWhereCondition('id', $this->objectId, '!='),
                    new DatabaseQueryWhereCondition('WFVersion', $this->WFVersion, '>'),
                    new DatabaseQueryWhereCondition('WFStatus', 6),
                );
                $higherLiveIds = $this->get_objectIdList(false, $higherLiveConditions);

                $obsoleteIds = array_unique(array_merge($lowerIds, $higherLiveIds));
                foreach ($obsoleteIds as $obsoleteId) {
                    $this->update_databaseAttributeList(array(
                        'Status'   => 1,
                        'WFStatus' => 21,
                    ), (int)$obsoleteId);
                }
            }
        }

        return true;
    }
}
?>
