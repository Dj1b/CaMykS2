<?php
/**
 * @brief Encryption library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre6
 * @date Creation: Oct 2019
 * @date Modification: Feb 2021
 * @copyright 2019 - 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @warning This library is not loaded with CaMykS initialisation.
 */

namespace CaMykS2;

/**
 * Cryptographer class.
 */
final class Cryptographer extends CaMykSLibrary {
    /**
     * @var array $engines
     * @brief Available engines.
     */
    private $engines = array('OpenSSL', 'Mcrypt', 'Sodium');

    /**
     * @var array $params
     * @brief Engine parameters.
     */
    protected $params = array(
        'engine'    => 'OpenSSL',
        'cipher'    => 'aes-256-cbc',
        'mode'      => '',              // specific to Mcrypt
        'key'       => '',
        'iv'        => '',
    );

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct($params=array()) {
        parent::__construct($params);
    }

    /* Override CaMykSLibrary methods */

    /**
     * Define error code list.
     * @return void
     */
    protected function define_errorCodes() {
        $this->errorCodes = array(
            /* Engine errors */
            1 => 'UnknownEngine',
            2 => 'EngineNotAvailable',

            /* Cipher errors */
            11 => 'UnknownCipher',

            /* Key / iv errors */
            21 => 'KeyIsEmpty',
            22 => 'IvIsEmpty',

            /* Result errors */
            31 => 'ResultAltered',

            /* Mcrypt specific errors */
            101 => 'UnknownMCryptMode',
        );
    }

    /* Specific public methods */

    /**
     * Check engine availability.
     * @return boolean success
     */
    public function check_engineAvailability() {
        switch($this->params['engine']) {
            case 'OpenSSL': return $this->_OpenSSL_check_engineAvailability();
            case 'Mcrypt': return $this->_Mcrypt_check_engineAvailability();
            case 'Sodium': return $this->_Sodium_check_engineAvailability();
        }

        /* No known engine defined */
        return $this->add_error(1, $this->params['engine'], __FILE__, __LINE__);
    }

    /**
     * Check encryption configuration.
     * @return boolean success
     */
    public function check_encryptionConfiguration() {
        switch($this->params['engine']) {
            case 'OpenSSL': return $this->_OpenSSL_check_encryptionConfiguration();
            case 'Mcrypt': return $this->_Mcrypt_check_encryptionConfiguration();
            case 'Sodium': return $this->_Sodium_check_encryptionConfiguration();
        }
    }

    /**
     * Encrypt data.
     * @param string $data
     * @param integer $base64Encode
     * @return false on failure, string on success
     */
    public function encrypt($data, $base64Encode=true) {
        if (!$this->check_engineAvailability())
            return false;

        if (!$this->check_encryptionConfiguration())
            return false;

        switch($this->params['engine']) {
            case 'OpenSSL': $result = $this->_OpenSSL_encrypt($data); break;
            case 'Mcrypt': $result = $this->_Mcrypt_encrypt($data); break;
            case 'Sodium': $result = $this->_Sodium_encrypt($data); break;
        }

        if ($result === false)
            return false;
        if ($base64Encode)
            return base64_encode($result);
        return $result;
    }

    /**
     * Decrypt data.
     * @param string $data
     * @param integer $base64Decode
     * @return false on failure, string on success
     */
    public function decrypt($data, $base64Decode=true) {
        if (!$this->check_engineAvailability())
            return false;

        if (!$this->check_encryptionConfiguration())
            return false;

        if ($base64Decode)
            $data = base64_decode($data);

        switch($this->params['engine']) {
            case 'OpenSSL': return $this->_OpenSSL_decrypt($data);
            case 'Mcrypt': return $this->_Mcrypt_decrypt($data);
            case 'Sodium': return $this->_Sodium_decrypt($data);
        }

        return false;
    }

