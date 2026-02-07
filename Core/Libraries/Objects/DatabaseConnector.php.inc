<?php
/**
 * @brief CaMykS Dababase connector.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre10
 * @date Creation: Oct 2019
 * @date Modification: Feb 2026
 * @copyright 2019 - 2026 CaMykS Team
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
        'Driver'        => '',
        'PDO'           => false,
        'Host'          => 'localhost',
        'Port'          => null,
        'Login'         => '',
        'Password'      => '',
        'Name'          => '',
        'TablePrefix'   => '',
    );

    /**
     * @var Object $connection
     * @brief Connection with the driver and the database.
     */
    private $connection = false;

    /**
     * @var string $connectionName
     * @brief Connection name.
     */
    private $connectionName;

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
     * @param string $connectionName
     * @return void
     */
    public function __construct(array $params=array(), string $connectionName='') {
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
                case 'SQLite':
                    $driver = 'SQLiteDatabaseDriver';
                    break;
                case 'PostgreSQL':
                    $driver = 'PostgreSQLDatabaseDriver';
                    break;
                case 'MySQLi':
                    $driver = 'MySQLiDatabaseDriver';
                    break;
                case 'MSSQL':
                    $driver = 'MSSQLDatabaseDriver';
                    break;
                default :
                    return $this->add_error(1, '');
            }
        }

        /* Load driver library */
        if ($CaMykS->load_library('DatabaseDrivers/'.$driver, 'Objects') === false)
            return $this->add_error(2, $driver);

        /* Load connection */
        $driver = $this->libraryNamespace.'\\'.$driver;
        $this->connection = new $driver($this);

        /* Store connection name */
        $this->connectionName = $connectionName;

        /* Register connector in CaMykS */
        $CaMykS->register_databaseConnection($this);
    }

    /* Mark: Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            /* Driver related errors */
            1  => 'DriverUnknown',
            2  => 'DriverUnavailable',

            /* Query related error */
            11 => 'InvalidQuery',
        );
    }

    /* Mark: Driver related methods */

    /**
     * Add an error.
     * @param integer $errorId
     * @param string $query
     * @param string $file
     * @param integer $line
     * @param boolean $isFatal
     * @return false
     */
    public function add_error(int $errorId, string $query, string $file='', int $line=0, bool $isFatal=false) {
        global $CaMykS;

        /* Build details from driver value and query */
        $details = 'Driver: '.$this->params['Driver']."\n";
        $details .= 'Query: '.$query."\n";

        /* Add error at the beginning of the list */
        array_unshift($this->errorList, $errorId);

        /* Send error to CaMykS */
        $CaMykS->trigger_databaseError($this->libraryName, $errorId, $this->get_errorCode($errorId), $details, $file, $line);

        /* Return false */
        return false;
    }

    /* Mark: Check methods */

    /**
     * Return connector status.
     * @return boolean result
     */
    public function check_status() {
        return ($this->connection !== false);
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

    /* Mark: Connection methods */

    /**
     * Return connection name.
     * @return string
     */
    public function get_connectionName() {
        return ($this->connectionName !== '' ? $this->connectionName : $this->params['Name']);
    }

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

    /* Mark: Driver methods */

    /**
     * Return database driver version.
     * @return string
     */
    public function get_driverVersion() {
        return $this->connection->get_driverVersion();
    }

    /**
     * Return database driver architecture.
     * @return string
     */
    public function get_driverArchitecture() {
        return $this->connection->get_driverArchitecture();
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database(?string $name=null) {
        if (!is_null($name)) $this->params['Name'] = $name;
        return $this->connection->select_database($this->params['Name']);
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database(?string $name=null) {
        if (!is_null($name)) $this->params['Name'] = $name;
        return $this->connection->check_database($this->params['Name']);
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $charset
     * @return boolean
     */
    public function create_database(?string $name=null, string $charset='UTF8') {
        if (!is_null($name)) $this->params['Name'] = $name;
        return $this->connection->create_database($this->params['Name'], $charset);
    }

    /**
     * Dump database.
     * @return mixed
     */
    public function dump_database() {
        return $this->connection->dump_database();
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

    /* Mark: Table methods */

    /**
     * Return database table prefix.
     * @return string
     */
    public function get_tablePrefix() {
        return $this->params['TablePrefix'];
    }

    /**
     * Return database table list.
     * @return array
     */
    public function get_tableList() {
        return $this->connection->get_tableList();
    }

    /**
     * Create a table in the database.
     * @param string $name
     * @param array $fields
     * @param array $params
     * @return boolean success
     */
    public function create_table(string $name, array $fields, array $params=array()) {
        return $this->connection->create_table($name, $fields, $params);
    }

    /**
     * Duplicate a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function duplicate_table(array $params=array()) {
        return $this->connection->duplicate_table($params);
    }

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function drop_table(string $name) {
        return $this->connection->drop_table($name);
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table(string $name) {
        return $this->connection->truncate_table($name);
    }

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function alter_table(array $params=array()) {
        return $this->connection->alter_table($params);
    }

    /* Mark: Query methods */

    /**
     * Execute query.
     * @param mixed $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_query(mixed $query, string $resourceName='Default') {
        if (is_string($query))
            return $this->execute_directQuery($query, $resourceName);
        if (is_object($query) and get_class($query) == 'CaMykS2\DbQuery')
            return $this->execute_builtQuery($query, $resourceName);
        return false;
    }

    /**
     * Execute direct query.
     * @param string $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_directQuery(string $query, string $resourceName='Default') {
        return $this->connection->execute_directQuery($query, $resourceName);
    }

    /**
     * Execute prepared query using DbQuery builder.
     * @param Object $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_builtQuery(CaMykS2\DbQuery $query, string $resourceName='Default') {
        return $this->connection->execute_builtQuery($query, $resourceName);
    }

    /**
     * Execute direct queries.
     * @param string $queries
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_queries(string $queries, string $resourceName='Default') {
        return $this->connection->execute_queries($queries, $resourceName);
    }

    /**
     * Record given query from connection library to query list.
     * @param string $query
     * @return void
     */
    public function record_query(string $query) {
        $query = trim($query);
        if ($query != '')
            $this->queries[] = $query;
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId(string $resourceName='Default') {
        return $this->connection->get_queryGeneratedId($resourceName);
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine(string $resourceName='Default') {
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
    public function get_queryNumDataLine(string $resourceName='Default') {
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
    public function get_queryAffectedRows(string $resourceName='Default') {
        $v = $this->connection->get_queryAffectedRows($resourceName);
        $this->statistics['UpdatedLines'] += $v;
        return $v;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default') {
        return $this->connection->get_queryFieldCount($resourceName);
    }

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string
     */
    public function get_queryFieldType(int $fIndex=0, string $resourceName='Default') {
        return $this->connection->get_queryFieldType($fIndex, $resourceName);
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_queryError(string $resourceName='Default') {
        return $this->connection->get_queryError($resourceName);
    }

    /* Mark: Ressource methods */

    /**
     * Close ressource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource(string $resourceName='Default') {
        return $this->connection->close_ressource($resourceName);
    }

    /* Mark: Data methods */

    /**
     * Return escaped string.
     * @param string $string
     * @return string
     */
    public function escape_string(string $string='') {
        return $this->connection->escape_string($string);
    }

    /* Mark: Static methods */

    /**
     * Return available driver list.
     * <br />Method is static to be called without having a working database connection.
     * @return array
     */
    public static function get_availableDrivers() {
        $list = array();

        if (extension_loaded('mysqli'))
            $list['MySQLi'] = 'MySQLi';
        if (extension_loaded('sqlite3'))
            $list['SQLite3'] = 'SQLite';
        if (extension_loaded('pgsql'))
            $list['PgSQL'] = 'PostgreSQL';
        if (extension_loaded('sqlsrv'))
            $list['MSSQL'] = 'MSSQL';

        return $list;
    }

    /**
     * Check if given driver is compatible with database local storage.
     * @param string $driver
     * @return boolean result
     */
    public static function is_driverCompatibleWithDatableLocalStorage(string $driver) {
        if ($driver == 'SQLite3')
            return false;
        return true;
    }
}
?>
