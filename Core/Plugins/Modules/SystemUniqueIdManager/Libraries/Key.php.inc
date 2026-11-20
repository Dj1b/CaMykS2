<?php
/**
 * @brief SystemUniqueIdManager admin module, key library.
 * @details Plugin / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Feb 2022
 * @date Modification: Nov 2026
 * @copyright 2022 - 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemUniqueIdManager;

use CaMykS2\Module;
use CaMykS2\SimpleModuleLibrary;
use CaMykS2\DatabaseQuery;
use CaMykS2\DatabaseQueryWhereCondition;

/**
 * Key class.
 */
final class Key extends SimpleModuleLibrary {
    /**
     * Class constructor.
     * @param Module $parent
     * @param integer $id
     * @return void
     */
    public function __construct(Module &$parent, int $id=0) {
        /* Execute parent construct */
        parent::__construct($parent, $id);

        /* Define management attributes */
        $this->define_databaseAttribute('CreationDate', 'Date',         true);
        $this->define_databaseAttribute('UniqueId',     'Text',         '', array('Length'=>36));
        $this->define_databaseAttribute('Plugin',       'Text',         '', array('Length'=>48));
        $this->define_databaseAttribute('Object',       'Text',         '', array('Length'=>48));
        $this->define_databaseAttribute('ObjectId',     'Integer',      0, array('Unsigned'=>true));
    }

    /* Override PluginLibrary method */
    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes(): void {
        parent::define_errorCodes();

        $this->errorCodes = array_merge($this->errorCodes, array(
            /* Key related errors */
            11 => 'UniqueIdGeneratingMaxAttemptsExceeded',
        ));
    }

    /* Override ModuleLibrary method */

    /**
     * Check object data
     * @param boolean $loadPreviousVersion
     * @return boolean success
     */
    public function check_data(bool $loadPreviousVersion=false): bool {
        /* Perform generic check_data method */
        if (parent::check_data(false) === false)
            return false;

        /* Check for unique id generating */
        if ($this->UniqueId === '')
            $this->UniqueId = (string)$this->generate_uniqueIdValue();

        /* Return success */
        return true;
    }

    /* Override SimpleModuleLibrary methods */

    /**
     * Return status values.
     * @return array
     */
    public function get_statusValueList(): array {
        return array(
            1 => $this->get_locale('KeyStatus_Free'),
            2 => $this->get_locale('KeyStatus_Used'),
        );
    }

    /**
     * Return multi object status values.
     * @param array $params
     * @return array
     */
    public function get_multiStatusValueList(array $params=array()): array {
        /* Check params */
        $params = array_merge(array('AddAll'=>false), $params);

        /* Initialise list */
        $list = array();

        /* Check for none value */
        if ($params['AddAll'])
            $list[0] = $this->get_locale('Keys_All');

        /* Insert values */
        $list[1] = $this->get_locale('KeysStatus_Free');
        $list[2] = $this->get_locale('KeysStatus_Used');

        /* Return list */
        return $list;
    }

    /* Specific methods */

    /**
     * Load free item from database.
     * @return boolean success
     */
    public function load_freeItem(): bool {
        /* Build query */
        $query = new DatabaseQuery('select');
        $query->add_table($this->dbTableName);
        $query->add_whereCondition(new DatabaseQueryWhereCondition('Status', 1));
        $query->set_limit(1, 0);

        /* Execute query */
        if (!$this->execute_DBQuery($query))
            return false;

        /* Check exists */
        $data = $this->get_DBQueryDataLine();
        if ($data === false || is_null($data))
            return false;

        /* Load object */
        $this->objectId = (int)$data['id'];
        $this->load_fromArray($data);

        /* Return success */
        return true;
    }

    /**
     * Generate a new unique id.
     * @return mixed
     */
    public function generate_uniqueIdValue(): mixed {
        /* Initialise attempts count */
        $c = 0;

        /* Check format to use */
        $format = isset($this->params['Format']) ? $this->params['Format'] : $this->get_configValue('Generating_Format');

        /* Check if max attempts limit is exceeded */
        while ($c++ < $this->get_configValue('Generating_MaxAttempts')) {

            /* Compute unique id */
            if ($format === 'UUID')
                $uid = $this->compute_UUIDUniqueId();
            else
                $uid = $this->compute_textUniqueId();

            /* Check if uid exists in database */
            $query = new DatabaseQuery('select');
            $query->add_table($this->dbTableName);
            $query->add_whereCondition(new DatabaseQueryWhereCondition('UniqueId', $uid));

            /* Execute query */
            if (!$this->execute_DBQuery($query))
                return false;

            /* Check exists */
            if (!$this->get_DBQueryDataLine())
                return $uid;

            /* Make a pause to change the microtime value */
            usleep(20);
        }

        /* Attempt limit exceeded */
        $this->add_error(11, 'Max attempts exceeded while generating a unique id.', __FILE__, __LINE__);

        /* Attempts limit exceeded, return failure */
        return false;
    }

    /* Private unique id generation sub methods */

    /**
     * Compute text unique id.
     * @return string
     */
    private function compute_textUniqueId(): string {
        $maxLength = isset($this->params['Length']) ? $this->params['Length'] : $this->get_configValue('Generating_TextLength');
        return substr(str_replace('.', '', uniqid('', true)), 0, (int)$maxLength);
    }

    /**
     * Compute UUID unique id.
     * @return string
     */
    private function compute_UUIDUniqueId(): string {
        /* Compute random bytes */
        if (version_compare(PHP_VERSION, 7, '>='))
            $bytes = random_bytes(16);
        else
            $bytes = openssl_random_pseudo_bytes(16);

        /* Check bytes length */
        if (strlen($bytes) !== 16)
            return false;

        /* Update some bytes to version */
        $bytes[6] = chr(ord($bytes[6]) & 0x0f | 0x40);
        $bytes[8] = chr(ord($bytes[8]) & 0x3f | 0x80);

        /* Return bytes with correct format */
        return vsprintf('%s%s-%s-%s-%s-%s%s%s', str_split(bin2hex($bytes), 4));
    }
}
?>
