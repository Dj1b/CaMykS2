<?php
/**
 * @brief PostgreSQL database driver.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Feb 2026
 * @date Modification: Oct 2026
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
    protected string $architecture = 'server';

    /**
     * @var resource $connection
     * @brief Internal connection to PostgreSQL server.
     */
    private mixed $connection = null;

    /**
     * @var array $charsets
     * @brief Return default charsets for encoding values.
     */
    private array $charsets = array(
        'UTF8'  => 'UTF8',
        'UTF16' => 'UTF16',
        'UTF32' => 'UTF32',
    );

    /**
     * Open connection.
     * @return boolean success
     */
    public function open(): bool {
        /* Build connection string */
        $string = 'host=' . $this->get_param('Host');
        if ($this->get_param('Port') > 0)
            $string .= ' port=' . $this->get_param('Port');
        if ($this->get_param('Name') !== '')
            $string .= ' dbname=' . $this->get_param('Name');
        if ($this->get_param('Login') !== '')
            $string .= ' user=' . $this->get_param('Login');
        if ($this->get_param('Password') !== '')
            $string .= ' password=' . $this->get_param('Password');

        /* Perform database server connection */
        try {
            $this->connection = pg_connect($string);
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
    public function close(): bool {
        /* Check connection is open */
        if (!$this->is_active())
            return true;

        /* Close database server connection */
        if (!pg_close($this->connection))
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
    public function get_driverVersion(): string {
        /* Check connection is open */
        if (!$this->is_active())
            return '';

        /* Return PostgreSQL version */
        $v = pg_version($this->connection);
        return $v['server'] ?? '';
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database($name=null): bool {
        /* PostgreSQL requires reconnection to change database */
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
    public function check_database($name=null): bool {
        $sql = "SELECT 1 FROM pg_database WHERE datname='".$this->escape_string($name ?? $this->get_param('Name'))."';";
        if (!$this->execute_directQuery($sql, 'checkDb'))
            return false;
        return $this->get_queryDataLine('checkDb') !== false;
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $charset
     * @return boolean
     */
    public function create_database(string $name='', string $charset='UTF8'): bool {
        $dbName = $name !== '' ? $name : $this->get_param('Name');
        $charsetVal = isset($this->charsets[$charset]) ? $this->charsets[$charset] : 'UTF8';
        $sql = 'CREATE DATABASE "'.$dbName.'" ENCODING \''.$charsetVal.'\';';
        return $this->execute_directQuery($sql);
    }

    /**
     * Dump database.
     * @return mixed
     */
    public function dump_database(): mixed {
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

            /* Get table creation (simplified for PgSQL native) */
            $content .= "-- structure not available in simplified native dump\n\n";

            /* Build table content */
            if ($this->execute_directQuery('SELECT * FROM "'.$table.'";', 'tableItemList')) {
                while ($insertData = $this->get_queryNumDataLine('tableItemList')) {
                    $content .= "INSERT INTO \"".$table."\" VALUES(";
                    for ($i=0; $i < count($insertData); $i++) {
                        if ($i != 0)
                            $content .=  ", ";
                        
                        if (is_null($insertData[$i])) {
                            $content .= 'NULL';
                        } elseif (is_string($insertData[$i])) {
                            $content .=  "'" . pg_escape_string($this->connection, $insertData[$i]) . "'";
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
    public function get_databaseSize(): int|false {
        /* Initialise size */
        $size = 0;

        /* Load information from database */
        $sql = "SELECT pg_database_size('".$this->escape_string($this->get_param('Name'))."');";
        if ($this->execute_directQuery($sql, 'dbSize')) {
            if ($data = $this->get_queryNumDataLine('dbSize')) {
                $size = (int)$data[0];
            }
        }
        return $size;
    }

    /**
     * Return database summary.
     * @return array
     */
    public function get_databaseSummary(): array {
        // To be implemented
    }

    /* Mark: Table methods */

    /**
     * Return database table list.
     * @return array
     */
    public function get_tableList(): array|false {
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
    public function create_table(string $name, array $fields, array $params=array()): bool {
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
            $sql .= '"id" BIGSERIAL,';

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
                            $s .= 'VARCHAR('.$data['Length'].')';
                        else
                            $s .= 'TEXT';
                    } else {
                        $s .= 'TEXT';
                    }
                    if ($data['Default'] !== '')
                        $s .= ' DEFAULT \''.addslashes($data['Default']).'\'';
                    break;
                case 'tinytext':
                case 'mediumtext':
                case 'longtext':
                case 'blob':
                    $s .= 'BYTEA';
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
        if (!$this->execute_directQuery($sql))
            return false;

        /* Create indexes separately */
        foreach ($params['Indexes'] as $iname => $indexes) {
            if (is_string($indexes)) {
                $indexSql = 'CREATE INDEX IF NOT EXISTS "idx_'.$name.'_'.$iname.'" ON "'.$name.'" ("'.$indexes.'");';
                $this->execute_directQuery($indexSql);
            } elseif (is_array($indexes)) {
                $indexSql = 'CREATE INDEX IF NOT EXISTS "idx_'.$name.'_'.$iname.'" ON "'.$name.'" ("'.implode('","', $indexes).'");';
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
    public function duplicate_table(array $params=array()): bool {
        // To be implemented
    }

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function drop_table(string $name): bool {
        return $this->execute_directQuery('DROP TABLE IF EXISTS "'.$name.'";');
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table(string $name): bool {
        return $this->execute_directQuery('TRUNCATE TABLE "'.$name.'";');
    }

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function alter_table(array $params=array()): bool {
        // To be implemented
    }

    /* Mark: Query methods */

    /**
     * Execute direct query.
     * @param string $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_directQuery(string $query, string $resourceName='Default'): mixed {
        $this->record_query($query);
        $this->queryResources[$resourceName] = pg_query($this->connection, $query);
        return ($this->queryResources[$resourceName] !== false);
    }

    /**
     * Return replace query using DbQuery builder (PostgreSQL ON CONFLICT syntax).
     * @param DbQuery $query
     * @return string
     */
    protected function get_builtReplaceQuery(DbQuery $query): string {
        $partition = $query->get_insertPartition();
        $conflictColumn = count($partition) > 0 ? '`'.$partition[0].'`' : '`id`';

        $assignments = $query->get_updateAssignments();
        $assignmentsSql = count($assignments) > 0 ?
            $this->get_updateAssignments($assignments) :
            $this->get_replaceAssignments($query);

        return 'insert into '.
            $this->get_tables($query->get_tables()).
            $this->get_insertPartition($partition).
            ' values '.
            $this->get_insertValues($query->get_insertValues()).
            ' on conflict ('.$conflictColumn.') do update set '.
            $assignmentsSql.
            ';';
    }

    /**
     * Execute direct queries.
     * @param string $queries
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_queries(string $queries, string $resourceName='Default'): mixed {
        $this->record_query($queries);
        $this->queryResources[$resourceName] = pg_query($this->connection, $queries);
        return ($this->queryResources[$resourceName] !== false);
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId(string $resourceName='Default'): mixed {
        /* PostgreSQL doesn't have lastInsertId, requires RETURNING clause or OID */
        /* Simplified for native driver */
        if (!$this->is_active()) return false;
        $res = pg_query($this->connection, "SELECT lastval();");
        if ($res) {
            $data = pg_fetch_row($res);
            return (int)$data[0];
        }
        return false;
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine(string $resourceName='Default'): array|false {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return pg_fetch_assoc($this->queryResources[$resourceName]);
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine(string $resourceName='Default'): array|false {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return pg_fetch_row($this->queryResources[$resourceName]);
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows(string $resourceName='Default'): int|false {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return pg_affected_rows($this->queryResources[$resourceName]);
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default'): int|false {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return pg_num_fields($this->queryResources[$resourceName]);
        return false;
    }

    /**
     * Return latest query field type.
     * @param integer $fIndex
     * @param string $resourceName
     * @return string
     */
    public function get_queryFieldType(int $fIndex=0, string $resourceName='Default'): string|false {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            $type = pg_field_type($this->queryResources[$resourceName], $fIndex);
            switch ($type) {
                case 'int2':
                case 'int4':
                case 'int8': return 'int';
                case 'float4':
                case 'float8':
                case 'numeric': return 'float';
                case 'bytea': return 'blob';
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
    public function get_queryError(string $resourceName='Default'): string {
        if ($this->is_active())
            return pg_last_error($this->connection);
        return '';
    }

    /* Mark: Resource methods */

    /**
     * Close resource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource(string $resourceName='Default'): bool {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            pg_free_result($this->queryResources[$resourceName]);
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
    public function escape_string(string $string=''): string {
        if (is_null($this->connection))
            return addslashes($string);
        return pg_escape_string($this->connection, $string);
    }
}
?>
