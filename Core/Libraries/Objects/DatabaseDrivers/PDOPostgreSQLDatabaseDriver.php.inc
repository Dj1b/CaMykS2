<?php
/**
 * @brief PDO PostgreSQL database driver.
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
 * PostgreSQLDatabaseDriver class.
 */
final class PostgreSQLDatabaseDriver extends SQLDatabaseDriverInterface {
    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture = 'server';

    /**
     * @var object $connection
     * @brief Internal PDO connection to PostgreSQL server.
     */
    private $connection;

    /**
     * @var array $charsets
     * @brief Return default charsets for encoding values.
     */
    private $charsets = array(
        'UTF8'  => 'UTF8',
        'UTF16' => 'UTF16',
        'UTF32' => 'UTF32',
    );

    /**
     * Open connection.
     * @return boolean success
     */
    public function open() {
        /* Build DSN */
        $dsn = 'pgsql:host=' . $this->get_param('Host');
        if ($this->get_param('Port') > 0)
            $dsn .= ';port=' . $this->get_param('Port');
        if ($this->get_param('Name') !== '')
            $dsn .= ';dbname=' . $this->get_param('Name');

        /* Perform database server connection */
        try {
            $this->connection = new \PDO($dsn, $this->get_param('Login'), $this->get_param('Password'));
            $this->connection->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
            $this->status = true;
        } catch (\PDOException $e) {
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

        /* Close database server connection */
        $this->connection = null;

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
        if (!$this->is_active())
            return '';

        /* Return PostgreSQL version */
        return $this->connection->query('SELECT version()')->fetchColumn();
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database() {
        /* Open connection */
        return $this->open();
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database() {
        if (!$this->execute_directQuery("SELECT 1 FROM pg_database WHERE datname='".$this->escape_string($this->get_param('Name'))."';"))
            return false;
        return $this->get_queryDataLine() !== false;
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $encoding
     * @return boolean
     */
    public function create_database($encoding='UTF8') {
        return $this->execute_directQuery('CREATE DATABASE "'.$this->get_param('Name').'" ENCODING \''.$this->charsets[$encoding].'\';');
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

            /* Get table creation SQL (simplified - PostgreSQL doesn't have SHOW CREATE TABLE) */
            $content .= "-- Table structure for ".$table."\n\n";

            /* Build table content */
            if ($this->execute_directQuery('SELECT * FROM "'.$table.'";', 'tableItemList')) {
                while ($insertData = $this->get_queryNumDataLine('tableItemList')) {
                    $content .= "INSERT INTO \"".$table."\" VALUES(";
                    for ($i=0; $i < count($insertData); $i++) {
                        if ($i != 0)
                            $content .=  ", ";
                        if (is_string($insertData[$i]))
                            $content .=  "'" . addslashes($insertData[$i]) . "'";
                        elseif (is_null($insertData[$i]))
                            $content .= 'NULL';
                        else
                            $content .= $insertData[$i];
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
        /* Initialise size */
        $size = 0;

        /* Load information from database */
        if ($this->execute_directQuery("SELECT pg_database_size('".$this->escape_string($this->get_param('Name'))."') as size;")) {
            if ($data = $this->get_queryDataLine()) {
                $size = $data['size'];
            }
        }
        return $size;
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
        $sql = "SELECT tablename FROM pg_tables WHERE schemaname='public';";
        if ($this->execute_directQuery($sql))
            while ($data = $this->get_queryDataLine())
                $list[] = $data['tablename'];

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
        global $camyks;

        /* Merge params with default ones */
        $params = array_merge(array(
            'Charset'       => '',
            'AutoId'        => true,
            'PrimaryKeys'   => array('id'),
            'Indexes'       => array(),
        ), $params);

        /* Build sql query */
        $sql = 'CREATE TABLE IF NOT EXISTS "'.$name.'" (';

        /* Add auto id */
        if ($params['AutoId'])
            $sql .= 'id BIGSERIAL,';

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
            $s = ' "'.$field.'" ';

            /* Add field description from type */
            switch ($data['Type']) {
                case 'text':
                    if (is_int($data['Length'])) {
                        if ($data['Length'] < 256)
                            $s .= 'VARCHAR('.$data['Length'].') DEFAULT \''.addslashes($data['Default']).'\'';
                        else
                            $s .= 'TEXT';
                    } elseif (is_string($data['Length'])) {
                        $data['Length'] = strtolower($data['Length']);
                        if ($data['Length'] === 'small')
                            $s .= 'VARCHAR(255) DEFAULT \''.addslashes($data['Default']).'\'';
                        else
                            $s .= 'TEXT';
                    } else {
                        $s .= 'TEXT';
                    }
                    break;
                case 'tinytext':
                case 'mediumtext':
                case 'longtext':
                    $s .= 'TEXT';
                    break;
                case 'integer':
                    if (!is_null($data['Size'])) {
                        $data['Size'] = strtolower($data['Size']);
                        if ($data['Size'] === 'tiny' || $data['Size'] === 'small')
                            $s .= 'SMALLINT';
                        elseif ($data['Size'] === 'medium')
                            $s .= 'INTEGER';
                        elseif ($data['Size'] === 'big')
                            $s .= 'BIGINT';
                        else
                            $s .= 'INTEGER';
                    } else {
                        if ($data['Max'] <= 32767)
                            $s .= 'SMALLINT';
                        elseif ($data['Max'] <= 2147483647)
                            $s .= 'INTEGER';
                        else
                            $s .= 'BIGINT';
                    }
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(int)$data['Default'];
                    break;
                case 'float':
                    $s .= 'DOUBLE PRECISION';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(float)$data['Default'];
                    break;
                case 'price':
                    $s .= 'NUMERIC(14,2)';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(float)$data['Default'];
                    break;
                case 'date':
                    $s .= 'DATE DEFAULT CURRENT_DATE';
                    break;
                case 'datetime':
                    $s .= 'TIMESTAMP DEFAULT CURRENT_TIMESTAMP';
                    break;
                case 'time':
                    $s .= 'TIME DEFAULT CURRENT_TIME';
                    break;
                default:
                    /* Type not known */
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

        /* Add primary keys */
        if (count($params['PrimaryKeys']) > 0)
            $sql .= ' PRIMARY KEY ("'.implode('","', $params['PrimaryKeys']).'"),';

        /* Remove trailing comma */
        $sql = rtrim($sql, ',');

        /* Close fields information */
        $sql .= ');';

        /* Execute table creation */
        $result = $this->execute_directQuery($sql);

        /* Create indexes separately */
        foreach ($params['Indexes'] as $name => $indexes) {
            if (is_string($indexes)) {
                $indexSql = 'CREATE INDEX IF NOT EXISTS idx_'.$name.'_'.$indexes.' ON "'.$name.'" ("'.$indexes.'");';
                $this->execute_directQuery($indexSql);
            } elseif (is_array($indexes)) {
                $indexSql = 'CREATE INDEX IF NOT EXISTS idx_'.$name.' ON "'.$name.'" ("'.implode('","', $indexes).'");';
                $this->execute_directQuery($indexSql);
            }
        }

        /* Return result */
        return $result;
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
        $sql = 'DROP TABLE IF EXISTS "'.$name.'";';
        return $this->execute_directQuery($sql);
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        $sql = 'TRUNCATE TABLE "'.$name.'";';
        return $this->execute_directQuery($sql);
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
        try {
            $this->queryResources[$resourceName] = $this->connection->query($query);
        } catch (\PDOException $e) {
            return false;
        }
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
        try {
            $this->queryResources[$resourceName] = $this->connection->exec($queries);
        } catch (\PDOException $e) {
            return false;
        }
        return ($this->queryResources[$resourceName] !== false);
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId(string $resourceName='Default') {
        if ($this->is_active())
            return $this->connection->lastInsertId();
        return false;
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->fetch(\PDO::FETCH_ASSOC);
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->fetch(\PDO::FETCH_NUM);
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->rowCount();
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->columnCount();
        return false;
    }

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string
     */
    public function get_queryFieldType($fIndex=0, $resourceName='Default') {
        if (isset($this->queryResources[$resourceName])) {
            $meta = $this->queryResources[$resourceName]->getColumnMeta($fIndex);
            return $this->get_compatibleType($meta['pdo_type'] ?? \PDO::PARAM_STR);
        }
        return false;
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_queryError(string $resourceName='Default') {
        if ($this->is_active()) {
            $error = $this->connection->errorInfo();
            return $error[2] ?? '';
        }
        return '';
    }

    /* Mark: Resource methods */

    /**
     * Close resource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName])) {
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
        /* PDO quote returns quoted string, so we need to remove quotes */
        $quoted = $this->connection->quote($string);
        return substr($quoted, 1, -1);
    }

    /* Mark: Private tool methods */

    /**
     * Return PostgreSQL compatible type from given PDO type.
     * @param integer $type
     * @return string
     */
    private function get_compatibleType($type=\PDO::PARAM_STR) {
        switch ($type) {
            case \PDO::PARAM_INT:
                return 'int';
            case \PDO::PARAM_BOOL:
                return 'bool';
            case \PDO::PARAM_NULL:
                return 'null';
            case \PDO::PARAM_STR:
            default:
                return 'string';
        }
    }
}
?>
