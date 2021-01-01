<?php
/**
 * @brief LocalStorage object.
 * <br />Manages local files storage, using file system or database.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Dec 2020
 * @date Modification: Dec 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * LocalStorage class.
 */
final class LocalStorage extends CaMykSLibrary {
	/**
     * @var string $storageMode
     * @brief Save storage mode.
     */
    private $storageMode = null;
    
    /**
     * @var Object $storageManager
     * @brief Link to database storage manager.
     */
    private $storageManager = null;
    
    /**
     * @var string $storageFolder
     * @brief File system folder
     */
    private $storageFolder = 'data';

    /**
     * Class constructor.
     * @return void
     */
    public function __construct() {
        /* Execute parent construct */
        parent::__construct();
    }
    
    /* Documents related methods */
    
    /**
     * Return documents URL.
     * @param string $plugin
     * @param string $path
     * @param array $files
     * @param string $URLMode
     * @return mixed
     */
    public function get_documentsURL($plugin, $path, $files=array(), $URLMode=null) {
    	
    }
    
    /**
     * Save documents.
     * @param string $plugin
     * @param string $path
     * @param array $files
     * @return mixed
     */
    public function save_uploadedDocuments($plugin, $path, $files=array()) {
    
    }
    
    /* Configuration related methods */
    
    /**
     * Return configuration data.
     * @param string $plugin
     * @param string $type
     * @param string $name
     * @return array
     */
    public function get_configuration($plugin, $type, $name) {
    	$this->load_storageMode();
    	
    	if ($this->storageMode === 'database') {
    		/* Load data from database */
    		$data = $this->storageManager->get_configuration($plugin, $type, $name);
    	} else {
    		/* Load data from files */
			$data = $this->get_configurationFile($plugin, $type, $name);
    	}
    	
    	/* Check data */
    	if ($data === false)
    		return array();
    		
    	/* Return decoded JSON data */
    	return json_decode($data);
    }
    
    /**
     * Save configuration data.
     * @param string $plugin
     * @param string $type
     * @param string $name
     * @param array $data
     * @return boolean success
     */
    public function save_configuration($plugin, $type, $name, $data) {
    	$this->load_storageMode();
    	
    	/* Encode data */
    	$data = json_encode($data);
    	
    	if ($this->storageMode === 'database') {
    		/* Save data to database */
    		return $this->storageManager->save_configuration($plugin, $type, $name, $data);
    	} else {
    		/* Save data to files */
    		return $this->save_configurationFile($plugin, $type, $name, $data);
    	}
    }
     
    /* Event methods */
    
    /**
     * On configuration switch from files to database.
     * @return boolean result
     */
    public function switch_dataFromFilesToDatabase() {
    	/* Load storage manager */
    	$this->load_storageManager();
    	
    	/* Check storage manager */
    	if ($this->storageManager === false)
    		return false;
    	
    	/* Inform storage manager of the change */
    	$this->storageManager->enable_databaseStorage();
    	
    	/* Read all file system files and send them to database */
    	/* @todo */
    }
    
    /**
     * On configuration switch from database to files.
     * @return boolean result
     */
    public function switch_dataFromDatabaseToFiles() {
    	$this->load_storageManager();
    	
    	/* Check storage manager */
    	if ($this->storageManager === false)
    		return false;
    	
    	/* Inform storage manager of the change */
    	$this->storageManager->disable_databaseStorage();
    	
    	/* Read all database saved files and store them in file system */
    	/* @todo */
    }
    
    /* File system related methods */
    
    /**
     * Return configuration data from file.
     * @param string $plugin
     * @param string $type
     * @param string $name
     * @return array
     */
    private function get_configurationFile($plugin, $type, $name) {
    	$file = new File($this->storageFolder.'/'.$plugin.'/'.$type, $name.'.json.inc');
    	return $file->get_contents();
    }
    
    /**
     * Save configuration data in file.
     * @param string $plugin
     * @param string $type
     * @param string $name
     * @param array $data
     * @return boolean success
     */
    private function save_configurationFile($plugin, $type, $name, $data) {
    	$file = new File($this->storageFolder.'/'.$plugin.'/'.$type, $name.'.json.inc', 'Write');
    	return $file->write($data);
    }
    
    /* Private specific tool methods */
    
    /**
     * Load storage mode.
     * @return void
     */
    private function load_storageMode() {
    	global $CaMykS;
    	
    	/* Check is already loaded */
    	if (!is_null($this->storageMode))
    		return;
    	
    	/* Load storage mode from website configuration */
    	$this->storageMode = $CaMykS->get_configValue('Website_LocalStorage');
    	
    	/* Check database storage is available */
    	if ($CaMykS->get_configValue('Website_LocalStorage') === 'database') {
    		$this->load_storageManager();
    		if ($this->storageManager === false)
    			$this->storageMode = 'files';
    	}
    }
    
    /**
     * Load storage manager module.
     * @return void
     */
    private function load_storageManager() {
    	$this->storageManager = $CaMykS->get_module('Tool/SystemFileStorageManager');
    }
}
?>