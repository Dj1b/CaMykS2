<?php
/**
 * @brief SQLite database driver.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2026
 * @date Modification: Feb 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @note Caution: Untested library.
 */

namespace CaMykS2;

/**
 * SQLiteDatabaseDriver class.
 */
final class SQLiteDatabaseDriver extends SQLDatabaseDriverInterface {
    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture = 'file';

    /**
     * @var \SQLite3 $connection
     * @brief Internal connection to SQLite database.
     */
    private $connection;

    /**
     * @var string $dbPath
     * @brief Database file path.
     */
    private $dbPath;

    /**
     * @var string $dbExtension
     * @brief Database file extension.
     */
    private $dbExtension = '.sqlite3.inc';

    /**
     * Class constructor.
     * @param Object $parent
     * @return void
     */
    public function __construct(&$parent) {
        global $CaMykS;
        parent::__construct($parent);

        /* Loading database path */
        $this->dbPath = $CaMykS->get_websiteDataFileFullPath('Databases');
    }

    /**
     * Open connection.
     * @return boolean success
     */
    public function open() {
        /* Build database full path */
        $path = $this->get_fullDatabaseName($this->get_param('Name'));

        /* Perform database file connection */
        try {
            $this->connection = new \SQLite3($path);

            /* Check connection */
            if ($this->connection->lastErrorCode() !== 0) {
                 $this->status = false;
                 return false;
            }

            /* Set encryption key if provided (requires specific SQLite3 libraries) */
            if ($this->get_param('Password') !== '') {
                /* Note: Standard PHP SQLite3 doesn't support PRAGMA key easily,
                 * but some libraries do. We'll try it just in case.
                 */
                $this->connection->exec("PRAGMA key = '".$this->escape_string($this->get_param('Password'))."';");
            }

            $this->status = true;
        } catch (\Exception $e) {
            $this->status = false;
        }

        /* Return result */
        return $this->is_active();
    }

    /**
     * Close connection.
     * @return boolean success
     */
    public function close() {
        /* Check connection is open */
        if (!$this->is_active())
            return true;

        /* Close database connection */
        if (!$this->connection->close())
            return false;

        /* Update status */
        $this->status = false;

        /* Return success */
        return true;
    }

    /* Mark: Driver methods */

