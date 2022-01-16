<?php
/**
 * @brief ModuleLibrary object.
 * <br />Abstract library to create module objects to manage data.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Jan 2021
 * @date Modification: Jan 2022
 * @copyright 2021 - 2022 CaMykS Team
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
    protected $objectId;

    /**
     * @var string $databaseSource
     * @brief Name of database source.
     */
    protected $databaseSource = 'Main';

    /**
     * @var Object $databaseConnector.
     * @brief Link to database connector.
     */
    protected $databaseConnector;

    /**
     * @var string $dbTableName
     * @brief Built main data table name in database.
     */
    protected $dbTableName;

    /**
     * @var string $dbLocaleTableName
     * @brief Built localised data table name in database.
     */
    protected $dbLocaleTableName;

    /**
     * @var array $dbTableSettings
     * @brief Settings for main table creation in database.
     */
    protected $dbTableSettings = array();

    /**
     * @var array $dbLocaleTableSettings
     * @brief Settings for locale table creation in database.
     */
    protected $dbLocaleTableSettings = array();

    /**
     * @var array $attributesList
     * @brief List off all attributes names in their sources.
     */
    protected $attributesList = array(
        'db'    => array(),
        'dbl'   => array(),
        'file'  => array(),
        'filel' => array(),
        'temp'  => array(),
        'templ' => array(),
    );

    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        global $CaMykS;
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define library variables */
        $this->objectId = $id;

        /* Load database connection */
        $this->databaseConnector = &$CaMykS->get_databaseConnection($this->databaseSource);

        /* Set up database tables */
        $this->dbTableName = $this->databaseConnector->get_tablePrefix().$this->libraryName;
        $this->dbLocaleTableName = $this->dbTableName.'Locale';
    }

    /* Data management methods */

    /**
     * Load object.
     * @param mixed $language
     * @return boolean success
     */
    public function load($language=true) {
        if ($this->objectId === 0)
            return $this->initialise($language);
        return $this->load_fromDatabase($language) and $this->load_files($language) and $this->load_temporary();
    }

    /**
     * Save object.
     * @return boolean success
     */
    public function save() {
        return true;
    }

    /**
     * Delete object.
     * @return boolean success
     */
    public function delete() {
        return true;
    }

    /**
     * Install object.
     * @return boolean success
     */
    public function install() {
        return $this->install_databaseAttributes() and $this->install_fileAttributes();
    }

    /**
     * Uninstall object.
     * @return boolean success.
     */
    public function uninstall() {

    }

    /**
     * Return a new instance of the object.
     * @param integer $objectId
     * @return Object
     */
    public function get_newInstance($objectId) {
        $class = $this->libraryNamespace.'\\'.$this->libraryName;
        return new $class($this->libraryParent, $objectId);
    }

    /* Data management sub methods */

    /**
     * Check object data.
     * @param boolean $loadPreviousVersion
     * @return boolean result
     */
    public function check_data($loadPreviousVersion=false) {
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

    /* Attributes related methods */

    /**
     * Define database attribute.
     * @param string $name
     * @param string $type
     * @param mixed $default
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_databaseAttribute($name, $type, $default=null, $params=array(), $isLocalised=false) {
        /* Define attribute */
        if (!$this->define_attribute($name, $type, $default, $params, $isLocalised))
            return;

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['dbl'][] = $name;
        else
            $this->attributesList['db'][] = $name;
    }

    /**
     * Define file attribute.
     * @param string $name
     * @param string $type
     * @param array $params
     * @param boolean $isLocalised
     * @return boolean success
     */
    protected function define_fileAttribute($name, $type='File', $params=array(), $isLocalised=false) {
        /* Define attribute */
        if (!$this->define_attribute($name, $type, null, $params, $isLocalised))
            return;

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['filel'][] = $name;
        else
            $this->attributesList['file'][] = $name;
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
    protected function define_temporaryAttribute($name, $type, $default=null, $params=array(), $isLocalised=false) {
        /* Define attribute */
        if (!$this->define_attribute($name, $type, $default, $params, $isLocalised))
            return;

        /* Store attribute name */
        if ($isLocalised)
            $this->attributesList['templ'][] = $name;
        else
            $this->attributesList['temp'][] = $name;
    }

    /**
     * Install database attributes.
     * @return boolean success
     */
    protected function install_databaseAttributes() {
        /* Check library has database attributes */
        if (count($this->attributesList['db']) === 0 and $this->attributesList['dbl'] === 0)
            return true;

        /* Create main table */
        $fields = array();
        foreach ($this->attributesList['db'] as $name)
            $fields[$name] = $this->libraryAttributes[$name]->export_databaseParameters();
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
            foreach ($this->attributesList['dbl'] as $name)
                $fields[$name] = $this->libraryAttributes[$name]->export_databaseParameters();
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
    protected function install_fileAttributes() {
        return true;
    }

    /* Multi object management methods */


    /* Database methods */

    /**
     * Send a database error to CaMykS.
     * @param string $error
     * @return false
     */
    protected function add_databaseError($error) {
        global $CaMykS;

        /* Send error to CaMykS */
        $CaMykS->trigger_databaseError($this->libraryName, 0, $this->get_errorCode(0), $error, $file, $line);

        /* Return false */
        return false;
    }

    /**
     * Execute database query.
     * @param mixed $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_DBQuery($query, $resourceName=null) {
        return $this->databaseConnector->execute_query($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute database direct query.
     * @param string $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_DBDirectQuery($query, $resourceName=null) {
        return $this->databaseConnector->execute_directQuery($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute prepared query using DbQuery builder.
     * @param Object $query
     * @param string $resourceName
     * @return boolean success
     */
    public function execute_DBBuiltQuery($query, $resourceName=null) {
        return $this->databaseConnector->execute_builtQuery($query, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Execute multiple database queries at once.
     * @param string $queries
     * @param string $resourceName
     */
    public function execute_DBMultiQueries($queries='', $resourceName=null) {
        return $this->databaseConnector->execute_queries($queries, $resourceName) or $this->add_databaseError($this->get_DBQueryError($resourceName));
    }

    /**
     * Return id generated by last query.
     * @param string $resourceName
     * @return integer
     */
    public function get_DBQueryGeneratedId($resourceName=null) {
        return $this->databaseConnector->get_queryGeneratedId($resourceName);
    }

    /**
     * Return next query data line.
     * @param string $resourceName
     * @return integer
     */
    public function get_DBQueryDataLine($resourceName=null) {
        return $this->databaseConnector->get_queryDataLine($resourceName);
    }

    /**
     * Return next query data line with numeric index.
     * @param string $resourceName
     * @return integer
     */
    public function get_DBQueryNumDataLine($resourceName=null) {
        return $this->databaseConnector->get_queryNumDataLine($resourceName);
    }

    /**
     * Return latest query affected rows.
     * @param string $resourceName
     * @return integer
     */
    public function get_DBQueryAffectedRows($resourceName=null) {
        return $this->databaseConnector->get_queryAffectedRows($resourceName);
    }

    /**
     * Return latest query error.
     * @param string $resourceName
     * @return string
     */
    public function get_DBQueryError($resourceName=null) {
        return $this->databaseConnector->get_queryError($resourceName);
    }

    /**
     * Create a table in database.
     * @param string $name
     * @param array $fields
     * @param array $params
     * @return boolean success
     */
    public function create_DBTable($name, $fields, $params=array()) {
        return $this->databaseConnector->create_table($name, $fields, $params);
    }

    /**
     * Return espaced string.
     * @param string $string
     * @return string
     */
    public function escape_string($string) {
        return $this->databaseConnector->escape_string($string);
    }
}
?>
