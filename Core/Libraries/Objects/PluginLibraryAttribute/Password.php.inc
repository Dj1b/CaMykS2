<?php
/**
 * @brief PasswordPluginLibraryAttribute object.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2026
 * @date Modification: Feb 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * PasswordPluginLibraryAttribute class.
 */
class PasswordPluginLibraryAttribute extends TextPluginLibraryAttribute {

    /**
     * @var constant array DEFAULTPARAMS
     * @brief Attribute default parameters.
     */
    const DEFAULTPARAMS = array('Algorithm'=>'sha256', 'Salt'=>'', 'MinLength'=>8, 'MaxLength'=>32, 'LowerCase'=>0, 'UpperCase'=>0, 'Number'=>0, 'Special'=>0);

    /**
     * @var constant string SPECIAL_CHARS
     * @brief List of authorized special characters.
     */
    const SPECIAL_CHARS = '!@#$%^&*()_+-=[]{}|;:,.<>?';

    /* Mark: Override PluginLibraryAttribute methods */

    /**
     * Check and optimise stored value.
     * @return boolean result
     */
    public function check_value() {
        global $CaMykS;

        /* Load password tool library */
        $CaMykS->load_library('password', 'Tools');

        /* Check empty value (if not mandatory) */
        if ($this->value === '') {
            return true;
        }

        /* Define hash length based on algorithm */
        $hashLength = 0;
        switch ($this->params['Algorithm']) {
            case 'sha512': $hashLength = 128; break;
            case 'sha384': $hashLength = 96; break;
            case 'sha256': $hashLength = 64; break;
            case 'sha1': $hashLength = 40; break;
            case 'md5': $hashLength = 32; break;
        }

        /* Check availability of hash length */
        if ($hashLength == 0)
            return false;

        /* Check if value is already a hash */
        if (strlen($this->value) == $hashLength and preg_match('/^[a-f0-9]+$/', $this->value)) {
            return true;
        }

        /* Check plain text length */
        if (strlen($this->value) < $this->params['MinLength'])
            return false;
        if ($this->params['MaxLength'] > 0 and strlen($this->value) > $this->params['MaxLength'])
            return false;

        /* Check lower case characters */
        if ($this->params['LowerCase'] > 0)
            if (preg_match_all('/[a-z]/', $this->value) < $this->params['LowerCase'])
                return false;

        /* Check upper case characters */
        if ($this->params['UpperCase'] > 0)
            if (preg_match_all('/[A-Z]/', $this->value) < $this->params['UpperCase'])
                return false;

        /* Check number characters */
        if ($this->params['Number'] > 0)
            if (preg_match_all('/[0-9]/', $this->value) < $this->params['Number'])
                return false;

        /* Check special characters */
        if ($this->params['Special'] > 0) {
            $count = 0;
            $len = strlen($this->value);
            for ($i = 0; $i < $len; $i++) {
                if (strpos(self::SPECIAL_CHARS, $this->value[$i]) !== false)
                    $count++;
            }
            if ($count < $this->params['Special'])
                return false;
        }

        /* Encode password */
        $this->value = password_encode($this->value, $this->params['Algorithm'], $this->params['Salt']);

        /* Return value is conform */
        return true;
    }

    /**
     * Return attribute parameters for table creation in database.
     * @return array
     */
    public function export_databaseParameters() {
        /* Define hash length based on algorithm */
        $hashLength = 0;
        switch ($this->params['Algorithm']) {
            case 'sha512': $hashLength = 128; break;
            case 'sha384': $hashLength = 96; break;
            case 'sha256': $hashLength = 64; break;
            case 'sha1': $hashLength = 40; break;
            case 'md5': $hashLength = 32; break;
        }

        return array(
            /* Generic values */
            'Type'      => 'text',
            'Default'   => $this->defaultValue,
            'IsUnique'  => $this->params['IsUnique'],

            /* Text settings */
            'Length'    => $hashLength,

            /* Primary key and index settings */
            'IndexName'     => $this->params['IndexName'],
            'IndexLength'   => $this->params['IndexLength'],
        );
    }

    /* Mark: Specific methods */

    /**
     * Return safe list of authorised special characters.
     * @return string
     */
    public function get_specialCharactersList() {
        return self::SPECIAL_CHARS;
    }
}
?>
