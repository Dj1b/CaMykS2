<?php
/**
 * @brief MySQLi database drivers.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Nov 2021
 * @date Modification: Jan 2022
 * @copyright 2021 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * MySQLiDatabaseDriver class.
 */
final class MySQLiDatabaseDriver extends SQLDatabaseDriverInterface {
    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture = 'server';

    /**
     * @var object $connection
     * @brief Internal connection to MySQL server.
     */
    private $connection;

    /**
     * @var array $charsets
     * @brief Return default charsets for encoding values.
     */
    private $charsets = array(
        'UTF8'  => 'utf8mb4',
        'UTF16' => 'utf16',
        'UTF32' => 'utf32',
    );

    /**
     * Open connection.
     * @return boolean success
     */
    public function open() {
        /* Perform database server connection */
        $this->connection = @(new\ MySQLi($this->get_param('Host').($this->get_param('Port') > 0 ? ':'.$this->get_param('Port') : ''), $this->get_param('Login'), $this->get_param('Password')));

        /* Stores if connection is made successfully */
        $this->status = ($this->connection->connect_errno === 0);

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

        /* Try to close database server connection */
        if (!$this->connection->close())
            return false;

        /* Update status */
        $this->status = false;

        /* Return success */
        return true;
    }

    /* Driver methods */

    /**
     * Return database driver version.
     * @return string
     */
    public function get_driverVersion() {
        /* Check connection is open */
        if (!$this->is_active())
            return '';

        /* Return database server version */
        return $this->connection->server_info;
    }

    /* Database methods */

