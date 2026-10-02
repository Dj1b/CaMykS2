<?php
/**
 * @brief Interface for CaMykS database drivers.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre7
 * @date Creation: Oct 2019
 * @date Modification: Oct 2026
 * @copyright 2019 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * DatabaseDriverInterface class.
 */
abstract class DatabaseDriverInterface {
    /**
     * @var DatabaseConnector $parent
     * @brief Link to parent DatabaseConnector object.
     */
    protected DatabaseConnector $parent;

    /**
     * @var boolean $status
     * @brief Status of the database connection.
     */
    protected bool $status = false;

    /**
     * @var array $queryResources
     * @brief List of open resources.
     */
    protected array $queryResources = array();

    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected string $architecture;

    /**
     * Class constructor.
     * @param DatabaseConnector $parent
     * @return void
     */
    public function __construct(DatabaseConnector &$parent) {
        $this->parent = $parent;
    }

    /* Mark: Parent methods */

    /**
     * Return parameter value.
     * @param string $param
     * @return mixed
     */
    protected function get_param(string $param): mixed {
        return $this->parent->get_param($param);
    }

    /**
     * Update parameter value.
     * @param string $param
     * @param mixed $value
     * @return mixed
     */
    protected function update_param(string $param, mixed $value): mixed {
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
    protected function add_error(int $errorId, string $query, string $file='', int $line=0): false {
        return $this->parent->add_error($errorId, $query, $file, $line);
    }

    /* Mark: Connection methods */

    /**
     * Open connection.
     * @return boolean success
     */
    abstract public function open(): bool;

    /**
     * Close connection.
     * @return boolean success
     */
    abstract public function close(): bool;

    /* Mark: Driver methods */

    /**
     * Return database driver version.
     * @return string
     */
    abstract public function get_driverVersion(): string;

    /**
     * Return database driver architecture.
     * @return string
     */
    public function get_driverArchitecture(): string {
        return $this->architecture;
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @return boolean success
     */
    abstract public function select_database(): bool;

    /**
     * Check database exists.
     * @return boolean success
     */
    abstract public function check_database(): bool;

    /**
     * Create a database.
     * @param string $charset
     * @return boolean
     */
    abstract public function create_database(string $charset='UTF8'): bool;

    /**
     * Dump database.
     * @return mixed
     */
    abstract public function dump_database(): mixed;

    /**
     * Return database size.
     * @return integer|false
     */
    abstract public function get_databaseSize(): int|false;

    /**
     * Return database summary.
     * @return array
     */
    abstract public function get_databaseSummary(): array;

    /* Mark: Table methods */

    /**
     * Return database table list.
     * @return array|false
     */
    abstract public function get_tableList(): array|false;

    /**
     * Create a table in the database.
     * @param string $name
     * @param array $fields
     * @param array $params
     * @return boolean success
     */
    abstract public function create_table(string $name, array $fields, array $params=array()): bool;

    /**
     * Duplicate a table in the database.
     * @param array $params
     * @return boolean success
     */
    abstract public function duplicate_table(array $params=array()): bool;

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    abstract public function drop_table(string $name): bool;

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    abstract public function truncate_table(string $name): bool;

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    abstract public function alter_table(array $params=array()): bool;

    /* Mark: Query methods */

    /**
     * Execute direct query.
     * @param string $query
     * @param string $resourceName
     * @return mixed
     */
    abstract public function execute_directQuery(string $query, string $resourceName='Default'): mixed;

    /**
     * Execute prepared query using DbQuery builder.
     * @param DbQuery $query
     * @param string $resourceName
     * @return mixed
     */
    abstract public function execute_builtQuery(DbQuery $query, string $resourceName='Default'): mixed;

    /**
     * Execute direct queries.
     * @param string $queries
     * @param string $resourceName
     * @return mixed
     */
    abstract public function execute_queries(string $queries, string $resourceName='Default'): mixed;

    /**
     * Record given query from connection library to query list
     * @param string $query
     * @return void
     */
    protected function record_query(string $query): void {
        $this->parent->record_query($query);
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return mixed
     */
    abstract public function get_queryGeneratedId(string $resourceName='Default'): mixed;

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array|false
     */
    abstract public function get_queryDataLine(string $resourceName='Default'): array|null|false;

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array|false
     */
    abstract public function get_queryNumDataLine(string $resourceName='Default'): array|null|false;

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return int|false
     */
    abstract public function get_queryAffectedRows(string $resourceName='Default'): int|false;

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return int|false
     */
    abstract public function get_queryFieldCount(string $resourceName='Default'): int|false;

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string|false
     */
    abstract public function get_queryFieldType(int $fIndex=0, string $resourceName='Default'): string|false;

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    abstract public function get_queryError(string $resourceName='Default'): string;

    /* Mark: Ressource methods */

    /**
     * Close ressource.
     * @param string $resourceName
     * @return boolean result
     */
    abstract public function close_ressource(string $resourceName='Default'): bool;

    /* Mark: Data methods */

    /**
     * Return escaped string.
     * @param string $string
     * @return string
     */
    abstract public function escape_string(string $string=''): string;

    /* Mark: Check methods */

    /**
     * Check connection is active.
     * @return boolean result
     */
    public function is_active(): bool {
        return $this->status;
    }
}
?>
