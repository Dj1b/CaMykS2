<?php
/**
 * @brief ModuleLibrary object.
 * <br />Abstract library to create module objects to manage data.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre9
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
     * @var ModuleLibrary|null $previousVersion
     * @brief Previous version of object before edition.
     */
    protected ?ModuleLibrary $previousVersion = null;

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
    protected string $dbTableName = '';

    /**
     * @var string $dbLocaleTableName
     * @brief Built localised data table name in database.
     */
    protected string $dbLocaleTableName = '';

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
        } else {
            $this->dbTableName = $this->libraryName;
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
        if ($this->objectId === 0) {
            $this->initialise();
            return true;
        }
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
        if ($this->objectId === 0)
            return false;
        return $this->delete_databaseAttributes() and $this->delete_fileAttributes();
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

    /* Mark: Data management methods */

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

    /* Mark: Public Multi object management methods */

    /**
     * Return default parameters
     * @return array
     */
    protected function get_queryDefaultParams(): array {
        return array();
    }

    /**
     * Build where statement from params.
     * @param array $params
     * @return array
     */
    protected function build_queryWhereParams(array $params=array()): array {
        return array();
    }

    /**
     * Return object list.
     * @param array $params
     * @return array
     */
    public function get_list(array $params=array()): array {
        global $CaMykS;

        /* Default params list */
        $language = isset($CaMykS) ? $CaMykS->get_currentLanguage() : true;
        $defaultParams = array('Language'=>$language, 'Limit'=>-1, 'Count'=>-1, 'OrderBy'=>array('id'=>'asc'));

        /* Merge params */
        $params = array_merge($this->get_queryDefaultParams(), $defaultParams, $params);

        /* Build where parameters */
        $whereParams = $this->build_queryWhereParams($params);

        /* Build and return list */
        return $this->get_objectList($params['Language'], $whereParams, $params['Limit'], $params['Count'], $params['OrderBy']);
    }

    /**
     * Return object count.
     * @param array $params
     * @return integer
     */
    public function get_count(array $params=array()): int {
        /* Default params list */
        $params = array_merge($this->get_queryDefaultParams(), $params);

        /* Build where parameters */
        $whereParams = $this->build_queryWhereParams($params);

        /* Return result */
        return $this->get_objectCount($whereParams);
    }

    /**
     * Return object id list.
     * @param array $params
     * @return array
     */
    public function get_idList(array $params=array()): array {
        global $CaMykS;

        /* Default params list */
        $language = isset($CaMykS) ? $CaMykS->get_currentLanguage() : true;
        $defaultParams = array('Language'=>$language, 'Limit'=>-1, 'Count'=>-1, 'OrderBy'=>array('id'=>'asc'));

        /* Merge params */
        $params = array_merge($this->get_queryDefaultParams(), $defaultParams, $params);

        /* Build where parameters */
        $whereParams = $this->build_queryWhereParams($params);

        /* Build and return list */
        return $this->get_objectIdList($params['Language'], $whereParams, $params['Limit'], $params['Count'], $params['OrderBy']);
    }

    /**
     * Return object attribute list.
     * @param string $attributeName
     * @param array $params
     * @return array
     */
    public function get_attributeList(string $attributeName, array $params=array()): array {
        global $CaMykS;

        /* Default params list */
        $language = isset($CaMykS) ? $CaMykS->get_currentLanguage() : true;
        $defaultParams = array('Language'=>$language, 'Limit'=>-1, 'Count'=>-1, 'OrderBy'=>array('id'=>'asc'));

        /* Merge params */
        $params = array_merge($this->get_queryDefaultParams(), $defaultParams, $params);

        /* Build where parameters */
        $whereParams = $this->build_queryWhereParams($params);

        /* Build and return list */
        return $this->get_objectAttributeList($attributeName, $params['Language'], $whereParams, $params['Limit'], $params['Count'], $params['OrderBy']);
    }

    /**
     * Delete object list.
     * @param array $params
     * @return boolean success
     */
    public function delete_list(array $params=array()): bool {
        /* Merge params */
        $params = array_merge($this->get_queryDefaultParams(), $params);

        /* Build where parameters */
        $whereParams = $this->build_queryWhereParams($params);

        /* Build and execute deletion */
        return $this->delete_objectList($whereParams);
    }

    /* Mark: Basic database methods */

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
    public function execute_DBQuery(mixed $query, string $resourceName='Default'): mixed {
        return $this->databaseConnector->execute_query($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute database direct query.
     * @param string $query
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBDirectQuery(string $query, string $resourceName='Default'): mixed {
        return $this->databaseConnector->execute_directQuery($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute prepared query using DatabaseQuery builder.
     * @param DatabaseQuery $query
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBBuiltQuery(DatabaseQuery $query, string $resourceName='Default'): mixed {
        return $this->databaseConnector->execute_builtQuery($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute multiple database queries at once.
     * @param string $queries
     * @param string $resourceName
     * @return mixed
     */
    public function execute_DBMultiQueries(string $queries='', string $resourceName='Default'): mixed {
        return $this->databaseConnector->execute_queries($queries, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryGeneratedId(string $resourceName='Default'): mixed {
        return $this->databaseConnector->get_queryGeneratedId($resourceName);
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryDataLine(string $resourceName='Default'): mixed {
        return $this->databaseConnector->get_queryDataLine($resourceName);
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryNumDataLine(string $resourceName='Default'): mixed {
        return $this->databaseConnector->get_queryNumDataLine($resourceName);
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return mixed
     */
    public function get_DBQueryAffectedRows(string $resourceName='Default'): mixed {
        return $this->databaseConnector->get_queryAffectedRows($resourceName);
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_DBQueryError(string $resourceName='Default'): string {
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

    /* Mark: Attribute management methods */

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
    protected function define_fileAttribute(string $name, string $type='Text', array $params=array(), bool $isLocalised=false): bool {
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

    /* Mark: Advanced database methods */

    /**
     * Load object attributes from database.
     * @param mixed $language
     * @return boolean success
     */
    protected function load_fromDatabase(mixed $language=true): bool {
        if ($this->objectId === 0)
            return false;

        /* Build query for main table */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_whereCondition(new DatabaseQueryWhereCondition('id', $this->objectId));

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return false;

        $data = $this->get_DBQueryDataLine();
        if ($data === false || is_null($data))
            return false;

        /* Populate non-localised database attributes */
        foreach ($this->attributesList['db'] as $name) {
            if (isset($data[$name]))
                $this->libraryAttributes[$name]->load_fromSave($data);
        }

        /* Check for localised attributes */
        if (count($this->attributesList['dbl']) > 0) {
            $query = new DatabaseQuery('select');
            $query->add_table($this->dbLocaleTableName);
            $query->add_whereCondition(new DatabaseQueryWhereCondition('_ObjectId', $this->objectId));

            if (!$this->execute_DBQuery($query))
                return false;

            $localeData = array();
            while ($line = $this->get_DBQueryDataLine()) {
                if (isset($line['_ObjectLanguage'])) {
                    $language = $line['_ObjectLanguage'];
                    foreach ($this->attributesList['dbl'] as $name) {
                        if (isset($line[$name]))
                            $localeData[$name.'_'.$language] = $line[$name];
                    }
                }
            }

            foreach ($this->attributesList['dbl'] as $name) {
                $this->libraryAttributes[$name]->load_fromSave($localeData);
            }
        }

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
     * @return boolean success
     */
    protected function insert_databaseAttributes(): bool {
        global $CaMykS;

        /* Compile data for non-localised attributes */
        $values = array();
        foreach ($this->attributesList['db'] as $attributeName)
            $values[$attributeName] = $this->libraryAttributes[$attributeName]->get_value();

        /* Build query for non-localised attributes */
        $query = new DatabaseQuery('insert');
        $query->add_table($this->dbTableName);
        $query->add_insertValues($values);

        if (!$this->execute_DBQuery($query))
            return false;

        $this->objectId = (int)$this->get_DBQueryGeneratedId();

        /* Check for localised attributes */
        if (count($this->attributesList['dbl']) > 0) {
            $languages = $this->get_configValue('Admin_EditionLanguages', null, array());
            if (empty($languages) && isset($CaMykS))
                $languages = $CaMykS->get_siteLanguages();

            foreach ($languages as $language) {
                $localeValues = array(
                    '_ObjectId' => $this->objectId,
                    '_ObjectLanguage' => $language,
                );
                foreach ($this->attributesList['dbl'] as $attributeName) {
                    $localeValues[$attributeName] = $this->libraryAttributes[$attributeName]->get_value($language);
                }

                $localeQuery = new DatabaseQuery('insert');
                $localeQuery->add_table($this->dbLocaleTableName);
                $localeQuery->add_insertValues($localeValues);
                if (!$this->execute_DBQuery($localeQuery))
                    return false;
            }
        }
        return true;
    }

    /**
     * Update object database attributes.
     * @return boolean success
     */
    protected function update_databaseAttributes(): bool {
        global $CaMykS;

        /* Compile data for non-localised attributes */
        $values = array();
        foreach ($this->attributesList['db'] as $attributeName)
            $values[$attributeName] = $this->libraryAttributes[$attributeName]->get_value();

        /* Build query for non-localised attributes */
        $query = new DatabaseQuery('update');
        $query->add_table($this->dbTableName);
        $query->add_updateAssignmentList($values);
        $query->add_whereCondition(new DatabaseQueryWhereCondition('id', $this->objectId));

        if (!$this->execute_DBQuery($query))
            return false;

        /* Check for localised attributes */
        if (count($this->attributesList['dbl']) > 0) {
            $languages = $this->get_configValue('Admin_EditionLanguages', null, array());
            if (empty($languages) && isset($CaMykS))
                $languages = $CaMykS->get_siteLanguages();

            foreach ($languages as $language) {
                $localeValues = array(
                    '_ObjectId' => $this->objectId,
                    '_ObjectLanguage' => $language,
                );
                foreach ($this->attributesList['dbl'] as $attributeName) {
                    $localeValues[$attributeName] = $this->libraryAttributes[$attributeName]->get_value($language);
                }

                $localeQuery = new DatabaseQuery('replace');
                $localeQuery->add_table($this->dbLocaleTableName);
                $localeQuery->add_insertValues($localeValues);
                if (!$this->execute_DBQuery($localeQuery))
                    return false;
            }
        }
        return true;
    }

    /**
     * Delete object database attributes.
     * @return boolean success
     */
    protected function delete_databaseAttributes(): bool {
        if ($this->objectId === 0)
            return false;

        /* Build query for main table */
        $query = new DatabaseQuery('delete');
        $query->add_table($this->dbTableName);
        $query->add_whereCondition(new DatabaseQueryWhereCondition('id', $this->objectId));

        if (!$this->execute_DBQuery($query))
            return false;

        /* Delete from locale table */
        if (count($this->attributesList['dbl']) > 0) {
            $localeQuery = new DatabaseQuery('delete');
            $localeQuery->add_table($this->dbLocaleTableName);
            $localeQuery->add_whereCondition(new DatabaseQueryWhereCondition('_ObjectId', $this->objectId));

            if (!$this->execute_DBQuery($localeQuery))
                return false;
        }

        return true;
    }

    /**
     * Delete object list (database and files).
     * @param array $whereParams
     * @return boolean success
     */
    protected function delete_objectList(array $whereParams=array()): bool {
        /* Retrieve target object IDs for file cleaning */
        $idList = $this->get_objectIdList(true, $whereParams);

        /* Delete files */
        if (!empty($idList))
            $this->delete_objectFileList($idList);

        /* Delete database records */
        return $this->delete_objectDatabaseList($whereParams);
    }

    /**
     * Delete object list from database.
     * @param array $whereParams
     * @return boolean success
     */
    protected function delete_objectDatabaseList(array $whereParams=array()): bool {
        /* If localized, get IDs for locale table deletion */
        if (count($this->attributesList['dbl']) > 0) {
            $idList = $this->get_objectIdList(true, $whereParams);
            if (!empty($idList)) {
                $localeQuery = new DatabaseQuery('delete');
                $localeQuery->add_table($this->dbLocaleTableName);
                $localeQuery->add_whereCondition(new DatabaseQueryWhereCondition('_ObjectId', $idList, 'in'));
                $this->execute_DBQuery($localeQuery);
            }
        }

        /* Build delete query for main table */
        $query = new DatabaseQuery('delete');
        $query->add_table($this->dbTableName);

        /* Apply where conditions */
        foreach ($whereParams as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        return (bool)$this->execute_DBQuery($query);
    }

    /**
     * Install database attributes.
     * @return boolean success
     */
    protected function install_databaseAttributes(): bool {
        /* Check library has database attributes */
        if (count($this->attributesList['db']) === 0 and count($this->attributesList['dbl']) === 0)
            return true;

        /* Create main table field list */
        $fields = array();
        foreach ($this->attributesList['db'] as $name)
            $fields[$name] = $this->libraryAttributes[$name]->export_databaseParameters();

        /* Create main table */
        if (!$this->create_DBTable($this->dbTableName, $fields, $this->dbTableSettings)) {
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
                return false;
            }
        }

        /* Return success */
        return true;
    }

    /* Mark: Advanced multi object method */

    /**
     * Return object list from database.
     * @param mixed $language
     * @param array $whereParams
     * @param integer $limit
     * @param integer $count
     * @param array $orderBy
     * @return array
     */
    protected function get_objectList(mixed $language=true, array $whereParams=array(), int $limit=-1, int $count=-1, array $orderBy=array('id'=>'asc')): array {
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);

        /* Add where conditions */
        foreach ($whereParams as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        /* Set limit and offset */
        if ($limit > -1 || $count > -1)
            $query->set_limit($count > 0 ? $count : 0, $limit > 0 ? $limit : -1);

        /* Set order by */
        $this->_add_queryOrderBy($query, $orderBy);

        if (!$this->execute_DBQuery($query))
            return array();

        $list = array();
        while ($line = $this->get_DBQueryDataLine()) {
            if (isset($line['id']))
                $list[$line['id']] = $line;
            else
                $list[] = $line;
        }

        return $list;
    }

    /**
     * Return object count from database.
     * @param array $whereParams
     * @return integer
     */
    protected function get_objectCount(array $whereParams=array()): int {
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('id', 'total', '', 'count');

        foreach ($whereParams as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        if (!$this->execute_DBQuery($query))
            return 0;

        $data = $this->get_DBQueryDataLine();
        return isset($data['total']) ? (int)$data['total'] : 0;
    }

    /**
     * Return object id list from database.
     * @param mixed $language
     * @param array $whereParams
     * @param integer $limit
     * @param integer $count
     * @param array $orderBy
     * @return array
     */
    protected function get_objectIdList(mixed $language=true, array $whereParams=array(), int $limit=-1, int $count=-1, array $orderBy=array('id'=>'asc')): array {
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('id');

        foreach ($whereParams as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        if ($limit > -1 || $count > -1)
            $query->set_limit($count > 0 ? $count : 0, $limit > 0 ? $limit : -1);

        $this->_add_queryOrderBy($query, $orderBy);

        if (!$this->execute_DBQuery($query))
            return array();

        $idList = array();
        while ($line = $this->get_DBQueryDataLine()) {
            if (isset($line['id']))
                $idList[] = (int)$line['id'];
        }

        return $idList;
    }

    /**
     * Return object attribute list from database.
     * @param string $attributeName
     * @param mixed $language
     * @param array $whereParams
     * @param integer $limit
     * @param integer $count
     * @param array $orderBy
     * @return array
     */
    protected function get_objectAttributeList(string $attributeName, mixed $language=true, array $whereParams=array(), int $limit=-1, int $count=-1, array $orderBy=array('id'=>'asc')): array {
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_selectItem('id');
        $query->add_selectItem($attributeName);

        foreach ($whereParams as $condition) {
            if ($condition instanceof DatabaseQueryWhereCondition)
                $query->add_whereCondition($condition);
        }

        if ($limit > -1 || $count > -1)
            $query->set_limit($count > 0 ? $count : 0, $limit > 0 ? $limit : -1);

        $this->_add_queryOrderBy($query, $orderBy);

        if (!$this->execute_DBQuery($query))
            return array();

        $list = array();
        while ($line = $this->get_DBQueryDataLine()) {
            if (isset($line['id']) && isset($line[$attributeName]))
                $list[$line['id']] = $line[$attributeName];
        }

        return $list;
    }

    /**
     * Add order by conditions to query, qualifying table names when omitted.
     * @param DatabaseQuery $query
     * @param array $orderBy
     * @return void
     */
    protected function _add_queryOrderBy(DatabaseQuery &$query, array $orderBy=array()): void {
        foreach ($orderBy as $field => $dir) {
            /* Case 1: Structured order element array */
            if (is_int($field) && is_array($dir)) {
                $col = isset($dir['Column']) ? $dir['Column'] : (isset($dir['value']) ? $dir['value'] : '');
                $way = isset($dir['Way']) ? $dir['Way'] : (isset($dir['way']) ? $dir['way'] : 'asc');
                $tbl = isset($dir['Table']) ? $dir['Table'] : (isset($dir['table']) ? $dir['table'] : '');
                $type = isset($dir['Type']) ? $dir['Type'] : (isset($dir['type']) ? $dir['type'] : 'column');
                $vals = isset($dir['Values']) ? $dir['Values'] : (isset($dir['values']) ? $dir['values'] : array());
                $nulls = isset($dir['Nulls']) ? $dir['Nulls'] : (isset($dir['nulls']) ? $dir['nulls'] : null);

                if ($tbl === '' && $col !== '' && strpos($col, '.') === false)
                    $tbl = $this->_get_attributeTableName($col);

                $query->add_orderBy($col, $way, $tbl, $type, $vals, $nulls);
                continue;
            }

            /* Case 2: Unindexed column list array('title', 'date') */
            if (is_int($field) && is_string($dir)) {
                $field = $dir;
                $dir = 'asc';
            }

            /* Case 3: Field value ordering array('status' => array('draft', 'published')) */
            if (is_array($dir)) {
                $tbl = '';
                $col = (string)$field;
                if (strpos($col, '.') !== false) {
                    $parts = explode('.', $col, 2);
                    $tbl = $parts[0];
                    $col = $parts[1];
                } else {
                    $tbl = $this->_get_attributeTableName($col);
                }
                $query->add_orderByField($col, $dir, 'asc', $tbl);
                continue;
            }

            /* Case 4: Standard column ordering array('title' => 'asc') */
            $col = (string)$field;
            $tbl = '';
            if (strpos($col, '.') !== false) {
                $parts = explode('.', $col, 2);
                $tbl = $parts[0];
                $col = $parts[1];
            } else {
                $tbl = $this->_get_attributeTableName($col);
            }

            $query->add_orderByColumn($col, (string)$dir, $tbl);
        }
    }

    /**
     * Return database table name for a given attribute name.
     * @param string $attributeName
     * @return string
     */
    protected function _get_attributeTableName(string $attributeName): string {
        if (!isset($this->dbTableName))
            return '';
        if (isset($this->attributesList['dbl']) && in_array($attributeName, $this->attributesList['dbl']) && isset($this->dbLocaleTableName))
            return $this->dbLocaleTableName;
        return $this->dbTableName;
    }

    /**
     * Update single database attribute directly.
     * @param string $attributeName
     * @param mixed $value
     * @param integer $id
     * @param mixed $language
     * @return boolean success
     */
    protected function update_databaseAttribute(string $attributeName, mixed $value, int $id=0, mixed $language=null): bool {
        return $this->update_databaseAttributeList(array($attributeName => $value), $id, $language);
    }

    /**
     * Update multiple database attributes directly.
     * @param array $values
     * @param integer $id
     * @param mixed $language
     * @return boolean success
     */
    protected function update_databaseAttributeList(array $values, int $id=0, mixed $language=null): bool {
        if ($id === 0)
            $id = $this->objectId;
        if ($id === 0)
            return false;

        $dbValues = array();
        $dblValues = array();

        foreach ($values as $attr => $val) {
            if (in_array($attr, $this->attributesList['db']))
                $dbValues[$attr] = $val;
            elseif (in_array($attr, $this->attributesList['dbl']))
                $dblValues[$attr] = $val;
        }

        if (!empty($dbValues)) {
            $query = new DatabaseQuery('update');
            $query->add_table($this->dbTableName);
            $query->add_updateAssignmentList($dbValues);
            $query->add_whereCondition(new DatabaseQueryWhereCondition('id', $id));
            if (!$this->execute_DBQuery($query))
                return false;
        }

        if (!empty($dblValues)) {
            $localeQuery = new DatabaseQuery('update');
            $localeQuery->add_table($this->dbLocaleTableName);
            $localeQuery->add_updateAssignmentList($dblValues);
            $localeQuery->add_whereCondition(new DatabaseQueryWhereCondition('_ObjectId', $id));
            if (!is_null($language))
                $localeQuery->add_whereCondition(new DatabaseQueryWhereCondition('_ObjectLanguage', $language));
            if (!$this->execute_DBQuery($localeQuery))
                return false;
        }

        return true;
    }

    /* Mark: Advanced file methods */

    /**
     * Load object file attributes.
     * @param mixed $language
     * @return boolean success
     */
    protected function load_files(mixed $language=true): bool {
        if (count($this->attributesList['file']) === 0 && count($this->attributesList['filel']) === 0)
            return true;

        /* Load non-localised files */
        foreach ($this->attributesList['file'] as $name) {
            $path = $this->get_filePath($name);
            if ($path !== false && file_exists($path)) {
                $this->libraryAttributes[$name]->set_value(basename($path));
            }
        }

        /* Load localised files */
        if (count($this->attributesList['filel']) > 0) {
            global $CaMykS;
            $languages = $this->get_configValue('Admin_EditionLanguages', null, array());
            if (empty($languages) && isset($CaMykS))
                $languages = $CaMykS->get_siteLanguages();

            foreach ($languages as $lang) {
                foreach ($this->attributesList['filel'] as $name) {
                    $path = $this->get_filePath($name, $lang);
                    if ($path !== false && file_exists($path)) {
                        $this->libraryAttributes[$name]->set_value(basename($path), $lang);
                    }
                }
            }
        }

        return true;
    }

    /**
     * Save object file attributes.
     * @return boolean success
     */
    protected function save_fileAttributes(): bool {
        if (count($this->attributesList['file']) === 0 && count($this->attributesList['filel']) === 0)
            return true;

        $folder = $this->get_fileFolder();
        if (!folder_create($folder))
            return false;

        return true;
    }

    /**
     * Delete object file attributes.
     * @return boolean success
     */
    protected function delete_fileAttributes(): bool {
        if ($this->objectId === 0)
            return false;

        $folder = $this->get_fileFolder();
        if (file_exists($folder))
            return folder_delete($folder);

        return true;
    }

    /**
     * Delete file attributes for a list of object IDs.
     * @param array $idList
     * @return boolean success
     */
    protected function delete_objectFileList(array $idList=array()): bool {
        $result = true;
        foreach ($idList as $id) {
            $instance = $this->get_newInstance((int)$id);
            if (!$instance->delete_fileAttributes())
                $result = false;
        }
        return $result;
    }

    /**
     * Check file attributes conformity.
     * @return boolean result
     */
    protected function check_fileAttributes(): bool {
        return true;
    }

    /**
     * Return file server path for an attribute.
     * @param string $attributeName
     * @param mixed $language
     * @return string|false
     */
    public function get_filePath(string $attributeName, mixed $language=null): string|false {
        if (!isset($this->libraryAttributes[$attributeName]))
            return false;

        $fileName = $this->libraryAttributes[$attributeName]->get_value($language);
        if (empty($fileName))
            return false;

        return $this->get_fileFolder().'/'.$fileName;
    }

    /**
     * Return file public URL for an attribute.
     * @param string $attributeName
     * @param mixed $language
     * @return string|false
     */
    public function get_fileURL(string $attributeName, mixed $language=null): string|false {
        $path = $this->get_filePath($attributeName, $language);
        if ($path === false)
            return false;

        global $CaMykS;
        if (isset($CaMykS))
            return $CaMykS->get_siteURL().str_replace($CaMykS->get_sitePath(), '', $path);

        return $path;
    }

    /**
     * Return file storage folder for object instance.
     * @return string
     */
    public function get_fileFolder(): string {
        global $CaMykS;
        $base = isset($CaMykS) ? $CaMykS->get_sitePath().'/Data/Modules/' : '/tmp/CaMykS2/Data/Modules/';
        return $base.$this->libraryParent->libraryName.'/'.$this->libraryName;
    }

    /**
     * Install file attributes.
     * @return boolean success
     */
    protected function install_fileAttributes(): bool {
        return true;
    }

    /* Mark: Advanced temporary methods */

    /**
     * Load temporary attributes.
     * @return boolean success
     */
    protected function load_temporary(): bool {
        return true;
    }

    /**
     * Save temporary attributes.
     * @return boolean success
     */
    protected function save_temporary(): bool {
        return true;
    }
}
?>
