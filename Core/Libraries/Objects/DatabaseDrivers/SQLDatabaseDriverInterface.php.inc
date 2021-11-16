<?php
/**
 * @brief Interface for CaMykS SQL database drivers.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Nov 2019
 * @date Modification: Nov 2021
 * @copyright 2019 - 2021 CaMykS Team
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

    /* Override DatabaseDriverInterface methods */

    /**
     * Execute prepared query using DbQuery builder.
     * @param Object $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_builtQuery($query, $resourceName='Default') {
        switch ($query->get_action()) {
            case 'select': return $this->execute_directQuery($this->get_builtSelectQuery($query), $resourceName);
            case 'insert': return $this->execute_directQuery($this->get_builtInsertQuery($query), $resourceName);
            case 'update': return $this->execute_directQuery($this->get_builtUpdateQuery($query), $resourceName);
            case 'delete': return $this->execute_directQuery($this->get_builtDeleteQuery($query), $resourceName);
        }

        /* Return default result */
        return false;
    }

    /* Execute built query methods */

    /**
     * Return select query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtSelectQuery($query) {
        return  'select '.
                $this->get_selectItems($query->get_selectItems()).
                'from '.
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
    protected function get_builtInsertQuery($query) {
        return  'insert into '.
                 $this->get_tables($query->get_tables()).
                //'partition '.
                $this->get_insertPartition($query->get_insertPartition()).
                'values '.
                $this->get_insertValues($query->get_selectInsertValues()).
                ';';
    }

    /**
     * Return update query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtUpdateQuery($query) {
        return  'update '.
                $this->get_tables($query->get_tables()).
                'set '.
                $this->get_updateAssignments($query->get_updateAssignments()).
                'where '.
                $this->get_whereConditions($query->get_whereConditions()).
                ';';
    }

    /**
     * Return delete query using DbQuery builder.
     * @param Object $query
     * @return string
     */
    protected function get_builtDeleteQuery($query) {
        return  'delete from '.
                $this->get_tables($query->get_tables()).
                'where '.
                $this->get_whereConditions($query->get_whereConditions()).
                $this->get_orderBy($query->get_orderBy()).
                $this->get_limits($query->get_limits()).
                ';';
    }

    /* Execute built query generic sub methods */

    /**
     * Return query tables from data.
     * @param array $tables
     * @return string
     */
    protected function get_tables($tables) {
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
     * Return query where condition from data.
     * @param array $whereConditions
     * @return string
     */
    protected function get_whereConditions($whereConditions) {
        /* Initialise result */
        $result = '';

        /* Return result */
        return $result;
    }

    /**
     * Return query order by from data.
     * @param array $orderBy
     * @return string
     */
    protected function get_orderBy($orderBy) {
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
    protected function get_limits($limits) {
        if ($limits['count'] > 0 and $limits['offset'] > 0)
            return ' limit '.$limits['offset'].', '.$limits['count'];

        if ($limits['count'] > 0)
            return ' limit '.$limits['count'];

        if ($limits['offset'] > 0)
            return ' limit '.$limits['offset'];

        return '';
    }

    /* Execute built select query sub methods */

    /**
     * Return select query items from data.
     * @param array $items
     * @return string
     */
    protected function get_selectItems($items) {
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
    protected function get_selectGroupBy($groupBy) {
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

    /* Execute built insert query sub methods */

    /**
     * Return insert query partition from data.
     * @param array $partition
     * @return string
     */
    protected function get_insertPartition($partition) {
        return '(`'.implode('`,`', $partition).'`)';
    }

    /**
     * Return insert query values from data.
     * @param array $valueList
     * @return string
     */
    protected function get_insertValues($valueList) {
        /* Initialise result list */
        $list = array();

        /* Read values */
        foreach ($valueList as $values) {
            $v = array();
            foreach ($values as $value)
                $v[] = $this->get_SQLValue($value);
            $list[] = '('.implode(', ', $v).')';
        }

        /* Return computed result */
        return implode(',', $list);
    }

    /* Execute built update query sub methods */

    /**
     * Return update query assignments from data.
     * @param array $assignments
     * @return string
     */
    protected function get_updateAssignments($assignments) {
        /* Initialise result list */
        $list = array();

        /* Read values */
        foreach ($assignments as $assignment)
            $list[] = '`'.$assignment['name'].'`='.$this->get_SQLValue($value);

        /* Return computed result */
        return implode(',', $list);
    }

    /* Specific tool methods */

    /**
     * Return SQL way to write given value.
     * @param mixed $value
     * @return string
     */
    private function get_SQLValue($value) {
        return '"'.$this->escape_string($value).'"';
    }
}
?>
