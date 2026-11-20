<?php
/**
 * @brief SystemPrivacyPolicyManager admin module, SiteIdentity library.
 * @details Plugin / Module Library / SiteIdentity
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre1
 * @date Creation: Oct 2026
 * @date Modification: Nov 2026
 * @copyright 2026 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2\Plugins\Modules\SystemPrivacyPolicyManager;

use CaMykS2\PluginConfiguration;

/**
 * SiteIdentity class.
 */
final class SiteIdentity extends PluginConfiguration {

    /**
     * Class constructor.
     * @param mixed $parent
     * @param string $name
     * @return void
     */
    public function __construct(&$parent, string $name='site_identity') {
        /* Execute parent construct */
        parent::__construct($parent, $name);

        /* Define Owner attributes */
        $this->define_attribute('Owner_Type',                    'ListItem', 0, array('Values' => array(0, 1, 2, 11)));
        $this->define_attribute('Owner_Address',                 'Text',     '', array('Length' => 255));
        $this->define_attribute('Owner_Phone',                   'Phone',    '', array('Length' => 16));
        $this->define_attribute('Owner_Mobile',                  'Phone',    '', array('Length' => 16));
        $this->define_attribute('Owner_Email',                   'Email',    '', array('Length' => 255));

        /* Define Company / Organization attributes */
        $this->define_attribute('Company_Name',                  'Text',     '', array('Length' => 64));
        $this->define_attribute('Company_LegalForm',             'Text',     '', array('Length' => 32));
        $this->define_attribute('Company_Capital',               'Text',     '', array('Length' => 32));
        $this->define_attribute('Company_VAT',                   'Text',     '', array('Length' => 32));
        $this->define_attribute('Company_Siret',                 'Text',     '', array('Length' => 24));
        $this->define_attribute('Company_RCSNumber',             'Text',     '', array('Length' => 24));
        $this->define_attribute('Company_RCSLocation',           'Text',     '', array('Length' => 64));
        $this->define_attribute('Company_NAF',                   'Text',     '', array('Length' => 24));

        /* Define Manager / Publication Director attributes */
        $this->define_attribute('Manager_User',                  'Text',     '_other', array('Length' => 64));
        $this->define_attribute('Manager_Name',                  'Text',     '', array('Length' => 64));
        $this->define_attribute('Manager_Email',                 'Email',    '', array('Length' => 255));
        $this->define_attribute('Manager_Phone',                 'Phone',    '', array('Length' => 16));
        $this->define_attribute('Manager_Mobile',                'Phone',    '', array('Length' => 16));
        $this->define_attribute('Manager_Address',               'Text',     '', array('Length' => 255));

        /* Define Creator / Agency attributes */
        $this->define_attribute('Creator_User',                  'Text',     '_other', array('Length' => 64));
        $this->define_attribute('Creator_Name',                  'Text',     '', array('Length' => 64));
        $this->define_attribute('Creator_Address',               'Text',     '', array('Length' => 255));
        $this->define_attribute('Creator_Phone',                 'Phone',    '', array('Length' => 16));
        $this->define_attribute('Creator_Mobile',                'Phone',    '', array('Length' => 16));
        $this->define_attribute('Creator_Email',                 'Email',    '', array('Length' => 255));
        $this->define_attribute('Creator_Website',               'URL',      '', array('Length' => 255));

        /* Define Webmaster attributes */
        $this->define_attribute('Webmaster_User',                'Text',     '_other', array('Length' => 64));
        $this->define_attribute('Webmaster_Name',                'Text',     '', array('Length' => 64));
        $this->define_attribute('Webmaster_Company',             'Text',     '', array('Length' => 255));
        $this->define_attribute('Webmaster_Email',               'Email',    '', array('Length' => 255));
        $this->define_attribute('Webmaster_Phone',               'Phone',    '', array('Length' => 16));
        $this->define_attribute('Webmaster_Mobile',              'Phone',    '', array('Length' => 16));

        /* Define DPO / Data Protection Officer attributes */
        $this->define_attribute('DPO_User',                      'Text',     '_other', array('Length' => 64));
        $this->define_attribute('DPO_Name',                      'Text',     '', array('Length' => 64));
        $this->define_attribute('DPO_Company',                   'Text',     '', array('Length' => 255));
        $this->define_attribute('DPO_Email',                     'Email',    '', array('Length' => 255));
        $this->define_attribute('DPO_Phone',                     'Phone',    '', array('Length' => 16));
        $this->define_attribute('DPO_Mobile',                    'Phone',    '', array('Length' => 16));

        /* Define Host attributes */
        $this->define_attribute('Host_Name',                     'Text',     '', array('Length' => 64));
        $this->define_attribute('Host_Address',                  'Text',     '', array('Length' => 255));
        $this->define_attribute('Host_Phone',                    'Phone',    '', array('Length' => 16));
        $this->define_attribute('Host_Mobile',                   'Phone',    '', array('Length' => 16));
        $this->define_attribute('Host_Email',                    'Email',    '', array('Length' => 255));
        $this->define_attribute('Host_Website',                  'URL',      '', array('Length' => 255));

        /* Define Law attributes */
        $this->define_attribute('Law_Tribunal',                  'Text',     '', array('Length' => 64));

        /* Define GDPR specific attributes */
        $this->define_attribute('GDPR_CNILRegistrationNumber',   'Text',     '', array('Length' => 64));
        $this->define_attribute('GDPR_CNILRegistrationDate',     'Date',     0);
        $this->define_attribute('GDPR_HostingLocationArea',      'Text',     '', array('Length' => 64));
        $this->define_attribute('GDPR_HostingLocationCountry',   'Text',     '', array('Length' => 64));
        $this->define_attribute('GDPR_HostingLocationCity',      'Text',     '', array('Length' => 64));
    }

