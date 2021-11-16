<?php
/**
 * @brief Interface for CaMykS database drivers.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Oct 2019
 * @date Modification: Nov 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * DatabaseDriverInterface class.
 */
abstract class DatabaseDriverInterface {
    /**
     * @var Object $parent
     * @brief Link to parent CDatabaseConnector object.
     */
    protected $parent;

    /**
     * @var boolean $status
     * @brief Status of the database connection.
     */
    protected $status = false;

    /**
     * @var array $queryResources
     * @brief List of open resources.
     */
    protected $queryResources = array();

    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture;

    /**
     * Class constructor.
     * @param Object $parent
     * @return void
     */
    public function __construct(&$parent) {
        $this->parent = $parent;
    }

    /* Parent methods */

    /**
     * Return parameter value.
     * @param string $param
     * @return mixed
     */
    protected function get_param($param) {
        return $this->parent->get_param($param);
    }

    /**
     * Update parameter value.
     * @param string $param
     * @param mixed $value
     * @return mixed
     */
    protected function update_param($param, $value) {
        return $this->parent->update_param($param, $value);
    }

    /**
     * Add an error.
     * @param integer $errorId
     * @param string $query
     * @param string $file
     * @param integer $line
     * @return false
     */
    protected function add_error($errorId, $query, $file='', $line=0) {
        return $this->parent->add_driverError($errorId, $query, $file, $line);
    }

    /* Connection methods */

    /**
     * Open connection.
     * @return boolean success
     */
    abstract public function open();

    /**
     * Close connection.
     * @return boolean success
     */
    abstract public function close();

    /* Driver methods */

    /**
     * Return database driver version.
     * @return string
     */
    abstract public function get_driverVersion();

    /**
     * Return database driver architecture.
     * @return string
     */
    public function get_driverArchitecture() {
        return $this->architecture;
    }

    /* Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    abstract public function select_database($name=null);

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    abstract public function check_database($name=null);

    /**
     * Create a database.
     * @param string $name
     * @param string $charset
     * @return boolean
     */
    abstract public function create_database($name, $charset='UTF8');

    /**
     * Dump database.
     * @return mixed
     */
    abstract public function dump_database();

    /**
     * Return database size.
     * @return integer
     */
    abstract public function get_databaseSize();

    /**
     * Return database summary.
     * @return array
     */
    abstract public function get_databaseSummary();

    /* Table methods */

    /**
     * Return database table list.
     * @return array
     */
    abstract public function get_tableList();

    /**
     * Create a table in the database.
     * @param string $name
     * @param array $fields
     * @param array $params
     * @return boolean success
     */
    abstract public function create_table($name, $fields, $params=array());

    /**
     * Duplicate a table in the database.
     * @param array $params
     * @return boolean success
     */
    abstract public function duplicate_table($params=array());

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    abstract public function drop_table($name);

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    abstract public function truncate_table($name);

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    abstract public function alter_table($params=array());

    /* Query methods */

    /**
     * Execute direct query.
     * @param string $query
     * @param string $resourceName
     * @return boolean success
     */
    abstract public function execute_directQuery($query, $resourceName='Default');

    /**
     * Execute prepared query using DbQuery builder.
     * @param Object $query
     * @param string $resourceName
     * @return boolean success
     */
    abstract public function execute_builtQuery($query, $resourceName='Default');

    /**
     * Execute direct queries.
     * @param string $queries
     * @param string $resourceName
     * @return boolean success
     */
    abstract public function execute_queries($queries, $resourceName='Default');

    /**
     * Record given query from connection library to query list
     * @param string $query
     * @return void
     */
    protected function record_query($query) {
        $this->parent->record_query($query);
    }

    /* Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    abstract public function get_queryGeneratedId($resourceName='Default');

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    abstract public function get_queryDataLine($resourceName='Default');

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    abstract public function get_queryNumDataLine($resourceName='Default');

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    abstract public function get_queryAffectedRows($resourceName='Default');

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    abstract public function get_queryFieldCount($resourceName='Default');

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string
     */
    abstract public function get_queryFieldType($fIndex=0, $resourceName='Default');

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    abstract public function get_queryError($resourceName='Default');

    /* Ressource methods */

    /**
     * Close ressource.
     * @param string $resourceName
     * @return boolean result
     */
    abstract public function close_ressource($resourceName='Default');

    /* Data methods */

    /**
     * Return escaped string.
     * @param string $string
     * @return string
     */
    abstract public function escape_string($string='');

    /* Check methods */

    /**
     * Check connection is active.
     * @return boolean result
     */
    public function is_active() {
        return $this->status;
    }
}
?>
