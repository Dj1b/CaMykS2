<?php
/**
 * @brief ModuleLibrary object.
 * <br />Abstract library to create module objects to manage data.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre8
 * @date Creation: Jan 2021
 * @date Modification: Oct 2026
 * @copyright 2021 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * ModuleLibrary class.
 */
abstract class ModuleLibrary extends PluginLibrary {
    /**
     * @var integer $objectId
     * @brief Object identifier.
     */
    protected int $objectId;

    /**
     * @var string $databaseSource
     * @brief Name of database source.
     */
    protected string $databaseSource = 'Main';

    /**
     * @var DatabaseConnector|false $databaseConnector.
     * @brief Link to database connector.
     */
    protected DatabaseConnector|false $databaseConnector;

    /**
     * @var string $dbTableName
     * @brief Built main data table name in database.
     */
    protected string $dbTableName;

    /**
     * @var string $dbLocaleTableName
     * @brief Built localised data table name in database.
     */
    protected string $dbLocaleTableName;

    /**
     * @var array $dbTableSettings
     * @brief Settings for main table creation in database.
     */
    protected array $dbTableSettings = array();

    /**
     * @var array $dbLocaleTableSettings
     * @brief Settings for locale table creation in database.
     */
    protected array $dbLocaleTableSettings = array();

    /**
     * @var array $attributesList
     * @brief List off all attributes names in their sources.
     */
    protected array $attributesList = array(
        'db'    => array(),
        'dbl'   => array(),
        'file'  => array(),
        'filel' => array(),
        'temp'  => array(),
        'templ' => array(),
    );

    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        global $CaMykS;
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define library variables */
        $this->objectId = $id;

        /* Load database connection */
        $this->databaseConnector = &$CaMykS->get_databaseConnection($this->databaseSource);

