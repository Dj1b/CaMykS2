<?php
/**
 * @brief Oracle database driver.
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
 * OracleDatabaseDriver class.
 */
final class OracleDatabaseDriver extends SQLDatabaseDriverInterface {
    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture = 'server';

    /**
     * @var resource $connection
     * @brief Internal connection to Oracle server.
     */
    private $connection;

    /**
     * Open connection.
     * @return boolean success
     */
    public function open() {
        /* Build connection string */
        $db = '';
        if ($this->get_param('Host') !== '') {
            $db = '//' . $this->get_param('Host');
            if ($this->get_param('Port') > 0)
                $db .= ':' . $this->get_param('Port');
            if ($this->get_param('Name') !== '')
                $db .= '/' . $this->get_param('Name');
        } else {
            $db = $this->get_param('Name');
        }

        /* Perform database server connection */
        try {
            $this->connection = oci_connect($this->get_param('Login'), $this->get_param('Password'), $db, 'AL32UTF8');
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
        if (!oci_close($this->connection))
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

        /* Return Oracle version */
        return oci_server_version($this->connection);
    }

    /* Mark: Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database($name=null) {
        /* Oracle matches database with user/schema or service name in connection */
        return true;
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database($name=null) {
        /* Check if schema exists */
        $sql = "SELECT count(*) FROM all_users WHERE username = '".strtoupper($this->escape_string($name ?? $this->get_param('Name')))."'";
        if (!$this->execute_directQuery($sql, 'checkDb'))
            return false;
        $data = $this->get_queryNumDataLine('checkDb');
        return ($data and $data[0] > 0);
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $encoding
     * @return boolean
     */
    public function create_database($name, $encoding='UTF8') {
        /* Creating a database in Oracle is a complex task usually done by DBA. */
        /* We'll assume we're creating a user/schema. */
        $sql = 'CREATE USER "'.$name.'" IDENTIFIED BY "'.$this->get_param('Password').'";';
        if (!$this->execute_directQuery($sql))
            return false;
        return $this->execute_directQuery('GRANT CONNECT, RESOURCE TO "'.$name.'";');
    }

    /**
     * Dump database.
     * @return mixed
     */
    public function dump_database() {
        // Simplified dump for Oracle
        return "-- Oracle native dump not implemented\n";
    }

    /**
     * Return database size.
     * @return integer
     */
    public function get_databaseSize() {
        $sql = "SELECT sum(bytes) FROM user_segments";
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
        $sql = "SELECT table_name FROM user_tables";
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
        $sql = 'CREATE TABLE "'.$name.'" (';

        /* Add auto id */
        if ($params['AutoId'])
            $sql .= '"id" NUMBER GENERATED BY DEFAULT AS IDENTITY,';

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
                        if ($data['Length'] <= 4000)
                            $s .= 'VARCHAR2('.$data['Length'].')';
                        else
                            $s .= 'CLOB';
                    } else {
                        $s .= 'CLOB';
                    }
                    if ($data['Default'] !== '')
                        $s .= ' DEFAULT \''.addslashes($data['Default']).'\'';
                    break;
                case 'tinytext':
                    $s .= 'VARCHAR2(255)';
                    break;
                case 'mediumtext':
                case 'longtext':
                    $s .= 'CLOB';
                    break;
                case 'integer':
                    $s .= 'NUMBER';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(int)$data['Default'];
                    break;
                case 'float':
                case 'price':
                    $s .= 'NUMBER';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(float)$data['Default'];
                    break;
                case 'date':
                    $s .= 'DATE';
                    break;
                case 'datetime':
                    $s .= 'TIMESTAMP';
                    break;
                case 'time':
                    $s .= 'TIMESTAMP'; /* Oracle doesn't have a pure TIME type */
                    break;
                case 'blob':
                    $s .= 'BLOB';
                    break;
                default:
                    $s .= 'CLOB';
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
        $sql .= ')';

        /* Execute table creation */
        if (!$this->execute_directQuery($sql))
            return false;

        /* Create indexes separately */
        foreach ($params['Indexes'] as $iname => $indexes) {
            if (is_string($indexes)) {
                $indexSql = 'CREATE INDEX "idx_'.$name.'_'.$iname.'" ON "'.$name.'" ("'.$indexes.'")';
                $this->execute_directQuery($indexSql);
            } elseif (is_array($indexes)) {
                $indexSql = 'CREATE INDEX "idx_'.$name.'_'.$iname.'" ON "'.$name.'" ("'.implode('","', $indexes).'")';
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
        $sql = 'DROP TABLE "'.$name.'" CASCADE CONSTRAINTS';
        return $this->execute_directQuery($sql);
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        $sql = 'TRUNCATE TABLE "'.$name.'"';
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
        $this->queryResources[$resourceName] = oci_parse($this->connection, $query);
        if ($this->queryResources[$resourceName] === false)
            return false;
        return oci_execute($this->queryResources[$resourceName]);
    }

    /**
     * Execute direct queries.
     * @param string $queries
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_queries($queries, $resourceName='Default') {
        /* Oracle doesn't support multiple queries in oci_parse by default */
        /* We'll split them (naively) */
        $list = explode(';', $queries);
        foreach ($list as $query) {
            $query = trim($query);
            if ($query !== '') {
                if (!$this->execute_directQuery($query, $resourceName))
                    return false;
            }
        }
        return true;
    }

    /* Mark: Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId(string $resourceName='Default') {
        /* Oracle requires RETURNING clause or manual sequence management. */
        /* Simplified: we don't have enough context here. */
        return false;
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return oci_fetch_array($this->queryResources[$resourceName], OCI_ASSOC + OCI_RETURN_NULLS);
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return oci_fetch_array($this->queryResources[$resourceName], OCI_NUM + OCI_RETURN_NULLS);
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return oci_num_rows($this->queryResources[$resourceName]);
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false)
            return oci_num_fields($this->queryResources[$resourceName]);
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
            $type = oci_field_type($this->queryResources[$resourceName], $fIndex + 1);
            switch ($type) {
                case 'NUMBER': return 'int'; /* Or float, Oracle uses NUMBER for both */
                case 'CLOB':
                case 'BLOB': return 'blob';
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
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            $e = oci_error($this->queryResources[$resourceName]);
        } else {
            $e = oci_error($this->connection);
        }
        return $e['message'] ?? '';
    }

    /* Mark: Resource methods */

    /**
     * Close resource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) and $this->queryResources[$resourceName] !== false) {
            oci_free_statement($this->queryResources[$resourceName]);
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
        /* oci8 doesn't have an escape function, use str_replace for single quotes */
        return str_replace("'", "''", $string);
    }
}
?>
