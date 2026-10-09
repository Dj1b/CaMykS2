<?php
/**
 * @brief ManagedSortableModuleLibrary object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Oct 2026
 * @date Modification: Oct 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ManagedSortableModuleLibrary class.
 */
class ManagedSortableModuleLibrary extends ManagedModuleLibrary {

    /**
     * @var constant array SORTINGPARAMS
     * @brief List of attributes for sorting scope.
     */
    protected const SORTINGPARAMS = array();

    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Managed sortable module library attributes */
        $this->define_databaseAttribute('SortPosition', 'Integer', 0, array('Min' => 0, 'Unsigned' => true));
    }

    /* Mark: Override ModuleLibrary methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        /* Perform parent check data loading previous version */
        if (!parent::check_data(true))
            return false;

        /* Check position attribute */
        if ($this->objectId > 0) {
            /* Object is updated */
            if ($this->SortPosition === 0 && isset($this->previousVersion->SortPosition))
                $this->SortPosition = $this->previousVersion->SortPosition;
        } else {
            /* Object is created */
            if ($this->SortPosition === 0)
                $this->SortPosition = $this->get_newPosition();
        }

        /* Return data is conform */
        return true;
    }

    /* Mark: Position management methods */

    /**
     * Return next available position.
     * @return integer
     */
    public function get_newPosition(): int {
        if ($this->databaseConnector === false || !isset($this->dbTableName))
            return 1;

        /* Build query to select max position */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('SortPosition', 'maxPosition', '', 'max');

        /* Build where conditions */
        $conditions = $this->build_sortingWhereConditions();
        foreach ($conditions as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return 1;

        /* Fetch data line */
        $data = $this->get_DBQueryDataLine();
        if ($data === false || is_null($data) || !isset($data['maxPosition']))
            return 1;

        return (int)$data['maxPosition'] + 1;
    }

    /**
     * Return minimum position for given scope.
     * @return integer
     */
    public function get_minPosition(): int {
        if ($this->databaseConnector === false || !isset($this->dbTableName))
            return 1;

        /* Build query to select min position */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('SortPosition', 'minPosition', '', 'min');

        /* Build where conditions */
        $conditions = $this->build_sortingWhereConditions();
        foreach ($conditions as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return 1;

        /* Fetch data line */
        $data = $this->get_DBQueryDataLine();
        if ($data === false || is_null($data) || !isset($data['minPosition']))
            return 1;

        return (int)$data['minPosition'];
    }

    /**
     * Return maximum position for given scope.
     * @return integer
     */
    public function get_maxPosition(): int {
        if ($this->databaseConnector === false || !isset($this->dbTableName))
            return 0;

        /* Build query to select max position */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('SortPosition', 'maxPosition', '', 'max');

        /* Build where conditions */
        $conditions = $this->build_sortingWhereConditions();
        foreach ($conditions as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return 0;

        /* Fetch data line */
        $data = $this->get_DBQueryDataLine();
        if ($data === false || is_null($data) || !isset($data['maxPosition']))
            return 0;

        return (int)$data['maxPosition'];
    }

    /**
     * Check if object is not in first position.
     * @return boolean
     */
    public function is_notInFirstPosition(): bool {
        if ($this->objectId === 0 || $this->SortPosition <= 1)
            return false;

        $minPosition = $this->get_minPosition();
        return $this->SortPosition > $minPosition;
    }

    /**
     * Check if object is not in last position.
     * @return boolean
     */
    public function is_notInLastPosition(): bool {
        if ($this->objectId === 0 || $this->SortPosition === 0)
            return false;

        $maxPosition = $this->get_maxPosition();
        return $this->SortPosition < $maxPosition;
    }

    /**
     * Move position down by 1.
     * @return boolean success
     */
    public function move_positionDown(): bool {
        return $this->move_position(1);
    }

    /**
     * Move position up by 1.
     * @return boolean success
     */
    public function move_positionUp(): bool {
        return $this->move_position(-1);
    }

    /**
     * Move position by relative offset.
     * @param integer $offset
     * @return boolean success
     */
    public function move_position(int $offset): bool {
        if ($offset === 0)
            return true;
        return $this->move_toPosition($this->SortPosition + $offset);
    }

    /**
     * Move to target position.
     * @param integer $targetPosition
     * @return boolean success
     */
    public function move_toPosition(int $targetPosition): bool {
        if ($this->objectId === 0 || $this->SortPosition === 0)
            return false;

        $oldPosition = $this->SortPosition;

        /* Get position boundaries */
        $minPosition = $this->get_minPosition();
        $maxPosition = $this->get_maxPosition();

        if ($maxPosition === 0)
            return false;

        /* Clamp target position */
        if ($targetPosition < $minPosition)
            $targetPosition = $minPosition;
        if ($targetPosition > $maxPosition)
            $targetPosition = $maxPosition;

        if ($targetPosition === $oldPosition)
            return true;

        /* Prepare conditions */
        $baseConditions = $this->build_sortingWhereConditions();

        if ($targetPosition < $oldPosition) {
            /* Moving UP (earlier in list): shift items in [targetPosition, oldPosition - 1] DOWN (+1) */
            $queryConditions = array_merge($baseConditions, array(
                new DatabaseQueryWhereCondition('SortPosition', $targetPosition, '>='),
                new DatabaseQueryWhereCondition('SortPosition', $oldPosition - 1, '<='),
            ));
            $list = $this->get_objectList(true, $queryConditions, -1, -1, array('SortPosition' => 'desc'));

            foreach ($list as $item) {
                if (isset($item['id']) && isset($item['SortPosition'])) {
                    $this->update_databaseAttribute('SortPosition', (int)$item['SortPosition'] + 1, (int)$item['id']);
                }
            }
        } else {
            /* Moving DOWN (later in list): shift items in [oldPosition + 1, targetPosition] UP (-1) */
            $queryConditions = array_merge($baseConditions, array(
                new DatabaseQueryWhereCondition('SortPosition', $oldPosition + 1, '>='),
                new DatabaseQueryWhereCondition('SortPosition', $targetPosition, '<='),
            ));
            $list = $this->get_objectList(true, $queryConditions, -1, -1, array('SortPosition' => 'asc'));

            foreach ($list as $item) {
                if (isset($item['id']) && isset($item['SortPosition'])) {
                    $this->update_databaseAttribute('SortPosition', (int)$item['SortPosition'] - 1, (int)$item['id']);
                }
            }
        }

        /* Update target object */
        if ($this->update_databaseAttribute('SortPosition', $targetPosition, $this->objectId)) {
            $this->SortPosition = $targetPosition;
            return true;
        }

        return false;
    }

    /**
     * Build sorting where conditions list from static::SORTINGPARAMS constant.
     * @return array
     */
    protected function build_sortingWhereConditions(): array {
        $conditions = array();
        foreach (static::SORTINGPARAMS as $key => $val) {
            if (is_int($key) && is_string($val)) {
                /* Attribute name specified, retrieve value from object instance */
                $value = isset($this->libraryAttributes[$val]) ? $this->libraryAttributes[$val]->get_value() : (isset($this->$val) ? $this->$val : null);
                if (!is_null($value))
                    $conditions[] = new DatabaseQueryWhereCondition($val, $value);
            } elseif (is_string($key)) {
                /* Key-value condition pair */
                $conditions[] = new DatabaseQueryWhereCondition($key, $val);
            } elseif ($val instanceof DatabaseQueryWhereCondition) {
                $conditions[] = $val;
            }
        }
        return $conditions;
    }
}
?>
