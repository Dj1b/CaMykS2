<?php
/**
 * @brief PDO MySQLi database driver.
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
 * PDOMySQLiDatabaseDriver class.
 */
final class PDOMySQLiDatabaseDriver extends SQLDatabaseDriverInterface {
    /**
     * @var string $architecture
     * @brief Architecture of the database, can be "server" or "file"
     */
    protected $architecture = 'server';

    /**
     * @var \PDO $connection
     * @brief Internal PDO connection to MySQL server.
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
        /* Build DSN */
        $dsn = 'mysql:host='.$this->get_param('Host');
        if ($this->get_param('Port') > 0)
            $dsn .= ';port='.$this->get_param('Port');
        if ($this->get_param('Name') !== '')
            $dsn .= ';dbname='.$this->get_param('Name');

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

        /* Return database server version */
        return $this->connection->getAttribute(\PDO::ATTR_SERVER_VERSION);
    }

    /* Mark: Database methods */

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
        try {
            $this->connection->exec('USE `'.($name ?? $this->get_param('Name')).'`;');
            return true;
        } catch (\PDOException $e) {
            return false;
        }
    }

    /**
     * Check database exists.
     * @param string $name
     * @return boolean success
     */
    public function check_database($name=null) {
        if (!$this->execute_directQuery('SHOW DATABASES LIKE "'.$this->escape_string($name ?? $this->get_param('Name')).'";'))
            return false;
        return $this->get_queryDataLine() !== false;
    }

    /**
     * Create a database.
     * @param string $name
     * @param string $encoding
     * @return boolean
     */
    public function create_database($name, $encoding='UTF8') {
        return $this->execute_directQuery('CREATE DATABASE `'.($name ?? $this->get_param('Name')).'` DEFAULT CHARACTER SET '.$this->charsets[$encoding].';');
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
        if ($this->execute_directQuery('SHOW TABLES;', 'tableList')) {

            /* Build tables to content */
            while ($table = $this->get_queryNumDataLine('tableList')) {

                /* Build table */
                if ($this->execute_directQuery('SHOW CREATE TABLE `'.$table[0].'`;', 'createTable')) {
                    if ($createTable = $this->get_queryNumDataLine('createTable')) {

                        /* Build table comments */
                        $content .= "-- ************************\n";
                        $content .= "-- Table \"".$table[0]."\"\n";
                        $content .= "-- ************************\n";

                        /* Build table creation */
                        $content .= $createTable[1].";\n\n";

                        /* Build table content */
                        if ($this->execute_directQuery('SELECT * FROM `'.$table[0].'`;', 'tableItemList')) {
                            while ($insertData = $this->get_queryNumDataLine('tableItemList')) {
                                $content .= "INSERT INTO `".$table[0]."` VALUES(";
                                for ($i=0; $i < $this->get_queryFieldCount('tableItemList'); $i++) {
                                    if ($i != 0)
                                        $content .=  ", ";

                                    if (is_null($insertData[$i])) {
                                        $content .= 'NULL';
                                    } elseif ($this->get_queryFieldType($i, 'tableItemList') == "string" || $this->get_queryFieldType($i, 'tableItemList') == "blob") {
                                        $content .=  "'" . addslashes($insertData[$i]) . "'";
                                    } else {
                                        $content .= addslashes($insertData[$i]);
                                    }
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
        if ($this->execute_directQuery('SHOW TABLE STATUS;')) {
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
        $sql = 'SHOW TABLES;';
        if ($this->execute_directQuery($sql))
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
            $sql .= '`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,';

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
                    if (is_int($data['Length'])) {
                        if ($data['Length'] < 256)
                            $s .= 'VARCHAR('.$data['Length'].') DEFAULT "'.addslashes($data['Default']).'"';
                        elseif ($data['Length'] < 65535)
                            $s .= 'TEXT';
                        elseif ($data['Length'] < 16777215)
                            $s .= 'MEDIUMTEXT';
                        else
                            $s .= 'LONGTEXT';
                    } elseif (is_string($data['Length'])) {
                        $data['Length'] = strtolower($data['Length']);
                        if ($data['Length'] === 'small')
                            $s .= 'VARCHAR(255) DEFAULT "'.addslashes($data['Default']).'"';
                        elseif ($data['Length'] === 'medium')
                            $s .= 'MEDIUMTEXT';
                        elseif ($data['Length'] === 'long')
                            $s .= 'LONGTEXT';
                        else
                            $s .= 'TEXT';
                    } else {
                        $s .= 'TEXT';
                    }
                    break;
                case 'tinytext':
                    $s .= 'TINYTEXT';
                    break;
                case 'mediumtext':
                    $s .= 'MEDIUMTEXT';
                    break;
                case 'longtext':
                    $s .= 'LONGTEXT';
                    break;
                case 'integer':
                    if (!is_null($data['Size'])) {
                        $data['Size'] = strtolower($data['Size']);
                        if ($data['Size'] === 'tiny')
                            $s .= 'TINYINT';
                        elseif ($data['Size'] === 'small')
                            $s .= 'SMALLINT';
                        elseif ($data['Size'] === 'medium' || $data['Size'] === '')
                            $s .= 'INT';
                        elseif ($data['Size'] === 'big')
                            $s .= 'BIGINT';
                        else
                            $s .= 'INT';
                    } else {
                        if ($data['Max'] <= 255)
                            $s .= 'TINYINT';
                        elseif ($data['Max'] <= 65535)
                            $s .= 'SMALLINT';
                        elseif ($data['Max'] <= 16777215)
                            $s .= 'MEDIUMINT';
                        elseif ($data['Max'] <= 4294967295)
                            $s .= 'INT';
                        else
                            $s .= 'BIGINT';
                    }
                    if ($data['Unsigned'])
                        $s .= ' UNSIGNED';
                    if (is_numeric($data['Default']))
                        $s .= ' DEFAULT '.(int)$data['Default'];
                    break;
                case 'float':
                    $s .= 'DOUBLE';
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
                    $s .= 'DATETIME';
                    break;
                case 'time':
                    $s .= 'TIME';
                    break;
                case 'blob':
                    $s .= 'BLOB';
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
            $sql .= ' PRIMARY KEY (`'.implode('`,`', $params['PrimaryKeys']).'`),';

        /* Add indexes */
        foreach ($params['Indexes'] as $name => $indexes) {
            if (is_string($indexes))
                $sql .= ' INDEX `idx_'.$name.'` (`'.$indexes.'`),';
            elseif (is_array($indexes))
                $sql .= ' INDEX `idx_'.$name.'` (`'.implode('`,`', $indexes).'`),';
        }

        /* Remove trailing comma */
        $sql = rtrim($sql, ',');

        /* Close fields information */
        $sql .= ')';

        /* Add engine and charset */
        $sql .= ' ENGINE=InnoDB';
        if ($params['Charset'] != '')
            $sql .= ' DEFAULT CHARSET='.$this->charsets[$params['Charset']];
        elseif ($this->get_param('Charset') != '')
             $sql .= ' DEFAULT CHARSET='.$this->charsets[$this->get_param('Charset')];
        else
            $sql .= ' DEFAULT CHARSET='.$this->charsets['UTF8'];

        $sql .= ';';

        /* Execute table creation */
        return $this->execute_directQuery($sql);
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
        $sql = 'DROP TABLE IF EXISTS `'.$name.'`;';
        return $this->execute_directQuery($sql);
    }

    /**
     * Truncate a table from the database.
     * @param string $name
     * @return boolean success
     */
    public function truncate_table($name) {
        $sql = 'TRUNCATE TABLE `'.$name.'`;';
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
        if (isset($this->queryResources[$resourceName]) && $this->queryResources[$resourceName] !== false)
            return $this->queryResources[$resourceName]->fetch(\PDO::FETCH_ASSOC);
        return false;
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return array
     */
    public function get_queryNumDataLine(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) && $this->queryResources[$resourceName] !== false)
            return $this->queryResources[$resourceName]->fetch(\PDO::FETCH_NUM);
        return false;
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryAffectedRows(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) && $this->queryResources[$resourceName] !== false)
            return $this->queryResources[$resourceName]->rowCount();
        return 0;
    }

    /**
     * Return latest query field count.
     * @param string $resourceName
     * @return integer
     */
    public function get_queryFieldCount(string $resourceName='Default') {
        if (isset($this->queryResources[$resourceName]) && $this->queryResources[$resourceName] !== false)
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
        if (isset($this->queryResources[$resourceName]) && $this->queryResources[$resourceName] !== false) {
            $meta = $this->queryResources[$resourceName]->getColumnMeta($fIndex);

            /* Map PDO types to CaMykS types */
            $type = $meta['native_type'] ?? '';
            switch ($type) {
                case 'TINY':
                case 'SHORT':
                case 'LONG':
                case 'LONGLONG':
                case 'INT24':
                    return 'int';
                case 'FLOAT':
                case 'DOUBLE':
                case 'DECIMAL':
                case 'NEWDECIMAL':
                    return 'float';
                case 'BLOB':
                case 'TINY_BLOB':
                case 'MEDIUM_BLOB':
                case 'LONG_BLOB':
                    return 'blob';
                case 'STRING':
                case 'VAR_STRING':
                    return 'string';
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
        /* If not active, use addslashes as fallback */
        if (!$this->is_active())
            return addslashes($string);

        $quoted = $this->connection->quote($string);
        if ($quoted === false)
             return addslashes($string);
        return substr($quoted, 1, -1);
    }
}
?>
