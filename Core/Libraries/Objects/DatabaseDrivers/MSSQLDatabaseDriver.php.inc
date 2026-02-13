<?php
/**
 * @brief MSSQL database driver.
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
 * MSSQLDatabaseDriver class.
 */
final class MSSQLDatabaseDriver extends SQLDatabaseDriverInterface {
    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture = 'server';

    /**
     * @var resource $connection
     * @brief Internal connection to MSSQL server.
     */
    private $connection;

    /**
     * Open connection.
     * @return boolean success
     */
    public function open() {
        /* Build connection settings */
        $server = $this->get_param('Host');
        if ($this->get_param('Port') > 0)
            $server .= ',' . $this->get_param('Port');

        $options = array(
            'UID' => $this->get_param('Login'),
            'PWD' => $this->get_param('Password'),
            'Database' => $this->get_param('Name'),
            'CharacterSet' => 'UTF-8',
        );

        /* Perform database server connection */
        try {
            $this->connection = sqlsrv_connect($server, $options);
            $this->status = ($this->connection !== false);
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

        /* Close database server connection */
        if (!sqlsrv_close($this->connection))
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
        if (!$this->is_active())
            return '';

        /* Return MSSQL version */
        $info = sqlsrv_server_info($this->connection);
        return $info['SQLServerVersion'] ?? '';
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database($name=null) {
        if (is_null($name)) $name = $this->get_param('Name');
        $sql = 'USE ['.$name.'];';
        return $this->execute_directQuery($sql);
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database($name=null) {
        $sql = "SELECT 1 FROM sys.databases WHERE name='".$this->escape_string($name ?? $this->get_param('Name'))."';";
        if (!$this->execute_directQuery($sql, 'checkDb'))
            return false;
        return $this->get_queryDataLine('checkDb') !== false;
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $encoding
     * @return boolean
     */
    public function create_database($name, $encoding='UTF8') {
        $sql = 'CREATE DATABASE ['.$name.'];';
        return $this->execute_directQuery($sql);
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

            /* Build table content */
            if ($this->execute_directQuery('SELECT * FROM ['.$table.'];', 'tableItemList')) {
                while ($insertData = $this->get_queryNumDataLine('tableItemList')) {
                    $content .= "INSERT INTO [".$table."] VALUES(";
                    for ($i=0; $i < count($insertData); $i++) {
                        if ($i != 0)
                            $content .=  ", ";

                        if (is_null($insertData[$i])) {
                            $content .= 'NULL';
                        } elseif (is_string($insertData[$i])) {
                            $content .=  "'" . $this->escape_string($insertData[$i]) . "'";
                        } else {
                            $content .= addslashes($insertData[$i]);
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
        /* Simplified for MSSQL native */
        $sql = "SELECT SUM(size) * 8 * 1024 FROM sys.master_files WHERE database_id = DB_ID('".$this->escape_string($this->get_param('Name'))."');";
        if ($this->execute_directQuery($sql, 'dbSize')) {
            if ($data = $this->get_queryNumDataLine('dbSize')) {
                return (int)$data[0];
            }
        }
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
        $sql = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE';";
        if ($this->execute_directQuery($sql))
            while ($data = $this->get_queryDataLine())
                $list[] = $data['TABLE_NAME'];

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
        $sql = 'CREATE TABLE ['.$name.'] (';

        /* Add auto id */
        if ($params['AutoId'])
            $sql .= '[id] BIGINT IDENTITY(1,1) NOT NULL,';

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
            $s = ' ['.$field.'] ';

            /* Add field description from type */
            switch ($data['Type']) {
                case 'text':
                    if (is_int($data['Length'])) {
                        if ($data['Length'] < 256)
                            $s .= 'NVARCHAR('.$data['Length'].')';
                        elseif ($data['Length'] < 4000)
                            $s .= 'NVARCHAR('.$data['Length'].')';
                        else
                            $s .= 'NVARCHAR(MAX)';
                    } else {
                        $s .= 'NVARCHAR(MAX)';
                    }
                    if ($data['Default'] !== '')
                        $s .= ' DEFAULT \''.addslashes($data['Default']).'\'';
                    break;
                case 'tinytext':
                    $s .= 'NVARCHAR(255)';
                    break;
                case 'mediumtext':
                case 'longtext':
                    $s .= 'NVARCHAR(MAX)';
                    break;
                case 'integer':
                    if (!is_null($data['Size'])) {
                        $data['Size'] = strtolower($data['Size']);
                        if ($data['Size'] === 'tiny')
                            $s .= 'TINYINT';
                        elseif ($data['Size'] === 'small')
                            $s .= 'SMALLINT';
                        elseif ($data['Size'] === 'medium')
                            $s .= 'INT';
                        elseif ($data['Size'] === 'big')
                            $s .= 'BIGINT';
                        else
                            $s .= 'INT';
                    } else {
                        if ($data['Max'] <= 255)
                            $s .= 'TINYINT';
                        elseif ($data['Max'] <= 32767)
                            $s .= 'SMALLINT';
                        elseif ($data['Max'] <= 2147483647)
                            $s .= 'INT';
                        else
                            $s .= 'BIGINT';
                    }
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(int)$data['Default'];
                    break;
                case 'float':
                    $s .= 'FLOAT';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(float)$data['Default'];
                    break;
                case 'price':
                    $s .= 'DECIMAL(14,2)';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(float)$data['Default'];
                    break;
                case 'date':
                    $s .= 'DATE';
                    break;
                case 'datetime':
                    $s .= 'DATETIME2';
                    break;
                case 'time':
                    $s .= 'TIME';
                    break;
                case 'blob':
                    $s .= 'VARBINARY(MAX)';
                    break;
                default:
                    $s .= 'NVARCHAR(MAX)';
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
            $sql .= ' PRIMARY KEY (['.implode('],[', $params['PrimaryKeys']).']),';

        /* Remove trailing comma */
        $sql = rtrim($sql, ',');

        /* Close fields information */
        $sql .= ');';

        /* Execute table creation */
        if (!$this->execute_directQuery($sql))
            return false;

        /* Create indexes separately */
        foreach ($params['Indexes'] as $iname => $indexes) {
            if (is_string($indexes)) {
                $indexSql = 'CREATE INDEX [idx_'.$name.'_'.$iname.'] ON ['.$name.'] (['.$indexes.']);';
                $this->execute_directQuery($indexSql);
            } elseif (is_array($indexes)) {
                $indexSql = 'CREATE INDEX [idx_'.$name.'_'.$iname.'] ON ['.$name.'] (['.implode('],[', $indexes).']);';
                $this->execute_directQuery($indexSql);
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
        return $this->execute_directQuery('DROP TABLE IF EXISTS ['.$name.'];');
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        return $this->execute_directQuery('TRUNCATE TABLE ['.$name.'];');
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
        $this->queryResources[$resourceName] = sqlsrv_query($this->connection, $query);
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
        $this->queryResources[$resourceName] = sqlsrv_query($this->connection, $queries);
        return ($this->queryResources[$resourceName] !== false);
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId(string $resourceName='Default') {
        if (!$this->is_active()) return false;
        $res = sqlsrv_query($this->connection, "SELECT SCOPE_IDENTITY();");
        if ($res) {
            $data = sqlsrv_fetch_array($res, SQLSRV_FETCH_NUMERIC);
            return (int)$data[0];
        }
        return false;
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return sqlsrv_fetch_array($this->queryResources[$resourceName], SQLSRV_FETCH_ASSOC);
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return sqlsrv_fetch_array($this->queryResources[$resourceName], SQLSRV_FETCH_NUMERIC);
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return sqlsrv_rows_affected($this->queryResources[$resourceName]);
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return sqlsrv_num_fields($this->queryResources[$resourceName]);
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
            $meta = sqlsrv_field_metadata($this->queryResources[$resourceName]);
            if (isset($meta[$fIndex])) {
                $type = $meta[$fIndex]['Type'];
                /* Simplified mapping */
                switch ($type) {
                    case -5: // bigint
                    case 4:  // int
                    case 5:  // smallint
                    case -6: // tinyint
                        return 'int';
                    case 6:  // float
                    case 7:  // real
                    case 3:  // decimal
                        return 'float';
                    case -2: // binary
                    case -3: // varbinary
                    case -4: // image
                        return 'blob';
                    default:
                        return 'string';
                }
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
        $errors = sqlsrv_errors(SQLSRV_ERR_ALL);
        if ($errors) {
            return $errors[0]['message'];
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
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            sqlsrv_free_stmt($this->queryResources[$resourceName]);
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
        /* sqlsrv doesn't have a native escape function, use str_replace for single quotes */
        return str_replace("'", "''", $string);
    }
}
?>