    /**
     * Return unique Id.
     * @param int $size
     * @return string
     */
    public function get_uniqueId($size=16) {
        if (!$this->check_engineAvailability())
            return false;

        switch($this->params['engine']) {
            case 'OpenSSL': return $this->_OpenSSL_getUniqueId($size);
            case 'Mcrypt': return $this->_Mcrypt_getUniqueId($size);
            case 'Sodium': return $this->_Sodium_getUniqueId($size);
        }

        return false;
    }

    /* Private OpenSSL methods */

    /**
     * Check OpenSSL availability.
     * @return boolean result
     */
    private function _OpenSSL_check_engineAvailability() {
        if (!extension_loaded('openssl'))
            return $this->add_error(2, 'OpenSSL', __FILE__, __LINE__);

        return true;
    }

    /**
     * Check OpenSSL encryption configuration.
     * @return boolean result
     */
    private function _OpenSSL_check_encryptionConfiguration() {
        if (!in_array($this->params['cipher'], openssl_get_cipher_methods()))
            return $this->add_error(11, $this->params['cipher'], __FILE__, __LINE__);

        if ($this->params['key'] == '')
            return $this->add_error(21, '', __FILE__, __LINE__);

        if ($this->params['iv'] == '')
            return $this->add_error(22, '', __FILE__, __LINE__);

        return true;
    }

    /**
     * Encrypt data using OpenSSL engine.
     * @param string $data
     * @return string
     */
    private function _OpenSSL_encrypt($data) {
        /* Compute key and iv */
        $key = hash('sha256', $this->params['key']);
        $iv = substr(hash('sha256', $this->params['iv']), 0, openssl_cipher_iv_length($this->params['cipher']));

        /* Return encrypted data */
        return openssl_encrypt($data, $this->params['cipher'], $key, 0, $iv);
    }

    /**
     * Decrypt data using OpenSSL engine.
     * @param string $data.
     * @return string
     */
    private function _OpenSSL_decrypt($data) {
        /* Compute key and iv */
        $key = hash('sha256', $this->params['key']);
        $iv = substr(hash('sha256', $this->params['iv']), 0, openssl_cipher_iv_length($this->params['cipher']));

        /* Return decrypted data */
        return openssl_decrypt($data,$this->params['cipher'], $key, 0, $iv);
    }

    /**
     * Return unique Id using OpenSSL engine.
     * @param int $size
     * @return string
     */
    private function _OpenSSL_getUniqueId($size) {
        return bin2hex(openssl_random_pseudo_bytes((int)($size/2)));
    }

    /* Private Mcrypt methods */

    /**
     * Check Mcrypt availability.
     * @return boolean result
     */
    private function _Mcrypt_check_engineAvailability() {
        if (!extension_loaded('mcrypt'))
            return $this->add_error(2, 'MCrypt', __FILE__, __LINE__);

        return true;
    }

    /**
     * Check Mcrypt encryption configuration.
     * @return boolean result
     */
    private function _Mcrypt_check_encryptionConfiguration() {
        if (!in_array($this->params['cipher'], mcrypt_list_algorithms()))
            return $this->add_error(11, $this->params['cipher'], __FILE__, __LINE__);

        if (!in_array($this->params['mode'], mcrypt_list_modes()))
            return $this->add_error(101, $this->params['mode'], __FILE__, __LINE__);

        if ($this->params['key'] == '')
            return $this->add_error(21, '', __FILE__, __LINE__);

        return true;
    }

    /**
     * Encrypt data using Mcrypt engine.
     * @param string $data
     * @return string
     */
    private function _Mcrypt_encrypt($data) {
        /* Compute key and iv */
        $keyHash = hash('sha256', $this->params['key']);
        $key = substr($keyHash, 0,   mcrypt_get_key_size($this->params['cipher'], $this->params['mode']));
        $iv  = substr($keyHash, 0, mcrypt_get_block_size($this->params['cipher'], $this->params['mode']));

        /* Return encrypted data */
        return mcrypt_encrypt($this->params['cipher'], $key, $data, $this->params['mode'], $iv);
    }

