<?php
/**
 * @brief Interface for CaMykS SQL database drivers.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre7
 * @date Creation: Nov 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @todo Management of where conditions, enhancement of select items management, see DbQuery library.
 */

namespace CaMykS2;

/**
 * SQLDatabaseDriverInterface class.
 */
abstract class SQLDatabaseDriverInterface extends DatabaseDriverInterface {

    /**
     * Class constructor.
     * @param Object $parent
     * @return void
     */
    public function __construct(&$parent) {
        parent::__construct($parent);
    }

    /* Mark: Override DatabaseDriverInterface methods */

    /**
     * Execute prepared query using DbQuery builder.
     * @param Object $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_builtQuery(DbQuery $query, string $resourceName='Default'): bool {
        switch ($query->get_action()) {
            case 'select': return $this->execute_directQuery($this->get_builtSelectQuery($query), $resourceName);
            case 'insert': return $this->execute_directQuery($this->get_builtInsertQuery($query), $resourceName);
            case 'update': return $this->execute_directQuery($this->get_builtUpdateQuery($query), $resourceName);
            case 'delete': return $this->execute_directQuery($this->get_builtDeleteQuery($query), $resourceName);
            case 'replace': return $this->execute_directQuery($this->get_builtReplaceQuery($query), $resourceName);
        }

        /* Return default result */
        return false;
    }

    /* Mark: Execute built query methods */

    /**
     * Return select query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtSelectQuery(DbQuery $query): string {
        return  'select '.
            $this->get_selectItems($query->get_selectItems()).
            ' from '.
            $this->get_tables($query->get_tables()).
            $this->get_whereConditions($query->get_whereConditions()).
            $this->get_selectGroupBy($query->get_selectGroupBy()).
            $this->get_orderBy($query->get_orderBy()).
            $this->get_limits($query->get_limits()).
            ';';
    }

    /**
     * Return insert query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtInsertQuery(DbQuery$query): string {
        return  'insert into '.
            $this->get_tables($query->get_tables()).
            $this->get_insertPartition($query->get_insertPartition()).
            ' values '.
            $this->get_insertValues($query->get_insertValues()).
            ';';
    }

    /**
     * Return update query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtUpdateQuery(DbQuery $query): string {
        return  'update '.
            $this->get_tables($query->get_tables()).
            ' set '.
            $this->get_updateAssignments($query->get_updateAssignments()).
            $this->get_whereConditions($query->get_whereConditions()).
            ';';
    }

    /**
     * Return delete query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtDeleteQuery(DbQuery $query): string {
        return  'delete from '.
            $this->get_tables($query->get_tables()).
            $this->get_whereConditions($query->get_whereConditions()).
            $this->get_orderBy($query->get_orderBy()).
            $this->get_limits($query->get_limits()).
            ';';
    }

    /**
     * Return replace query using DbQuery builder (default MySQL syntax).
     * @param DbQuery $query
     * @return string
     */
    protected function get_builtReplaceQuery(DbQuery $query): string {
        $sql = 'insert into '.
            $this->get_tables($query->get_tables()).
            $this->get_insertPartition($query->get_insertPartition()).
            ' values '.
            $this->get_insertValues($query->get_insertValues());

        $assignments = $query->get_updateAssignments();
        if (count($assignments) > 0) {
            $sql .= ' on duplicate key update '.$this->get_updateAssignments($assignments);
        } else {
            $sql .= ' on duplicate key update '.$this->get_replaceAssignments($query);
        }
        return $sql.';';
    }

    /* Mark: Execute built query generic sub methods */

    /**
     * Return query tables from data.
     * @param array $tables
     * @return string
     */
    protected function get_tables(array $tables): string {
        /* Initialise result */
        $result = '';

        /* Set is first */
        $isFirst = true;

        /* Read tables */
        foreach($tables as $table) {
            $s = '';

            if ($isFirst) {
                $isFirst = false;
            } elseif ($table['join'] == '') {
                $s .= ',';
            } else {
                $s .= ' '.$table['join'].' join';
            }
            $s .= ' '.$table['table'];
            if ($table['alias'] != '')
                $s .= ' as '.$table['alias'];

            $result .= $s;
        }

        /* Return result */
        return $result;
    }

    /**
     * Return query where conditions from data.
     * @param array $whereConditions
     * @return string
     */
    protected function get_whereConditions($whereConditions): string {
        /* Check where conditions */
        if (!is_array($whereConditions) or !isset($whereConditions['Operator']) or !isset($whereConditions['Conditions']))
            return 'false';
        if (count($whereConditions['Conditions']) == 0)
            return '';

        /* Initialise conditions list */
        $conditions = array();

        /* Read conditions list */
        foreach ($whereConditions['Conditions'] as $condition) {
            if (is_object($condition) and get_class($condition) == 'CaMykS2\DbQueryWhereCondition')
                $conditions[] = $this->get_whereCondition($condition);
            elseif (is_array($condition))
                $conditions[] = '('.$this->get_whereConditions($condition).')';
            else
                $conditions[] = 'false';
        }

        /* Return result */
        return ' where '.implode(' '.$whereConditions['Operator'].' ', $conditions);
    }

    /**
     * Return query single where condition from DbQueryWhereCondition object.
     * @param DbQueryWhereCondition $condition
     * @return string
     */
    protected function get_whereCondition($condition): string {


        /* Initiate result */
        $result = '';

        /* Add identifier */
        $result .= ($condition->identifierTable !== '' ? $condition->identifierTable.'.' : '').'`'.$condition->identifier.'` ';

        /* Check negaative */
        if ($condition->isNot) $result .= 'not ';

        /* Add operator and value */
        switch ($condition->operator) {
            /* String operator */
            case 'start': $result .= 'like "%'.$this->get_escapedValue($condition->value).'"'; break;
            case 'end': $result .= 'like "'.$this->get_escapedValue($condition->value).'%"'; break;
            case 'contain': $result .= 'like "%'.$this->get_escapedValue($condition->value).'%"'; break;

            /* List operator */
            case 'in':
                if (is_array($condition->value)) {
                    if (count($condition->value))
                        return 'false';
                    if (count($condition->value) == 1)
                        foreach($condition->value as $value)
                            $result .= '= '.$this->get_whereConditionValue($value);
                    else {
                        $values = array();
                        foreach ($condition->value as $value)
                            $values = $this->get_whereConditionValue($value);
                        $result .= 'in ('.implode(',', $values).')';
                    }
                } else {
                    $result .= '= '.$this->get_whereConditionValue($condition->value, $condition->valueTable);
                }
                break;

            /* Mathematic operators */
            case 'between':
                if (is_array($condition->value)) {
                    if (count($condition->value) == 2)
                        $result .= 'between '.$this->get_whereConditionValue($condition->value[0], $condition->valueTable).' and '.$this->get_whereConditionValue($condition->value[1], $condition->valueTable);
                    elseif (count($condition->value) == 1)
                        $result .= 'between 0 and '.$this->get_whereConditionValue($condition->value[0], $condition->valueTable);
                    else
                        return 'false';
                } else {
                    $result .= 'between 0 and '.$this->get_whereConditionValue($condition->value, $condition->valueTable);
                }
                break;
            case '>':
            case '<':
            case '>=':
            case '<=':
            case '!=':

            /* Default operator */
            case '=':
            default:
                $result .= $condition->operator.' '.$this->get_whereConditionValue($condition->value, $condition->valueTable);
        }

        /* Return result */
        return $result;
    }

    /**
     * Return encoded value for single where condition.
     * @param mixed $value
     * @return mixed
     */
    protected function get_whereConditionValue($value, $table=''): mixed {
        if ($table !== '')
            return $table.'`'.$value.'`';
        if (is_numeric($value))
            return $value;
        return $this->get_escapedValue($value);
    }

    /**
     * Return query order by from data.
     * @param array $orderBy
     * @return string
     */
    protected function get_orderBy($orderBy): string {
        /* Check order by */
        if (count($orderBy) == 0)
            return '';

        /* Initialise result list */
        $list = array();

        /* Read order by tables */
        foreach($orderBy as $table) {
            if ($table['type'] == 'special')
                $s = $table['type'].'()';
            else {
                $s = '';
                if ($table['value'] != '')
                    $s .= $table['value'].'.';
                $s .= $table['column'];
                if ($table['way'] != '')
                    $s .= ' '.$table['way'];
            }
            $list[] = $s;
        }

        /* Return computed result */
        return ' order by '.implode(',', $list);
    }

    /**
     * Return query limits from data.
     * @param array $limits
     * @return string
     */
    protected function get_limits($limits): string {
        if ($limits['count'] > 0 and $limits['offset'] > 0)
            return ' limit '.$limits['offset'].', '.$limits['count'];

        if ($limits['count'] > 0)
            return ' limit '.$limits['count'];

        if ($limits['offset'] > 0)
            return ' limit '.$limits['offset'];

        return '';
    }

    /* Mark: Execute built select query sub methods */

    /**
     * Return select query items from data.
     * @param array $items
     * @return string
     */
    protected function get_selectItems($items): string {
        /* Initialise result list */
        $list = array();

        /* Read items */
        foreach($items as $item) {
            $s = '';
            if ($item['table'] != '')
                $s .= $item['table'].'.';
            $s .= $item['column'];
            if ($item['function'] != '')
                $s = $item['function'].'('.$s.')';
            if ($item['alias'] != '')
                $s .= ' as '.$item['alias'];
            $list[] = $s;
        }

        /* Return computed result */
        return implode(',', $list);
    }

    /**
     * Return select query group by from data.
     * @param array $groupBy
     * @return string
     */
    protected function get_selectGroupBy($groupBy): string {
        /* Check group by */
        if (count($groupBy) == 0)
            return '';

        /* Initialise result list */
        $list = array();

        /* Read group by tables */
        foreach($groupBy as $table) {
            $s = '';
            if ($table['table'] != '')
                $s .= $table['table'].'.';
            $s .= $table['column'];
            $list[] = $s;
        }

        /* Return computed result */
        return ' group by '.implode(',', $list);
    }

    /* Mark: Execute built insert query sub methods */

    /**
     * Return insert query partition from data.
     * @param array $partition
     * @return string
     */
    protected function get_insertPartition($partition): string {
        return '(`'.implode('`,`', $partition).'`)';
    }

    /**
     * Return insert query values from data.
     * @param array $valueList
     * @return string
     */
    protected function get_insertValues($valueList): string {
        /* Initialise result list */
        $list = array();

        /* Read values */
        foreach ($valueList as $values) {
            $v = array();
            foreach ($values as $value)
                $v[] = $this->get_escapedValue($value);
            $list[] = '('.implode(', ', $v).')';
        }

        /* Return computed result */
        return implode(',', $list);
    }

    /* Mark: Execute built update query sub methods */

    /**
     * Return update query assignments from data.
     * @param array $assignments
     * @return string
     */
    protected function get_updateAssignments($assignments): string {
        /* Initialise result list */
        $list = array();

        /* Read values */
        foreach ($assignments as $assignment)
            $list[] = '`'.$assignment['name'].'`='.$this->get_escapedValue($assignment['value']);

        /* Return computed result */
        return implode(',', $list);
    }

    /**
     * Return default assignments for replace query from insert partition and values.
     * @param DbQuery $query
     * @return string
     */
    protected function get_replaceAssignments(DbQuery $query): string {
        $partition = $query->get_insertPartition();
        $values = $query->get_insertValues();
        if (count($partition) === 0 or count($values) === 0)
            return '';

        $firstRow = $values[0];
        $list = array();
        foreach ($partition as $idx => $col) {
            if (array_key_exists($idx, $firstRow))
                $list[] = '`'.$col.'`='.$this->get_escapedValue($firstRow[$idx]);
        }
        return implode(',', $list);
    }

    /* Mark: Specific tool methods */

    /**
     * Return SQL way to write given value.
     * @param mixed $value
     * @return string
     */
    protected function get_escapedValue($value): string {
        return '"'.$this->escape_string($value).'"';
    }
}
?>