    /**
     * Select database.
     * @param string $name
     * @return boolean success
     */
    public function select_database($name=null) {
        /* Check connection is open */
        if (!$this->is_active())
            return false;

        /* Select database */
        return $this->connection->select_db($name);
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database($name=null) {
        return $this->select_database($name);
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $encoding
     * @return boolean
     */
    public function create_database($name, $encoding='UTF8') {
        return $this->execute_directQuery('create database `'.$name.'` default character set '.$this->charsets[$encoding].';');
    }

    /**
     * Dump database.
     * @return mixed
     */
    public function dump_database() {
        $content = '';

        /* Build header comments */
        $content = "-- ************************\n";
        $content .= "-- Database \"".$this->get_param('Database')."\" dump\n";
        $content .= "-- ".date('r')."\n";
        $content .= "-- ************************\n\n";

        /* Load table list */
        if ($this->execute_directQuery('show tables;', 'tableList')) {

            /* Build tables to content */
            while ($table = $this->get_queryNumDataLine('tableList')) {

                /* Build table */
                if ($this->execute_directQuery('show create table '.$table[0].';', 'createTable')) {
                    if ($createTable = $this->get_queryNumDataLine('createTable')) {

                        /* Build table comments */
                        $content .= "-- ************************\n";
                        $content .= "-- Table \"".$table[0]."\"\n";
                        $content .= "-- ************************\n";

                        /* Build table creation */
                        $content .= $createTable[1].";\n\n";

                        /* Build table content */
                        if ($this->execute_directQuery('select * from '.$table[0].';', 'tableItemList')) {
                            while ($insertData = $this->get_queryNumDataLine('tableItemList')) {
                                $content .= "insert into ".$table[0]." values(";
                                for ($i=0; $i < $this->get_queryFieldCount('tableItemList'); $i++) {
                                    if ($i != 0)
                                        $content .=  ", ";
                                    if ($this->get_queryFieldType($i, 'tableItemList') == "string" || $this->get_queryFieldType($i, 'tableItemList') == "blob")
                                        $content .=  "'" . addslashes($insertData[$i]) . "'";
                                    else
                                        $content .= addslashes($insertData[$i]);
                                }
                                $content .=  ");\n";
                            }
                        }
                    }
                    $content .= "\n\n";
                }
            }
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
        if ($this->execute_query('show table status;')) {
            /* set size with infos */
            while ($data = $this->get_queryDataLine()) {
                $size += $data['Data_length'] + $data['Index_length'];
            }
        }
        return $size;
    }

    /**
     * Return database summary.
     * @return array
     */
    public function get_databaseSummary() {

    }

    /* Table methods */

    /**
     * Return database table list.
     * @return array
     */
    public function get_tableList() {
        /* Initialise table list */
        $list = array();

        /* Load list from database */
        $sql = 'show tables;';
        if ($this->execute_query($sql))
            while ($data = $this->get_queryNumDataLine())
                $list[] = $data[0];

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
        $sql = 'create table if not exists '.$name.' (';

        /* Add auto id */
        if ($params['AutoId'])
            $sql .= ' id bigint unsigned not nulll auto_increment,';

        /* Add fields description */
        foreach ($fields as $field => $data) {

            /* Merge field settings */
            $data = array_merge(array(
                /* Generic values */
                'Type'      => '',  /* text, integer, float, date, datetime, time */
                'Default'   => '',
                'NotNull'   => false,

                /* Text settings */
                'Length'    => '',

                /* Numeric settings */
                'Min'       => 0,
                'Max'       => 2147483648,
                'Size'      => null,
                'Signed'    => true,

                /* Primary key and index settings */
                'PrimaryKey'    => false,
                'IndexName'     => false,
                'IndexLength'   => 12,
            ), $data);

            /* Add field name */
            $s = ' `'.$field['name'].'` ';

            /* Add field description from type */
            switch ($field['Type']) {
                case 'text':
                    if (is_int($data['Length'])) {
                        if ($data['Length'] < 256)
                            $s .= 'varchar('.$data['Length'].') default "'.addslashes($data['Default']).'"';
                        if ($data['Length'] < 65535)
                            $s .= 'text ';
                        if ($data['Length'] < 16777215)
                            $s .= 'mediumtext';
                        else
                            $s .= ' longtext';
                    } elseif (is_string($data['Length'])) {
                        $data['Length'] = strtolower($data['Length']);
                        if ($data['Length'] === 'small')
                            $s .= 'varchar(255) default "'.addslashes($data['Default']).'"';
                        elseif ($data['Length'] === 'medium')
                            $s .= 'mediumtext';
                        elseif ($data['Length'] === 'long')
                            $s .= 'longtext';
                        else
                            $s .= 'text';
                    }
                    break;
                case 'integer':
                    if (!is_null($data['Size']) and in_array(strtolower($data['Size']), array('tiny', 'small', 'medium', '', 'big'))) {
                        $data['Size'] = strtolower($data['Size']);
                        $s .= $data['Size'].'int';
                        if ($data['Signed'] === false)
                            $s .= ' unsigned';
                    } else {
                        if ($data['Min'] >= 0 and $data['Max'] <= 255)
                            $s .= 'tinyint unsigned';
                        elseif ($data['Min'] >= -128 and $data['Max'] <= 127)
                            $s .= 'tinyint';
                        elseif ($data['Min'] >= 0 and $data['Max'] <= 65535)
                            $s .= 'smallint unsigned';
                        elseif ($data['Min'] >= -32768 and $data['Max'] <= 32767)
                            $s .= 'smallint';
                        elseif ($data['Min'] >= 0 and $data['Max'] <= 16777215)
                            $s .= 'mediumint unsigned';
                        elseif ($data['Min'] >= -8388608 and $data['Max'] <= 8388607)
                            $s .= 'mediumint';
                        elseif ($data['Min'] >= 0 and $data['Max'] <= 4294967295)
                            $s .= 'int unsigned';
                        elseif ($data['Min'] >= -2147483648 and $data['Max'] <= 2147483647)
                            $s .= 'int';
                        elseif ($data['Min'] < 0)
                            $s .= 'bigint';
                        else
                            $s .= 'bigint unsigned';
                    }
                    if (is_numeric($data['Default']))
                        $s .= ' default '.(int)$data['Default'];

                    break;
                case 'float':
                    $s .= 'float';
                    if ($data['Signed'] === false or $data['Min'] >= 0)
                        $s .= ' unsigned';
                    $s .= ' default '.(float)$data['Default'];
                    break;
                case 'price':
                    $s .= 'decimal';
                    if (($data['Min'] >= 0 or $data['Signed'] === false) and $data['Max'] <= 999999)
                        $s .= '(8, 2) unsigned';
                    elseif ($data['Min'] >= -500000 and $data['Max'] <= 499999)
                        $s .= '(8, 2)';
                    elseif ($data['Min'] < 0 or $data['Signed'] === false)
                        $s .= '(14,2) unsigned';
                    else
                        $s .= '(14,2)';
                    $s .= ' default '.(float)$data['Default'];
                    break;
                case 'date':
                case 'datetime':
                case 'time':
                    $s .= $data['Type'].' default CURRENT_TIMESTAMP';
                    break;
                default:
                    /* Type not known : return error */

            }

            /* Check for not null information */
            if ($data['NotNull'])
                $s .= ' not null';

            /* Add separator between fields */
            $sql .= $s.', ';
        }

        /* Add primary keys */
        if (count($params['PrimaryKeys']) > 0)
            $sql .= ' primary key (`'.implode('`,`', $params['PrimaryKeys']).'`)';

        /* Add indexes */
        foreach ($params['Indexes'] as $name => $index) {
            if (is_string($index)) {

            }




            if (is_integer($name))
                $name = 'key';

        }

        /* Add charset */
        if ($params['Charset'] !== '')
            $sql .= ' character set '.$this->charsets[$params['Charset']].'';

        /* Finalise query */
        $sql .= ';';

        /* Return result of SQL query execution */
        return ($this->execute_directQuery($sql) or $this->add_error(11, $sql, __FILE__, __LINE__));
    }

    /**
     * Duplicate a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function duplicate_table($params=array()) {

    }

    /**
     * Drop a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function drop_table($name) {
        $sql = 'drop table '.$name.';';
        return $this->execute_directQuery($sql);
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        $sql = 'trauncate table '.$name.';';
        return $this->execute_directQuery($sql);

    }

    /**
     * Modify a table in the database.
     * @param array $params
     * @return boolean success
     */
    public function alter_table($params=array()) {

    }

    /* Query methods */

    /**
     * Execute direct query.
     * @param string $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_directQuery($query, $resourceName='Default') {
        $this->record_query($query);
        $this->queryResources[$resourceName] = @$this->connection->query($query);
        return ($this->queryResources[$resourceName] !== false);
    }

    /**
     * Execute direct queryies.
     * @param string $queries
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_queries($queries, $resourceName='Default') {
        $this->record_query($queries);
        $this->queryResources[$resourceName] = @$this->connection->multi_query($queries);
        return ($this->queryResources[$resourceName] !== false);
    }

    /* Query return methods */

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryGeneratedId($resourceName='Default') {
        if ($this->is_active())
            return $this->connection->insert_id;
        return false;
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return array
     */
    public function get_queryDataLine($resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->fetch_assoc();
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine($resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->fetch_row();
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows($resourceName='Default') {
        if ($this->is_active())
            return $this->connection->affected_rows;
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount($resourceName='Default') {
        if (isset($this->queryResources[$resourceName]))
            return $this->queryResources[$resourceName]->field_count();
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
            $finfo = $this->queryResources[$resourceName]->fetch_field_direct($fIndex);
            return $this->get_compatibleType($finfo->type);
        }
        return false;
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_queryError($resourceName='Default') {
        if ($this->is_active())
            return $this->connection->error;
        return '';
    }

    /* Ressource methods */

    /**
     * Close ressource.
     * @param string $resourceName
     * @return boolean result
     */
    public function close_ressource($resourceName='Default') {
        if (isset($this->queryResources[$resourceName])) {
            if (!$this->queryResources[$resourceName]->close())
                return false;
            unset($this->queryResources[$resourceName]);
        }
        return true;
    }

    /* Data methods */

    /**
     * Return escaped string.
     * @param string $string
     * @return string
     */
    public function escape_string($string='') {
        return mysqli_real_escape_string($this->connection, $string);
    }

    /* Private tool methods */

    /**
     * Return mysql compatible type from given type.
     * @param integer $type
     * @return string
     */
    private function get_compatibleType($type=1) {
        switch ($type) {
            /* int */
            case 1 /* tinyint */: case 2 /* smallint */: case 3 /* int */:  case 8 /* bigint */: case 9 /* mediumint */: case 16 /* bit */: return 'int';

            /* float */
            case 4 /* float */: case 5 /* double */: case 246 /* decimal*/: return 'real';

            /* date & time */
            case 7: return 'timestamp';
            case 10: return 'date';
            case 11: return 'time';
            case 12: return 'datetime';
            case 13: return 'year';

            /* blobs */
            case 252: return 'blob';

            /* texts */
            case 253 /* varchar */ : case 254 /* char */: return 'string';
        }
        return '';
    }
}
?>
