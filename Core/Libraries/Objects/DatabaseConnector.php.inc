<?php
/**
 * @brief CaMykS Dababase connector.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre4.2
 * @date Creation: Oct 2019
 * @date Modification: Mar 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * DatabaseConnector class.
 */
final class DatabaseConnector extends CaMykSLibrary {
    /**
     * var array $params
     * @brief Connection parameters.
     */
    protected $params = array(
        'Driver'    => '',
        'PDO'       => false,
        'Host'      => 'localhost',
        'Port'      => null,
        'Login'     => '',
        'Password'  => '',
        'Database'  => '',
    );

    /**
     * @var Object $connection
     * @brief Connection with the driver and the database.
     */
    private $connection = false;

    /**
     * @var array $statistics
     * @brief Statistics to record.
     */
    private $statistics = array('Queries'=>0, 'FetchedLines'=>0, 'UpdatedLines'=>0);

    /**
     * @var array $queries
     * @brief List of executed queries.
     */
    private $queries = array();

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct($params=array()) {
        global $CaMykS;

        /* Merge parameters in parent object */
        parent::__construct($params);

        /* Check driver */
        if ($this->params['PDO']) {
            $driver = 'PDODBConnection';
        } else {
            /* Preload SQL interface for SQL drivers */
            if (preg_match('/sql/i', $this->params['Driver']))
                $CaMykS->load_library('DatabaseDrivers/SQLDatabaseDriverInterface', 'Objects');

            /* Define driver library */
            switch ($this->params['Driver']) {
                case 'SQLite3':
                    $driver = 'SQLite3DatabaseDriver';
                    break;
                case 'PgSQL':
                    $driver = 'PgSQLDatabaseDriver';
                    break;
                case 'MySQLi':
                    $driver = 'MySQLiDatabaseDriver';
                    break;
                case 'MySQL':
                    $driver = 'MySQLDatabaseDriver';
                    break;
                default :
                    return $this->add_error(1);
            }
        }

        /* Load driver library */
        if (!$CaMykS->load_library('DatabaseDrivers/'.$driver, 'Objects'))
            return $this->add_error(2);

       /* Load connection */
       $this->connection = new $$driver($this);

        /* Register connector in CaMykS */
        $CaMykS->register_databaseConnection($this);
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            /* Driver related errors */
            1  => 'DriverUnknown',
            2  => 'DriverUnavailable',
        );
    }

    /* Check methods */

    /**
     * Return connector status.
     * @return boolean result
     */
    public function check_status() {
        if ($this->connection === false)
            return false;
        return $this->connection->check_status();
    }

    /**
     * Return connection status.
     * @return boolean result
     */
    public function check_connection() {
        if ($this->connection === false)
            return false;
        return $this->connection->check_connection();
    }

    /* Connection methods */

    /**
     * Initiate driver connection.
     * @return boolean success
     */
    public function open_connection() {
        if ($this->connection === false)
            return false;
        return $this->connection->open();
    }

    /**
     * Close drive connection.
     * @return boolean success
     */
    public function close_connection() {
        if ($this->connection === false)
            return false;
        return $this->connection->close();
    }

    /* Driver methods */

    /**
     * Return database driver version.
     * @return string
     */
    public function get_driverVersion() {
        return $this->connection->get_driverVersion();
    }

    /* Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database($name=null) {
        if ($name != null) $this->params['database'] = $name;
        return $this->connection->select_database($this->params['database']);
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database($name=null) {
        if ($name != null) $this->params['database'] = $name;
        return $this->connection->check_database($this->params['database']);
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $charset
     * @return boolean
     */
    public function create_database($name, $charset='UTF8') {
        if ($name != null) $this->params['database'] = $name;
        return $this->connection->create_database($this->params['database'], $charset);
    }

    /**
     * Dump database.
     * @param mixed $file
     * @return mixed
     */
    public function dump_database($file=false) {
        return $this->connection->dump_database($file);
    }

    /**
     * Return database size.
     * @return integer
     */
    public function get_databaseSize() {
        return $this->connection->get_databaseSize();
    }

    /**
     * Return database summary.
     * @return array
     */
    public function get_databaseSummary() {
        return $this->connection->get_databaseSummary();
    }

    /* Table methods */

    /**
     * Return database table list.
     * @return array
     */
    public function get_tableList() {
        return $this->connection->get_tableList();
    }

    /**
     * Create a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function create_table($params=array()) {
        return $this->connection->create_table($params);
    }

    /**
     * Duplicate a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function duplicate_table($params=array()) {
        return $this->connection->duplicate_table($params);
    }

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function drop_table($name) {
        return $this->connection->drop_table($name);
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        return $this->connection->truncate_table($name);
    }

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function alter_table($params=array()) {
        return $this->connection->alter_table($params);
    }

    /* Query methods */

    /**
     * Execute direct query.
     * @param string $query
     * @return boolean success
     */
    public function execute_directQuery($query) {
        return $this->connection->execute_directQuery($query);
    }

    /**
     * Execute prepared query using CQuery builder.
     * @param Object $query
     * @return boolean success
     */
    public function execute_builtQuery($query) {
        return $this->connection->execute_builtQuery($query);
    }

    /**
     * Record given query from connection library to query list
     * @param string $query
     * @return void
     */
    public function record_query($query) {
        $query = trim($query);
        if ($query != '')
            $this->queries[] = $query;
    }

    /* Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId($resourceName='DefaultResource') {
        return $this->connection->get_queryGeneratedId($resourceName);
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine($resourceName='DefaultResource') {
        if ($l = $this->connection->get_queryDataLine($resourceName)) {
            $this->statistics['FetchedLines']++;
            return $l;
        }
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine($resourceName='DefaultResource') {
        if ($l = $this->connection->get_queryNumDataLine($resourceName)) {
            $this->statistics['FetchedLines']++;
            return $l;
        }
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows($resourceName='DefaultResource') {
        $v = $this->connection->get_queryAffectedRows($resourceName);
        $this->statistics['UpdatedLines'] += $v;
        return $v;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount($resourceName='DefaultResource') {
        return $this->connection->get_queryFieldCount($resourceName);
    }

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string
     */
    public function get_queryFieldType($fIndex=0, $resourceName='DefaultResource') {
        return $this->connection->get_queryFieldType($fIndex, $resourceName);
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_queryError($resourceName='DefaultResource') {
        return $this->connection->get_queryError($resourceName);
    }

    /* Ressource methods */

    /**
     * Close ressource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource($resourceName='DefaultResource') {
        return $this->connection->close_ressource($resourceName);
    }

    /* Data methods */

    /**
     * Return escaped string.
     * @param string $string
     * @return string
     */
    public function escape_string($string='') {
        return $this->connection->escape_string($string);
    }

    /* Static methods */

    /**
     * Return available driver list.
     * <br />Method is static to be called without having a working database connection.
     * @return array
     */
    public static function get_availableDrivers() {
        $list = array();

        if (extension_loaded('mysql'))
            $list['MySQL'] = 'MySQL';
        if (extension_loaded('mysqli'))
            $list['MySQLi'] = 'MySQLi';
        if (extension_loaded('PDO'))
            $list['PDO:MySQL'] = 'PDO:MySQL';
        if (extension_loaded('sqlite3'))
            $list['SQLite3'] = 'SQLite3';
        if (extension_loaded('pgsql'))
            $list['PgSQL'] = 'PgSQL';

        return $list;
    }
}
?>