    /* Mark: Helper methods */

    /**
     * Return owner type list.
     * @return array
     */
    public function get_ownerTypeList(): array {
        global $CaMykS;

        return array(
            0  => $CaMykS->get_locale('OwnerType_Company', '', array(), 'Société'),
            1  => $CaMykS->get_locale('OwnerType_Association', '', array(), 'Association'),
            2  => $CaMykS->get_locale('OwnerType_PublicEstablishment', '', array(), 'Établissement public'),
            11 => $CaMykS->get_locale('OwnerType_Individual', '', array(), 'Personne physique'),
        );
    }

    /**
     * Return user selection list for contacts.
     * @return array
     */
    public function get_userSelectionList(): array {
        global $CaMykS;

        $list = array();

        /* 1. Propriétaire entry if defined */
        $ownerTitle = $this->get_resolvedOwnerName();
        $list['_owner'] = $CaMykS->get_locale('UserSelect_Owner', '', array(), 'Propriétaire') . ($ownerTitle !== '' ? ' ('.$ownerTitle.')' : '');

        /* 2. Webmaster entry if defined */
        $webmasterData = $this->get_resolvedWebmasterData();
        $webmasterTitle = isset($webmasterData['Name']) && $webmasterData['Name'] !== '' ? $webmasterData['Name'] : '';
        $list['_webmaster'] = $CaMykS->get_locale('UserSelect_Webmaster', '', array(), 'Webmaster') . ($webmasterTitle !== '' ? ' ('.$webmasterTitle.')' : '');

        /* 3. Site users from SystemAdministratorManager */
        if (isset($CaMykS) && method_exists($CaMykS, 'get_module')) {
            $adminModule = $CaMykS->get_module('SystemAdministratorManager');
            if ($adminModule !== false && method_exists($adminModule, 'get_libraryInstance')) {
                $userLib = $adminModule->get_libraryInstance('User');
                if ($userLib !== false && method_exists($userLib, 'get_objectList')) {
                    $users = $userLib->get_objectList();
                    foreach ($users as $userId => $userObj) {
                        $name = trim(($userObj->FirstName ?? '') . ' ' . ($userObj->LastName ?? ''));
                        if ($name === '')
                            $name = $userObj->Login ?? ('User #'.$userId);
                        $email = $userObj->Email ?? '';
                        $list[(string)$userId] = $name . ($email !== '' ? ' <'.$email.'>' : '');
                    }
                }
            }
        }

        /* 4. Other / Manual entry */
        $list['_other'] = $CaMykS->get_locale('UserSelect_Other', '', array(), 'Autre (saisie manuelle)');

        return $list;
    }

