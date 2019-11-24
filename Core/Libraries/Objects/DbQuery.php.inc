<?php
/**
 * @brief CaMykS Database query builder library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0.3
 * @date Creation: Oct 2019
 * @date Modification: Nov 2019
 * @copyright 2019 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 * @todo Management of where conditions including the parsing from a SQL query. <br />
 * Enhancement of select items management, with unsupported functions, that require additional data.
 */

namespace CaMykS2;

/**
 * DbQuery class.
 */
final class DbQuery extends CaMykSLibrary {
    /**
     * @var array $actionValues
     * @brief List of available actions.
     * @note Will become a constant after dropping support for PHP 5.3.
     */
    private $actionValues = array('select', 'insert', 'update', 'delete');

    /**
     * @var array $itemsFunction
     * @brief List of functions applicable to items.
     * @note Will become a constant after dropping support for PHP 5.3.
     */
    private $itemsFunctions = array(
        /* Math functions */
        'rand', 'round', 'avg', 'count', 'max', 'min', 'sum',
        /* String functions */
        'length', 'soundex', 'reverse', 'trim', 'ltrim', 'rtrim', 'upper', 'lower', 'ucase', 'lcase',
        /* Unsupported string methods *
        'concat', 'replace', 'substring', 'left', 'right', 'lpad', 'rpad', 'locate', 'instr',
        */
        /* Encryption methods */
        'md5',
    );

    /**
     * @var array $joinValues
     * @brief Define join values.
     * @note Will become a constant after dropping support for PHP 5.3.
     */
    private $joinValues = array('inner', 'outer', 'left', 'right', 'cross', 'full', 'natural');

    /**
     * @var array $orderBySpecialValues
     * @brief Define order by special values.
     * @note Will become a constant after dropping support for PHP 5.3.
     */
    private $orderBySpecialValues = array('rand');

    /**
     * @var array $orderByWayValues
     * @brief Define order by way values.
     * @note Will become a constant after dropping support for PHP 5.3.
     */
    private $orderByWayValues = array('asc', 'desc');

    /**
     * @var string $action
     * @brief Database action to execute.
     */
    private $action = '';

    /**
     * @var array $tables
     * @brief Database tables list.
     */
    private $tables = array();

    /**
     * @var array $whereConditions
     * @brief Where conditions for queries.
     */
    private $whereConditions = array();

    /**
     * @var array $orderBy
     * @brief Order by conditions for queries.
     */
    private $orderBy = array();

    /**
     * @var int $limitCount
     * @brief Limit count value for queries.
     */
    private $limitCount = -1;

    /**
     * @var int $limitOffset
     * @brief Limit offset value for select queries.
     */
    private $limitOffset = -1;

    /**
     * @var array $selectItems
     * @brief Tables items for select queries.
     */
    private $selectItems = array();

    /**
     * @var array $selectGroupBy
     * @brief Group by conditions for select queries.
     */
    private $selectGroupBy = array();

    /**
     * @var array $insertPartition
     * @brief Table partition for insert queries.
     */
    private $insertPartition = null;

    /**
     * @var array $insertValues
     * @brief Values lists to be inserted for insert queries.
     */
    private $insertValues = array();

    /**
     * @var array $updateAssignments
     * @brief Table assignments with associated values for update queries.
     */
    private $updateAssignments = array();