    /**
     * Return database driver version.
     * @return string
     */
    public function get_driverVersion() {
        /* Check connection is open */
        if (!$this->is_active()) {
            $v = \SQLite3::version();
            return $v['versionString'];
        }

        /* Return SQLite version from connection */
        return $this->connection->querySingle('SELECT sqlite_version()');
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database($name=null) {
        /* SQLite uses file-based databases, changing name means reopening */
        if (is_null($name) or $name == $this->get_param('Name'))
            return true;

        /* Close current connection */
        $this->close();

        /* Update name */
        $this->set_param('Name', $name);

        /* Open new connection */
        return $this->open();
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database($name=null) {
        return file_exists($this->get_fullDatabaseName($name ?? $this->get_param('Name')));
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $encoding
     * @return boolean
     */
    public function create_database($name, $encoding='UTF8') {
        /* SQLite creates database file on connection */
        /* Just check if we can create the file */
        $path = $this->get_fullDatabaseName($name);
        if (file_exists($path))
            return true;

        return touch($path);
    }

    /**
     * Dump database.
     * @return mixed
     */
    public function dump_database() {
        $content = '';

        /* Build header comments */
        $content = "-- ************************\n";
        $content .= "-- Database \"".$this->get_param('Name')."\" dump\n";
        $content .= "-- ".date('r')."\n";
        $content .= "-- ************************\n\n";

        /* Load table list */
        $tables = $this->get_tableList();

        /* Build tables to content */
        foreach ($tables as $table) {
            /* Build table comments */
            $content .= "-- ************************\n";
            $content .= "-- Table \"".$table."\"\n";
            $content .= "-- ************************\n";

            /* Get table creation SQL */
            $sql = "SELECT sql FROM sqlite_master WHERE type='table' AND name='".$this->escape_string($table)."';";
            $createTable = $this->connection->querySingle($sql, true);
            if ($createTable and isset($createTable['sql'])) {
                $content .= $createTable['sql'].";\n\n";
            }

            /* Build table content */
            $res = $this->connection->query('SELECT * FROM `'.$table.'`;');
            if ($res) {
                while ($insertData = $res->fetchArray(SQLITE3_NUM)) {
                    $content .= "INSERT INTO `".$table."` VALUES(";
                    for ($i=0; $i < count($insertData); $i++) {
                        if ($i != 0)
                            $content .=  ", ";

                        if (is_null($insertData[$i])) {
                            $content .= 'NULL';
                        } elseif (is_string($insertData[$i])) {
                            $content .=  "'" . addslashes($insertData[$i]) . "'";
                        } else {
                            $content .= $insertData[$i];
                        }
                    }
                    $content .=  ");\n";
                }
            }
            $content .= "\n\n";
        }

        /* Return built content */
        return $content;
    }

    /**
     * Return database size.
     * @return integer
     */
    public function get_databaseSize() {
        $path = $this->get_fullDatabaseName($this->get_param('Name'));
        if (file_exists($path))
            return filesize($path);
        return 0;
    }

    /**
     * Return database summary.
     * @return array
     */
    public function get_databaseSummary() {
        // To be implemented
    }

    /* Mark: Table methods */

    /**
     * Return database table list.
     * @return array
     */
    public function get_tableList() {
        /* Initialise table list */
        $list = array();

        /* Load list from database */
        $sql = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
        $res = $this->connection->query($sql);
        if ($res) {
            while ($data = $res->fetchArray(SQLITE3_ASSOC))
                $list[] = $data['name'];
        }

        /* Return result */
        return $list;
    }

    /**
     * Create a table in the database.
     * @param string $name
     * @param array $fields
     * @param array $params
     * @return boolean success
     */
    public function create_table($name, $fields=array(), $params=array()) {
        /* Merge params with default ones */
        $params = array_merge(array(
            'Charset'       => '',
            'AutoId'        => true,
            'PrimaryKeys'   => array('id'),
            'Indexes'       => array(),
        ), $params);

        /* Build sql query */
        $sql = 'CREATE TABLE IF NOT EXISTS `'.$name.'` (';

        /* Add auto id */
        if ($params['AutoId'])
            $sql .= '`id` INTEGER PRIMARY KEY AUTOINCREMENT,';

        /* Add fields description */
        foreach ($fields as $field => $data) {

            /* Merge field settings */
            $data = array_merge(array(
                /* Generic values */
                'Type'      => '',  /* text, integer, float, date, datetime, time */
                'Default'   => '',
                'NotNull'   => false,
                'IsUnique'  => false,

                /* Text settings */
                'Length'    => '',

                /* Numeric settings */
                'Min'       => 0,
                'Max'       => 4294967296,
                'Size'      => null,
                'Unsigned'  => false,

                /* Primary key and index settings */
                'PrimaryKey'    => false,
                'IndexName'     => false,
                'IndexLength'   => null,
            ), $data);

            /* Add field name */
            $s = ' `'.$field.'` ';

            /* Add field description from type */
            switch ($data['Type']) {
                case 'text':
                case 'tinytext':
                case 'mediumtext':
                case 'longtext':
                case 'blob':
                case 'date':
                case 'datetime':
                case 'time':
                    $s .= 'TEXT';
                    if ($data['Default'] !== '')
                        $s .= ' DEFAULT "'.addslashes($data['Default']).'"';
                    break;
                case 'integer':
                    $s .= 'INTEGER';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(int)$data['Default'];
                    break;
                case 'float':
                case 'price':
                    $s .= 'REAL';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(float)$data['Default'];
                    break;
                default:
                    $s .= 'TEXT';
            }

            /* Check for not null information */
            if ($data['NotNull'])
                $s .= ' NOT NULL';

            /* Check for is unique information */
            if ($data['IsUnique'])
                $s .= ' UNIQUE';

            /* Add separator between fields */
            $sql .= $s.',';

            /* Check for index */
            if ($data['IndexName'] === true) {
                $params['Indexes'][] = $field;
            } elseif ($data['IndexName'] !== false) {
                if (!isset($params['Indexes'][$data['IndexName']]))
                    $params['Indexes'][$data['IndexName']] = array();
                $params['Indexes'][$data['IndexName']][] = $field;
            }

            /* Check for primary key */
            if ($data['PrimaryKey'] === true)
                $params['PrimaryKeys'][] = $field;
        }

        /* Remove trailing comma */
        $sql = rtrim($sql, ',');

        /* Close fields information */
        $sql .= ');';

        /* Execute table creation */
        $result = $this->connection->exec($sql);
        if (!$result) return false;

        /* Create indexes separately in SQLite */
        foreach ($params['Indexes'] as $iname => $indexes) {
            if (is_string($indexes)) {
                $indexSql = 'CREATE INDEX IF NOT EXISTS `idx_'.$name.'_'.$iname.'` ON `'.$name.'` (`'.$indexes.'`);';
                $this->connection->exec($indexSql);
            } elseif (is_array($indexes)) {
                $indexSql = 'CREATE INDEX IF NOT EXISTS `idx_'.$name.'_'.$iname.'` ON `'.$name.'` (`'.implode('`,`', $indexes).'`);';
                $this->connection->exec($indexSql);
            }
        }

        /* Return result */
        return true;
    }

    /**
     * Duplicate a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function duplicate_table($params=array()) {
        // To be implemented
    }

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function drop_table($name) {
        return $this->connection->exec('DROP TABLE IF EXISTS `'.$name.'`;');
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        return $this->connection->exec('DELETE FROM `'.$name.'`;');
    }

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function alter_table($params=array()) {
        // To be implemented
    }

    /* Mark: Query methods */

    /**
     * Execute direct query.
     * @param string $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_directQuery($query, $resourceName='Default') {
        $this->record_query($query);
        $this->queryResources[$resourceName] = $this->connection->query($query);
        return ($this->queryResources[$resourceName] !== false);
    }

    /**
     * Execute direct queries.
     * @param string $queries
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_queries($queries, $resourceName='Default') {
        $this->record_query($queries);
        return $this->connection->exec($queries);
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId(string $resourceName='Default') {
        if ($this->is_active())
            return $this->connection->lastInsertRowID();
        return false;
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return $this->queryResources[$resourceName]->fetchArray(SQLITE3_ASSOC);
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return $this->queryResources[$resourceName]->fetchArray(SQLITE3_NUM);
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows(string $resourceName='Default') {
        if ($this->is_active())
            return $this->connection->changes();
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return $this->queryResources[$resourceName]->numColumns();
        return false;
    }

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string
     */
    public function get_queryFieldType($fIndex=0, $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            $type = $this->queryResources[$resourceName]->columnType($fIndex);
            switch ($type) {
                case SQLITE3_INTEGER: return 'int';
                case SQLITE3_FLOAT: return 'float';
                case SQLITE3_BLOB: return 'blob';
                case SQLITE3_TEXT:
                default:
                    return 'string';
            }
        }
        return false;
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_queryError(string $resourceName='Default') {
        if ($this->is_active())
            return $this->connection->lastErrorMsg();
        return '';
    }

    /* Mark: Resource methods */

    /**
     * Close resource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            $this->queryResources[$resourceName]->finalize();
            $this->queryResources[$resourceName] = null;
            unset($this->queryResources[$resourceName]);
        }
        return true;
    }

    /* Mark: Data methods */

    /**
     * Return escaped string.
     * @param string $string
     * @return string
     */
    public function escape_string($string='') {
        return \SQLite3::escapeString($string);
    }

    /* Mark: Private tool methods */

    /**
     * Return full database path.
     * @param string $dbName
     * @return string
     */
    private function get_fullDatabaseName($dbName) {
        /* Return database path */
        return $this->dbPath.'/'.$dbName.$this->dbExtension;
    }
}
?>