    /**
     * Decrypt data using Mcrypt engine.
     * @param string $data.
     * @return string
     */
    private function _Mcrypt_decrypt($data) {
        /* Compute key and iv */
        $keyHash = hash('sha256', $this->params['key']);
        $key = substr($keyHash, 0,   mcrypt_get_key_size($this->params['cipher'], $this->params['mode']));
        $iv  = substr($keyHash, 0, mcrypt_get_block_size($this->params['cipher'], $this->params['mode']));

        /* Return decrypted data */
        return mcrypt_decrypt($this->params['cipher'], $key, $data, $this->params['mode'], $iv);
    }

    /**
     * Return unique Id using Mcrypt engine.
     * @param int $size
     * @return string
     */
    private function _Mcrypt_getUniqueId($size) {
        return mcrypt_create_iv($size);
    }

    /* Private Sodium methods */

    /**
     * Check Sodium availability.
     * @return boolean result
     */
    private function _Sodium_check_engineAvailability() {
        if (!extension_loaded('sodium'))
            return $this->add_error(2, 'Sodium', __FILE__, __LINE__);

        return true;
    }

    /**
     * Check Sodium encryption configuration.
     * @return boolean result
     */
    private function _Sodium_check_encryptionConfiguration() {
        if ($this->params['key'] == '')
            return $this->add_error(21, '', __FILE__, __LINE__);

        return true;
    }

    /**
     * Encrypt data using Sodium engine.
     * @param string $data
     * @return string
     */
    private function _Sodium_encrypt($data) {
        /* Compute key and iv */
        if ($this->params['iv'] != '') {
            $key = substr(hash('sha256', $this->params['key']), 0, SODIUM_CRYPTO_SECRETBOX_KEYBYTES);
            $iv  = substr(hash('sha256', $this->params['iv']), 0, SODIUM_CRYPTO_SECRETBOX_NONCEBYTES);
        } else {
            $keyHash = hash('sha256', $this->params['key']);
            $key = substr($keyHash, 0, SODIUM_CRYPTO_SECRETBOX_KEYBYTES);
            $iv = substr($keyHash, SODIUM_CRYPTO_SECRETBOX_KEYBYTES, SODIUM_CRYPTO_SECRETBOX_NONCEBYTES);
        }

        /* Get encrypted data */
        return sodium_crypto_secretbox($data, $iv, $key);
    }

    /**
     * Decrypt data using Sodium engine.
     * @param string $data.
     * @return string
     */
    private function _Sodium_decrypt($data) {
        /* Compute key and iv */
        if ($this->params['iv'] != '') {
            $key = substr(hash('sha256', $this->params['key']), 0, SODIUM_CRYPTO_SECRETBOX_KEYBYTES);
            $iv  = substr(hash('sha256', $this->params['iv']), 0, SODIUM_CRYPTO_SECRETBOX_NONCEBYTES);
        } else {
            $keyHash = hash('sha256', $this->params['key']);
            $key = substr($keyHash, 0, SODIUM_CRYPTO_SECRETBOX_KEYBYTES);
            $iv = substr($keyHash, SODIUM_CRYPTO_SECRETBOX_KEYBYTES, SODIUM_CRYPTO_SECRETBOX_NONCEBYTES);
        }

        /* Get decrypted data */
        if (($result = sodium_crypto_secretbox_open($data, $iv, $key)) === false) {
            $this->add_error(31, '', __FILE__, __LINE__);
        }

        /* Return result */
        return $result;
    }

    /**
     * Return unique Id using Sodium engine.
     * @param int $size
     * @return string
     */
    private function _Sodium_getUniqueId($size) {
        /* Define minimum size */
        $tsize = max(16, (int)($size/2));

        /* Return value, troncated if required */
        return substr(sodium_bin2hex(sodium_crypto_pwhash($tsize, md5(microtime()), random_bytes(SODIUM_CRYPTO_PWHASH_SALTBYTES), SODIUM_CRYPTO_PWHASH_OPSLIMIT_INTERACTIVE, SODIUM_CRYPTO_PWHASH_MEMLIMIT_INTERACTIVE)), 0, $size);
    }
}
?>