        /* Set up database tables */
        if ($this->databaseConnector !== false) {
            $this->dbTableName = $this->databaseConnector->get_tablePrefix().$this->libraryName;
            $this->dbLocaleTableName = $this->dbTableName.'Locale';
        }
    }

    /* Mark: Magic methods */

    /**
     * Read attribute value as variable.
     * @param string $name
     * @return mixed
     */
    public function __get(string $name): mixed {
        if (in_array($name, array('objectId')))
            return $this->$name;
        return parent::__get($name);
    }

    /* Mark: Data management methods */

    /**
     * Load object.
     * @param mixed $language
     * @return boolean success
     */
    public function load(mixed $language=true): bool {
        if ($this->objectId === 0)
            return $this->initialise($language);
        return $this->load_fromDatabase($language) and $this->load_files($language) and $this->load_temporary();
    }

    /**
     * Save object.
     * @return boolean success
     */
    public function save(): bool {
        if (!$this->check_data())
            return false;
        return $this->save_databaseAttributes() and $this->save_fileAttributes();
    }

    /**
     * Delete object.
     * @return boolean success
     */
    public function delete(): bool {
        return true;
    }

    /**
     * Install object.
     * @return boolean success
     */
    public function install(): bool {
        return $this->install_databaseAttributes() and $this->install_fileAttributes();
    }

    /**
     * Uninstall object.
     * @return boolean success.
     */
    public function uninstall(): bool {
        return true;
    }

    /**
     * Check library is installed.
     * @return boolean result
     */
    public function is_installed(): bool {
        global $CaMykS;

        /* Load plugin manager */
        $pluginManager = $CaMykS->get_module('SystemPluginManager');

        /* Check localised library */
        if (count($this->attributesList['dbl']) > 0)
            return $pluginManager->check_tableExists($this->dbLocaleTableName) and $pluginManager->check_tableExists($this->dbTableName);

        /* Check not localised library */
        if (count($this->attributesList['db']) > 0)
            return $pluginManager->check_tableExists($this->dbTableName);

        /* Library has no database attribute, no installation required */
        return true;
    }

    /**
     * Return a new instance of the object.
     * @param integer $objectId
     * @return ModuleLibrary
     */
    public function get_newInstance(int $objectId=0): ModuleLibrary {
        $class = $this->libraryNamespace.'\\'.$this->libraryName;
        return new $class($this->libraryParent, $objectId);
    }

    /* Mark: Data management sub methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        /* Check attributes */
        if (!$this->check_attributesValues())
            return false;

        if ($loadPreviousVersion) {
            /* Load previous version */
            $this->previousVersion = $this->get_newInstance($this->objectId);
            $this->previousVersion->load(true);
        }

        /* Return data is conform */
        return true;
    }

    /**
     * Save object database attributes.
     * @return boolean success
     */
    protected function save_databaseAttributes(): bool {
        if ($this->objectId === 0)
            return $this->insert_databaseAttributes();
        return $this->update_databaseAttributes();
    }

    /**
     * Insert object database attributes.
     * @return boolean succes
     */
    protected function insert_databaseAttributes(): bool {
        /* Compile data for non-localised attributes */
        $values = array();
        foreach ($this->attributesList['db'] as $attributeName)
            $values[$attributeName] = $this->libraryAttributes[$attributeName]->get_value();

        /* Build query for non-localised attributes */
        $query = new DbQuery('insert');
        $query->add_table($this->dbTableName);
        $query->add_insertValues($values);

        if (!$this->execute_DBQuery($query))
            return false;

        $this->objectId = $this->get_DBQueryGeneratedId();

        /* Check for localised attributes */
        if (count($this->attributesList['dbl']) > 0) {
            /* Compile data form localised attributes */

            /* Build query for localised attributes */

        }
        return true;
    }

    /**
     * Update object database attributes.
     * @return boolean success
     */
    protected function update_databaseAttributes(): bool {
        /* Compile data for non-localised attributes */
        $values = array();
        foreach ($this->attributesList['db'] as $attributeName)
            $values[$attributeName] = $this->libraryAttributes[$attributeName]->get_value();

        /* Build query for non-localised attributes */
        $query = new DbQuery('update');
        $query->add_table($this->dbTableName);
        $query->add_updateAssignmentList($values);
        $query->add_whereCondition(new DbQueryWhereCondition('id', $this->objectId));

        if (!$this->execute_DBQuery($query))
            return false;

        /* Check for localised attributes */
        if (count($this->attributesList['dbl']) > 0) {
            /* Compile data form localised attributes */

            /* Build query for localised attributes */

        }
        return true;
    }

    /**
     * Save object file attributes.
     * @return boolean success
     */
    protected function save_fileAttributes(): bool {
        return true;
    }

    /**
     * Define database attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_databaseAttribute(string $name, string $type, mixed $default=null, array $params=array(), bool $isLocalised=false): bool {
        /* Define attribute */
        if (!$this->define_attribute($name, $type, $default, $params, $isLocalised))
            return false;

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['dbl'][] = $name;
        else
            $this->attributesList['db'][] = $name;

        return true;
    }

    /**
     * Define file attribute.
     * @param string $name
     * @param string $type
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_fileAttribute(string $name, string $type='File', array $params=array(), bool $isLocalised=false): bool {
        /* Define attribute */
        if (!$this->define_attribute($name, $type, null, $params, $isLocalised))
            return false;

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['filel'][] = $name;
        else
            $this->attributesList['file'][] = $name;

        return true;
    }

    /**
     * Define temporary attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_temporaryAttribute(string $name, string $type, mixed $default=null, array $params=array(), bool $isLocalised=false): bool {
        /* Define attribute */
        if (!$this->define_attribute($name, $type, $default, $params, $isLocalised))
            return false;

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['templ'][] = $name;
        else
            $this->attributesList['temp'][] = $name;

        return true;
    }

    /**
     * Install database attributes.
     * @return boolean success
     */
    protected function install_databaseAttributes(): bool {
        /* Check library has database attributes */
        if (count($this->attributesList['db']) === 0 and $this->attributesList['dbl'] === 0)
            return true;

        /* Create main table field list */
        $fields = array();
        foreach ($this->attributesList['db'] as $name)
            $fields[$name] = $this->libraryAttributes[$name]->export_databaseParameters();

        /* Create main table */
        if (!$this->create_DBTable($this->dbTableName, $fields, $this->dbTableSettings)) {
            /* Add error */

            /* Return failure */
            return false;
        }

        /* Create locale table */
        if (count($this->attributesList['dbl']) > 0) {
            /* Add system fields */
            $fields = array(
                '_ObjectId'         => array('Type' => 'integer', 'Default' => 0, 'Min' => 0, 'Unsigned' => true, 'IndexName'=>true),
                '_ObjectLanguage'   => array('Type' => 'text', 'Default' => '', 'Length' => 16),
            );

            /* Add attribute fields */
            foreach ($this->attributesList['dbl'] as $name)
                $fields[$name] = $this->libraryAttributes[$name]->export_databaseParameters();

            /* Create table */
            if (!$this->create_DBTable($this->dbLocaleTableName, $fields, $this->dbLocaleTableSettings)) {
                /* Add error */

                /* Return failure */
                return false;
            }
        }

        /* Return success */
        return true;
    }

    /**
     * Install file attributes.
     * @return boolean success
     */
    protected function install_fileAttributes(): bool {
        return true;
    }

    /* Mark: Multi object management methods */


    /* Mark: Database methods */

    /**
     * Send a database error to CaMykS.
     * @param string $error
     * @return false
     */
    protected function add_databaseError(string $error): false {
        global $CaMykS;

        /* Send error to CaMykS */
        $CaMykS->trigger_databaseError($this->libraryName, 0, $this->get_errorCode(0), $error);

        /* Return false */
        return false;
    }

    /**
     * Execute database query.
     * @param mixed $query
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBQuery(mixed $query, ?string $resourceName=null): mixed {
        return $this->databaseConnector->execute_query($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute database direct query.
     * @param string $query
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBDirectQuery(string $query, ?string $resourceName=null): mixed {
        return $this->databaseConnector->execute_directQuery($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute prepared query using DbQuery builder.
     * @param DbQuery $query
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBBuiltQuery(DbQuery $query, ?string $resourceName=null): mixed {
        return $this->databaseConnector->execute_builtQuery($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute multiple database queries at once.
     * @param string $queries
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBMultiQueries(string $queries='', ?string $resourceName=null): mixed {
        return $this->databaseConnector->execute_queries($queries, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryGeneratedId(?string $resourceName=null): mixed {
        return $this->databaseConnector->get_queryGeneratedId($resourceName);
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryDataLine(?string $resourceName=null): mixed {
        return $this->databaseConnector->get_queryDataLine($resourceName);
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryNumDataLine(?string $resourceName=null): mixed {
        return $this->databaseConnector->get_queryNumDataLine($resourceName);
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryAffectedRows(?string $resourceName=null): mixed {
        return $this->databaseConnector->get_queryAffectedRows($resourceName);
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_DBQueryError(?string $resourceName=null): string {
        return $this->databaseConnector->get_queryError($resourceName);
    }

    /**
     * Create a table in database.
     * @param string $name
     * @param array $fields
     * @param array $params
     * @return boolean success
     */
    public function create_DBTable(string $name, array $fields, array $params=array()): bool {
        return $this->databaseConnector->create_table($name, $fields, $params);
    }

    /**
     * Return espaced string.
     * @param string $string
     * @return string
     */
    public function escape_string(string $string): string {
        return $this->databaseConnector->escape_string($string);
    }
}
?>