    /**
     * Return resolved owner name (company name or manager name for individual).
     * @return string
     */
    public function get_resolvedOwnerName(): string {
        if ((int)$this->Owner_Type === 11) {
            $manager = $this->get_resolvedManagerData();
            return $manager['Name'];
        }
        return (string)$this->Company_Name;
    }

    /**
     * Return resolved Manager data.
     * @return array
     */
    public function get_resolvedManagerData(): array {
        return $this->resolve_contactData('Manager');
    }

    /**
     * Return resolved Creator data.
     * @return array
     */
    public function get_resolvedCreatorData(): array {
        return $this->resolve_contactData('Creator');
    }

    /**
     * Return resolved Webmaster data.
     * @return array
     */
    public function get_resolvedWebmasterData(): array {
        return $this->resolve_contactData('Webmaster');
    }

    /**
     * Return resolved DPO data.
     * @return array
     */
    public function get_resolvedDPOData(): array {
        return $this->resolve_contactData('DPO');
    }

    /**
     * Internal helper to resolve contact data from user selection or manual fields.
     * @param string $prefix
     * @return array
     */
    private function resolve_contactData(string $prefix): array {
        global $CaMykS;

        $userKeyProp = $prefix . '_User';
        $userKey = $this->$userKeyProp ?? '_other';

        /* If specific user ID selected from SystemAdministratorManager */
        if (is_numeric($userKey) && (int)$userKey > 0 && isset($CaMykS) && method_exists($CaMykS, 'get_module')) {
            $adminModule = $CaMykS->get_module('SystemAdministratorManager');
            if ($adminModule !== false && method_exists($adminModule, 'get_libraryInstance')) {
                $userLib = $adminModule->get_libraryInstance('User', (int)$userKey);
                if ($userLib !== false && method_exists($userLib, 'load') && $userLib->load()) {
                    return array(
                        'Name'    => trim(($userLib->FirstName ?? '') . ' ' . ($userLib->LastName ?? '')),
                        'Email'   => (string)($userLib->Email ?? ''),
                        'Phone'   => (string)($userLib->Phone ?? ''),
                        'Mobile'  => (string)($userLib->Mobile ?? ''),
                        'Address' => (string)($userLib->Address ?? ''),
                        'Company' => (string)($userLib->Company ?? ''),
                    );
                }
            }
        }

        /* Fallback to manual attributes */
        $nameProp    = $prefix . '_Name';
        $emailProp   = $prefix . '_Email';
        $phoneProp   = $prefix . '_Phone';
        $mobileProp  = $prefix . '_Mobile';
        $addressProp = $prefix . '_Address';
        $companyProp = $prefix . '_Company';

        return array(
            'Name'    => isset($this->$nameProp) ? (string)$this->$nameProp : '',
            'Email'   => isset($this->$emailProp) ? (string)$this->$emailProp : '',
            'Phone'   => isset($this->$phoneProp) ? (string)$this->$phoneProp : '',
            'Mobile'  => isset($this->$mobileProp) ? (string)$this->$mobileProp : '',
            'Address' => isset($this->$addressProp) ? (string)$this->$addressProp : '',
            'Company' => isset($this->$companyProp) ? (string)$this->$companyProp : '',
        );
    }
}
?>