    /**
     * Class constructor.
     * @param string $data
     * @return boolean success
     */
    public function __construct($data='') {
        parent::__construct();

        /* check if data is empty */
        if ($data == '')
            return true;

        /* check if data is a known action */
        elseif (in_array($action, $this->actionValues)) {
            $this->action = $action;
            return true;

        /* check if data is a SQL command */
        } elseif ($this->read_fromSQL($data))
            return true;

        /* no valid data */
        return false;
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            /* Action related errors */
            1  => 'ActionAlreadyDefined',
            2  => 'ActionUnknown',
            3  => 'ActionIsNotDefined',

            /* Generic related errors */
            11 => 'TableIsNull',
            12 => 'MultiTableIsOnlySupportedBySelectCommand',
            13 => 'TableJoinUnknown',
            14 => 'OrderBySpecialUnknown',
            15 => 'OrderByWayUnknown',
            16 => 'ColumnNameInvalid',
            17 => 'ColumnFunctionUnknown',

            /* Select related errors */
            21 => 'GroupByDataIsInvalid',
            22 => 'LimitDataIsInvalid',

            /* Insert related errors */
            31 => 'PartitionIsOnlyAvailableWithInsertCommand',
            32 => 'PartitionDataIsInvalid',
            33 => 'ValuesDataIsInvalid',
            34 => 'ValuesDataHasNotTheSameItemCount',
            35 => 'ValuesDataDoesNotCorrespondToPartition',

            /* Update related errors */
            41 => 'AssignmentIsOnlyAvailableWithUpdateCommand',
            42 => 'AssignmentDataIsInvalid',

            /* Delete related errors */


            /* SQLReader related errors */
            61 => 'SQLQueryIsInvalid',
        );
    }

    /* Check methods */

    /**
     * Check query is ready to be run.
     * @return boolean result
     */
    public function is_ready() {

        /* Return success */
        return true;
    }

    /* Export methods */

    /**
     * Export query as array.
     * @return array
     */
    public function export() {
        switch ($this->action) {
            case 'select' : return $this->_export_selectQuery();
            case 'insert' : return $this->_export_insertQuery();
            case 'update' : return $this->_export_updateQuery();
            case 'delete' : return $this->_export_deleteQuery();
        }
    }

    /**
     * Export query as JSON.
     * @return array
     */
    public function export_asJSON() {
        return json_encode($this->export());
    }

    /* Generic data methods */

    /**
     * Define action to execute.
     * @param string $action
     * @return boolean success
     */
    public function set_action($action='') {
        /* Check action is already set */
        if ($this->action != '')
            return $this->add_error(2, $this->action, __FILE__, __LINE__);

        /* Check is known action */
        if (!in_array($action, $this->actionValues))
            return $this->add_error(1, $action, __FILE__, __LINE__);

        /* Save action and return success */
        $this->action = strtolower($action);
        return true;
    }

    /**
     * Return action value.
     * @return string
     */
    public function get_action() {
        return $this->action;
    }

    /**
     * Define destination table.
     * @param string $table
     * @param string $alias
     * @param string $join
     * @return boolean success
     */
    public function add_table($table=null, $alias=null, $join='inner') {
        /* Check if query action is defined */
        if ($this->action == '')
            return $this->add_error(3, '', __FILE__, __LINE__);

        /* Check table value */
        if (is_null($table))
            return $this->add_error(11, '', __FILE__, __LINE__);

        /* Check already defined table and action */
        if (count($this->tables) == 1 and $this->action != 'select')
            return $this->add_error(12, $this->action, __FILE__, __LINE__);

        /* Check already defined table and action */
        if (!in_array(strtolower($join), $this->joinValues))
            return $this->add_error(13, $join, __FILE__, __LINE__);

        /* Add table to table list */
        $this->tables[] = array('table'=>$table, 'alias'=>$alias, 'join'=>$join);

        /* Return success */
        return true;
    }

    /**
     * Return tables values.
     * @return array
     */
    public function get_tables() {
        return $this->tables;
    }




    /**
     * Return whereConditions values.
     * @return array
     */
    public function get_whereConditions() {
        return $this->whereConditions;
    }

    /**
     * Add order by.
     * @param string $value
     * @param string $way
     * @param string $table
     * @param string $type
     * @return boolean success
     */
    public function add_orderBy($value, $way='asc', $table='', $type='column') {
        /* Put some values in lower case */
        $way = strtolower($way);
        $type = strtolower($type);

        /* Check for null value */
        if (is_null($value)) {
            return $this->add_error(16, '', __FILE__, __LINE__);

        /* Check for rand() value */
        } elseif (strtolower($value) == 'rand()') {
            $value = 'rand';
            $type = 'special';

        /* Check for special value */
        } elseif ($type == 'special') {
            $value = strtolower($value);
            if (!in_array($value, $this->orderBySpecialValues))
              return $this->add_error(14, $value, __FILE__, __LINE__);

        /* Check way */
        } elseif (!in_array($way, $this->orderByWayValues))
            return $this->add_error(15, $way, __FILE__, __LINE__);

        /* Add value to list */
        $this->orderBy[] = array('value'=>$value, 'way'=>$way, 'table'=>$table, 'type'=>$type);

        /* Return success */
        return true;
    }

    /**
     * Return order by list.
     * @return array
     */
    public function get_orderBy() {
        return $this->orderBy;
    }

    /**
     * Set limit.
     * @param integer $count
     * @param integer $offset
     * @return boolean success
     */
    public function set_limit($count=-1, $offset=-1) {
        $this->limitCount = (int)$count;
        $this->limitOffset = (int)$offset;
        return true;
    }

    /**
     * Return limit count.
     * @return integer
     */
    public function get_limitCount() {
        return $this->limitCount;
    }

    /**
     * Return limit offset.
     * @return integer
     */
    public function get_limitOffset() {
        return $this->limitOffset;
    }

    /**
     * Return limits.
     * @return array
     */
    public function get_limits() {
        return array('count'=>$this->limitCount, 'offset'=>$this->limitOffset);
    }

    /* Select specific methods */

    /**
     * Add select item.
     * @param mixed $column
     * @param string $alias
     * @param string $table
     * @param string $function
     * @return boolean success
     */
    public function add_selectItem($column, $alias='', $table='', $function='') {
        /* Check for null value */
        if (is_null($column))
            return $this->add_error(21, '', __FILE__, __LINE__);
        if (is_string($column))
            $column = array('column'=>$column, 'alias'=>$alias, 'table'=>$table, 'function'=>$function);

        /* Add value to list */
        $this->selectItems[] = $column;

        /* Return success */
        return true;
    }

    /**
     * Return select items list.
     * @return array
     */
    public function get_selectItems() {
        return $this->selectItems;
    }

    /**
     * Add select group by.
     * @param string $column
     * @param string $table
     * @return boolean success
     */
    public function add_selectGroupBy($column, $table='') {
        /* Check for null value */
        if (is_null($column))
            return $this->add_error(22, '', __FILE__, __LINE__);

        /* Add value to list */
        $this->selectGroupBy[] = array('column'=>$column, 'table'=>$table);

        /* Return success */
        return true;
    }

    /**
     * Return select group by list.
     * @return array
     */
    public function get_selectGroupBy() {
        return $this->selectGroupBy;
    }

    /* Insert specific methods */

    /**
     * Set insert partition.
     * @param array $partition
     * @return boolean succes
     */
    public function set_insertPartition($partition=null) {
        /* Check if query action is defined to insert */
        if ($this->action != 'insert')
            return $this->add_error(31, $this->action, __FILE__, __LINE__);

        /* Check partition */
        if (!is_array($partition) or !array_checkDataType($partition, 'string'))
            return $this->add_error(32, '', __FILE__, __LINE__);

        /* Define partition */
        $this->insertPartition = $partition;

        /* Return success */
        return true;
    }

    /**
     * Return insert partition.
     * @return array
     */
    public function get_insertPartition() {
        return $this->insertPartition;
    }

    /**
     * Add insert values.
     * @param string $values
     * @return boolean success
     */
     public function add_insertValues($values) {
        /* Check if query action is defined to insert */
        if ($this->action != 'insert')
            return $this->add_error(31, $this->action, __FILE__, __LINE__);

        /* Check values */
        if (!is_array($values))
            return $this->add_error(33, '', __FILE__, __LINE__);

        /* Check values length */
        if ($this->insertPartition != null) {
            if (count($this->insertPartition) != count($values))
                return $this->add_error(35, '', __FILE__, __LINE__);
        } elseif (count($this->insertValues) > 0 and count($this->insertValues[0]) != count($values))
            return $this->add_error(34, '', __FILE__, __LINE__);

        /* Add extra checks for associative array */
        if (array_isAssociative($values)) {
            if ($this->insertPartition != null) {
                /* Check keys are identical and put values in the partition orders */
                $v = array();
                foreach ($this->insertPartition as $key) {
                    if (isset($values[$key]))
                        $v[] = $values[$key];
                    else
                        return $this->add_error(35, '', __FILE__, __LINE__);
                }
            } else {
                /* Use keys as partition, if keys are correct */
                $this->set_insertPartition(array_keys($values));
            }
            $values = array_values($values);
        }

        /* Add the values */
        $this->insertValues[] = $values;

        /* Return success */
        return true;
     }

     /**
      * Add insert values list.
      * @param array $list
      * @return boolean success
      */
     public function add_insertValuesList($list) {
        /* Check if query action is defined to insert */
        if ($this->action != 'insert')
            return $this->add_error(31, $this->action, __FILE__, __LINE__);

        /* Check values */
        if (!is_array($list))
            return $this->add_error(33, '', __FILE__, __LINE__);

        /* Add values */
        foreach ($list as $values)
            if (!$this->add_insertValues($values))
                return false;

        /* Return sucess */
        return true;
     }

     /**
      * Return insert values list.
      * @return array
      */
    public function get_insertValues() {
        return $this->insertValues;
    }

     /* Update specific methods */

    /**
     * Add update assignment.
     * @param string $name
     * @param mixed $value
     * @return boolean success
     */
    public function add_updateAssignment($name=null, $value=null) {
        /* Check if query action is defined to update */
        if ($this->action != 'update')
            return $this->add_error(41, $this->action, __FILE__, __LINE__);

        /* Check name and value */
        if (!is_string($name))
            return $this->add_error(42, '', __FILE__, __LINE__);

        /* Add name and value to assignment list */
        $this->updateAssignments[] = array(
            'name'  => $name,
            'value' => $value,
        );

        /* Return success */
        return true;
    }

    /**
     * Return update assignment list.
     * @return array
     */
    public function get_updateAssignments() {
        return $this->updateAssignments;
    }

    /* SQL based methods */

    /**
     * Update query reading and parsing a SQL query.
     * @param string $query
     * @return boolean success
     */
    public function read_fromSQL($query='') {
        /* Trim query */
        $query = trim($query);

        /* Check action */
        $action = strtolower(strtok($query, ' '));
        switch ($action) {
            case 'select':
                return $this->_read_fromSQLSelect($query);
            case 'insert':
                return $this->_read_fromSQLInsert($query);
            case 'update':
                return $this->_read_fromSQLUpdate($query);
            case 'delete':
                return $this->_read_fromSQLDelete($query);
        }

        /* No relevant action found */
        return $this->add_error(2, '', __FILE__, __LINE__);
    }

    /**
     * Update where conditions reading and parsing a SQL where conditions.
     * @param string $conditions
     * @return boolean success
     */
    public function read_whereConditionsFromSQL($conditions='') {

        /* Return success */
        return true;
    }

    /* Private export methods */

    /**
     * Export select query.
     * @return array
     */
    private function  _export_selectQuery() {
        $result = array();
        $result['action'] = 'select';
        $result['items'] = $this->get_selectItems();
        $result['tables'] = $this->get_tables();
        $result['whereConditions'] = $this->get_whereConditions();
        $result['groupBy'] = $this->get_selectGroupBy();
        $result['orderBy'] = $this->get_orderBy();
        $result['limitCount'] = $this->get_limitCount();
        $result['limitOffset'] = $this->get_limitOffset();
        return $result;
    }

    /**
     * Export insert query.
     * @return array
     */
    private function  _export_insertQuery() {
        $result = array();
        $result['action'] = 'insert';
        $result['tables'] = $this->get_tables();
        $result['partition'] = $this->get_insertPartition();
        $result['values'] = $this->get_insertValues();
        return $result;
    }

    /**
     * Export update query.
     * @return array
     */
    private function  _export_updateQuery() {
        $result = array();
        $result['action'] = 'update';
        $result['tables'] = $this->get_tables();
        $result['whereConditions'] = $this->get_whereConditions();
        $result['assignments'] = $this->get_updateAssignments();
        return $result;
    }

    /**
     * Export delete query.
     * @return array
     */
    private function  _export_deleteQuery() {
        $result = array();
        $result['action'] = 'delete';
        $result['tables'] = $this->get_tables();
        $result['whereConditions'] = $this->get_whereConditions();
        $result['orderBy'] = $this->get_orderBy();
        $result['limitCount'] = $this->get_limitCount();
        $result['limitOffset'] = $this->get_limitOffset();
        return $result;
    }

    /* Private read SQL methods */

    /**
     * Update query reading and parsing a select SQL query.
     * @param string $query
     * @return boolean success
     */
    private function _read_fromSQLSelect($query) {
        /* Check SQL query */
        if (!preg_match('/^select (.+) from ([\w\s\,]+)( where (.+))?( group by (.+))?( order by (.+))?( limit ([0-9]+(\s*(\,|offset)\s*[0-9]+)?))?;?$/Ui', $query, $matches))
          return $this->add_error(61, '', __FILE__, __LINE__);

        /* Define action */
        if (!$this->set_action('select'))
            return false;

        /* Read and define items */
        if (!$this->_read_itemsFromSQLSelect($matches[1]))
            return false;

        /* Read and define tables */
        if (!$this->_read_tablesFromSQL($matches[2]))
            return false;

        /* Read and define where condition */
        if (!$this->read_whereConditionsFromSQL($matches[4]))
            return false;

        /* Read and define group by */
        if (!$this->_read_groupByFromSQLSelect($matches[6]))
            return false;

        /* Read and define order by */
        if (!$this->_read_orderByFromSQL($matches[8]))
            return false;

        /* Read and define limit */
        if (!$this->_read_limitFromSQL($matches[10]))
            return false;

        /* Return success */
        return true;
    }

    /**
     * Update query reading and parsing a insert SQL query.
     * @param string $query
     * @return boolean success
     */
    private function _read_fromSQLInsert($query) {
        /* Check SQL query */
        if (!preg_match('/^insert into ([\w]+)( partition)?( \([\w, `]*\))? values (.*);?$/Ui', $query, $matches))
            return $this->add_error(61, '', __FILE__, __LINE__);

        /* Define action */
        if (!$this->set_action('insert'))
            return false;

        /* Define table */
        if (!$this->add_table($matches[1]))
            return false;

        /* Read and define partition */
        if ($matches[3] != '')
            if (!$this->_read_partitionFromSQLInsert($matches[3]))
                return false;

        /* Read and define values */
        if (!$this->_read_valuesFromSQLInsert($matches[4]))
            return false;

        /* Return success */
        return true;
    }

    /**
     * Update query reading and parsing a update SQL query.
     * @param string $query
     * @return boolean success
     */
    private function _read_fromSQLUpdate($query) {
        /* Check SQL query */
        if (!preg_match('/^update ([\w]+) set (.*) where (.*);?$/Ui', $query, $matches))
            return $this->add_error(61, '', __FILE__, __LINE__);

        /* Define action */
        if (!$this->set_action('update'))
            return false;

        /* Define table */
        if (!$this->add_table($matches[1]))
            return false;

        /* Read and define values */
        if (!$this->_read_assignmentsFromSQLUpdate($matches[2]))
            return false;

        /* Read and define where conditions */
        if (!$this->read_whereConditionsFromSQL($matches[3]))
            return false;

        /* Return success */
        return true;
    }

    /**
     * Update query reading and parsing a delete SQL query.
     * @param string $query
     * @return boolean success
     */
    private function _read_fromSQLDelete($query) {
        /* Check SQL query */
        if (!preg_match('/^delete from ([\w]+) where (.*)( order by (.+))?( limit ([0-9]+(\s*(\,|offset)\s*[0-9]+)?))?;?$/Ui', $query, $matches))
            return $this->add_error(61, '', __FILE__, __LINE__);

        /* Define action */
        if (!$this->set_action('delete'))
            return false;

        /* Define table */
        if (!$this->add_table($matches[1]))
            return false;

        /* Read and define where conditions */
        if (!$this->read_whereConditionsFromSQL($matches[2]))
            return false;

        /* Read and define order by */
        if (!$this->_read_orderByFromSQL($matches[4]))
            return false;

        /* Read and define limit */
        if (!$this->_read_limitFromSQL($matches[6]))
            return false;

        /* Return success */
        return true;
    }

    /**
     * Update tables, reading and parsing from a SQL query.
     * @param string $data
     * @return boolean success
     */
    private function _read_tablesFromSQL($data) {
        $data = trim($data);

        /* Split tables */
        $data = preg_split('/((\,)|('.implode('|', $this->joinValues).')[\s]*join)/Ui', $data, 0, PREG_SPLIT_DELIM_CAPTURE);

        /* Check tables one by one */
        for ($i=0; $i < count($data); $i = $i+4) {
            $t = trim($data[$i]);
            if (!isset($data[$i-1]) or $data[$i-1] = ',')
                $j = 'inner';
            else
                $j = strtok(' ',$data[$i-1]);;

            if (preg_match('/^(\w*)\s*(as)?\s*(\w*)$/Ui', $t, $matches)) {
                if (!$this->add_table(trim($matches[1]), trim($matches[3]), $j))
                    return false;
            } elseif (!$this->add_table($t))
                return false;
        }

        /* Return success */
        return true;
    }

    /**
     * Update order by, reading and parsing from a SQL query.
     * @param string $data
     * @return boolnean success
     */
    private function _read_orderByFromSQL($data) {
        $data = trim($data);

        /* Split all values */
        $data = explode(',', $data);

        foreach ($data as $o) {
            $o = trim($o);

            /* Check special value */
            if (preg_match('/^('.implode('|', $this->orderBySpecialValues).')\(\)$/', $o, $matches)) {
                if (!$this->add_orderBy($matches[1], '', '', 'special'))
                    return false;
                continue;
            }

            /* Check way is indicated */
            if (preg_match('/^(.*)(\s+)('.implode('|', $this->orderByWayValues).')$/Ui', $o, $matches)) {
                $column = $matches[1];
                $way = $matches[3];
            } else {
                $column = $o;
                $way = 'asc';
            }

            /* Read column details */
            $column = $this->_read_columnFromSQL($column);

            /* Check result */
            if ($column === false)
                return false;
            if ($column['alias'] != '' or $column['function'] != '')
                return $this->add_error(16, '', __FILE__, __LINE__);

            /* Add column */
            if (!$this->add_orderBy($column['column'], $way, $column['table']))
                return false;
        }

        /* Return success */
        return true;
    }

    /**
     * Update limit, reading and parsing from a SQL query.
     * @param string $data
     * @return boolean success
     */
    private function _read_limitFromSQL($data) {
        $data = trim($data);

        /* Check for count and offset separated by a comma */
        if (preg_match('/^([0-9]+)\s*,\s*([0-9]+)$/', $data, $matches)) {
            $this->set_limit((int)$matches[2], (int)$matches[1]);

        /* Check for count and offset separated by offset key word */
        } elseif (preg_match('/^([0-9]+)\s*offset\s*([0-9]+)$/i', $data, $matches)) {
            $this->set_limit((int)$matches[1], (int)$matches[2]);

        /* Check for count only value */
        } elseif ((int)$data >= 0) {
            $this->set_limit((int)$data);

        /* Else limit data is malformed */
        } else {
            return $this->set_error(22);
        }

        /* Return success */
        return true;
    }

    /**
     * Update select items, reading and parsing from a SQL query.
     * @param string $data
     * @return boolean success
     */
    private function _read_itemsFromSQLSelect($data) {
        $data = trim($data);

        /* Split all values */
        $data = explode(',', $data);

        foreach ($data as $c) {
            $c = trim($c);

            /* Read column details */
            $column = $this->_read_columnFromSQL($c);

            /* Check result */
            if ($column === false)
                return false;

            /* Add column */
            if (!$this->add_selectItem($column))
                return false;
        }

        /* Return success */
        return true;
    }

    /**
     * Update select group by, reading and parsing from a SQL query.
     * @param string $data
     * @return boolnean success
     */
    private function _read_groupByFromSQLSelect($data) {
        $data = trim($data);

        /* Split all values */
        $data = explode(',', $data);

        foreach ($data as $c) {
            $c = trim($c);

            /* Read column details */
            $column = $this->_read_columnFromSQL($c);

            /* Check result */
            if ($column === false)
                return false;
            if ($column['alias'] != '' or $column['function'] != '')
                return $this->add_error(21, '', __FILE__, __LINE__);

            /* Add column */
            if (!$this->add_selectGroupBy($column['column'], $column['table']))
                return false;
        }

        /* Return success */
        return true;
    }

    /**
     * Update insert partition, reading and parsing from SQL query.
     * @param string $data
     * @return boolean success
     */
    private function _read_partitionFromSQLInsert($data) {
        $data = trim($data);

        /* Remove parenthesis */
        $data = substr($data, 1, -1);

        /* Split items */
        $data = explode(',', $data);

        /* Initialise partition */
        $partition = array();

        /* Go through all items */
        foreach ($data as $item) {
            $item = trim($item);
            if (preg_match('/^`(\w+)`$/', $item, $matches))
                $partition[] = $matches[1];
            else
                $partition[] = $item;
        }

        /* Add partition to query */
        if (!$this->set_insertPartition($partition))
            return false;

        /* Return success */
        return true;
    }

    /**
     * Update values from insert query, reading and parsing SQL query.
     * @param string $data
     * @return boolean success
     */
    private function _read_valuesFromSQLInsert($data) {
        $data = trim($data);

        /* Go through all values */
        while ($data != '' and strpos($data, '(') !== false) {
            /* Add first values found */
            $result = $this->_read_singleValuesFromSQLInsert($data);
            if ($result === false)
                return false;

            /* Reduce values list */
            $data = $result;
        }

        /* Return success */
        return true;
    }

   /**
     * Update single values from insert query, reading and parsing SQL query.
     * @param string $data
     * return boolean success
     */
    private function _read_assignmentsFromSQLUpdate($data) {
        /* Read assignment values */
        $values = explode(',', $data);
        foreach ($values as $value) {
            $value = trim($value);
            $value = explode('=', $value);
            if (!$this->add_updateAssignment(trim($value[0]), $this->_read_string($value[1])))
                return false;
        }

        /* Return success */
        return true;
    }

    /* Sub reading methods */

    /**
     * Read and return column details.
     * @param string $data
     * @return array
     */
    private function _read_columnFromSQL($data) {
        /* Initialise result */
        $column = array('column'=>'', 'alias'=>'', 'table'=>'', 'function'=>'');

        /* Check for alias */
        if (preg_match('/^(\S*)\s+as\s+(\w+)$/Ui', $data, $matches)) {
            $column['alias'] = $matches[2];
            $data = $matches[1];
        }

        /* Check for function */
        if (preg_match('/^(\w+)\((\S+)\)$/Ui', $data, $matches)) {
            if (!in_array($matches[1], $this->itemsFunctions))
                return $this->add_error(17, '', __FILE__, __LINE__);
            $column['function'] = $matches[1];
            $data = $matches[2];
        }

        /* Check for table and column name */
        if (preg_match('/^((\w+)\.)?`?(\w+)`?$/Ui', $data, $matches)) {
            $column['table'] = $matches[2];
            $column['column'] = $matches[3];
        } else
            return $this->add_error(16, '', __FILE__, __LINE__);

        /* Return success */
        return $column;
    }

    /**
     * Read and insert single values from insert query, reading and parsing SQL query.
     * @param string $data
     * @return srting
     */
    private function _read_singleValuesFromSQLInsert($data) {
        /* Initialise result */
        $values = array();

        /* Remove opening parenthesis */
        $data = trim(substr($data, 1));

        /* Get position of next key characters */
        $closePos = strpos($data, ')');
        $commaPos = strpos($data, ',');
        $quotePos = strpos($data, '"');
        $apostPos = strpos($data, '\'');

        /* Check closing parenthesis */
        if ($closePos === false)
            return $this->add_error(33, '', __FILE__, __LINE__);

        /* Check for no string as variable */
        if ($quotePos === false and $apostPos === false) {
            /* Quickly get non-string values from data */
            $d = substr($data, 0, $closePos);
            $d = explode(',', $d);
            foreach ($d as $v)
                $values[] = json_decode($v);

        } else {
            /* Find array items */
            while ($data != '') {

                /* Get values, including strings from data */
                $nextCharPos = $closePos;
                if ($commaPos !== false) $nextCharPos = min($nextCharPos, $commaPos);
                if ($quotePos !== false) $nextCharPos = min($nextCharPos, $quotePos);
                if ($apostPos !== false) $nextCharPos = min($nextCharPos, $apostPos);

                /* Check for right parenthesis as first character */
                if ($nextCharPos == $closePos and $closePos === 0)
                    break;

                /* Check for comma as first character */
                if ($nextCharPos == $commaPos and $commaPos === 0) {
                    $data = trim(substr($data, 1));

                /* Check for comma / right parenthesis as next key character */
                } elseif ($nextCharPos === $closePos or $nextCharPos === $commaPos) {
                    $value = json_decode(trim(substr($data, 0, $nextCharPos)));
                    /* Check for unreadable value */
                    if ($value == '')
                        return $this->add_error(33);
                    $values[] = $value;
                    $data = substr($data, $nextCharPos);

                /* Check for quotes as next key character */
                } elseif ($nextCharPos === $quotePos or $nextCharPos === $apostPos) {
                    if ($nextCharPos === $quotePos) {
                        $qChar = '"';
                        $qqChar = '\"';
                    } else {
                        $qChar = "'";
                        $qqChar = "\'";
                    }

                    $cursor = 1;
                    $temp = substr($data, $cursor);

                    $qPos = strpos($temp, $qChar);
                    $qqPos = strpos($temp, $qqChar);

                    if ($qPos === false)
                        return $this->add_error(33, '', __FILE__, __LINE__);

                    while ($qqPos >= 0 and $qPos == $qqPos+1) {
                        $cursor += $qPos+1;
                        $temp = substr($data, $cursor);

                        $qPos = strpos($temp, $qChar);
                        $qqPos = strpos($temp, $qqChar);

                        if ($qPos === false)
                              return $this->add_error(33, '', __FILE__, __LINE__);
                    }

                    $cursor += $qPos+1;
                    //$value = substr($data, 1, $cursor-2);
                    //$values[] = preg_replace('/\\'.$qqChar.'/', $qChar, $value);

                    $values[] = $this->_read_string(substr($data, 0, $cursor));

                    $data = substr($data, $cursor);
                }

                /* Update data and key char position */
                $data = trim($data);
                $closePos = strpos($data, ')');
                $commaPos = strpos($data, ',');
                $quotePos = strpos($data, '"');
                $apostPos = strpos($data, '\'');
            }
        }

        /* Add values to query */
        if (!$this->add_insertValues($values))
            return false;

        /* Remove close parenthesis, extra spaces and comma */
        if (($p = strpos($data, '(')) === false)
            $data = '';
        else
            $data = substr($data, strpos($data, '('));

        /* Return remaining data */
        return $data;
    }

    /* Private tool methods */

    /**
     * Read value from string.
     * @param string $data
     * @return mixed
     */
    private function _read_string($data) {
        $data = trim($data);

        /* Check is data a string */
        if (!preg_match('/^(\'|")(.*)(\'|")$/U', $data, $matches))
            /* If not, use JSON decode */
            return json_decode($data);

        /* Check quotes */
        if ($matches[1] != $matches[3]) // quotes are not identical, what to do
            return $data;

        /* Stripslashes only for external quote */
        $value = preg_replace('/\\\\'.$matches[1].'/', $matches[1], $matches[2]);

        /* Return value */
        return $value;
    }
}
?>
