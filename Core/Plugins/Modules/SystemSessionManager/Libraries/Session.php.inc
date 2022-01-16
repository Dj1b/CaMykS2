<?php
/**
 * @brief SystemSessionManager admin module, session library.
 * @details Session / Module Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jan 2022
 * @date Modification: Jan 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemSessionManagerSession class.
 */
final class SystemSessionManagerSession extends ModuleLibrary {
    /**
     * Class constructor.
     * @param Object $parent
     * @param integer $id
     * @return void
     */
    public function __construct(&$parent, $id=0) {
        /* Execute parent construct */
        parent::__construct($parent);

        /* Define management attributes */
        $this->define_databaseAttribute('PHPSessionid',         'UniqueId',     '');
        $this->define_databaseAttribute('CreationDate',         'Date',         true);
        $this->define_databaseAttribute('CloseDate',            'Date',         0);
        $this->define_databaseAttribute('Status',               'Boolean',      false);
        $this->define_databaseAttribute('IPAddress',            'IPAddress',    '');
        $this->define_databaseAttribute('MultiIPAddress',       'Boolean',      0);
        $this->define_databaseAttribute('UserAgent',            'Text',         '', array('Length'=>255));
        $this->define_databaseAttribute('MultiIPUserAgent',     'Boolean',      0);

        /* Define client attributes */
        $this->define_databaseAttribute('Type',                 'ListItem',     '', array('Values'=>array('Visitor', 'Admin', 'Robot', 'Fake', 'Hack', 'Unknown')));
        $this->define_databaseAttribute('Format',               'ListItem',     '', array('Values'=>array('Desktop', 'Tablet', 'Mobile', 'Watch', 'VR', 'Unknown')));

        /* Define visitor/admin device attributes */
        $this->define_databaseAttribute('DeviceType',           'ListItem',     '', array('Values'=>array('Computer', 'Tablet', 'Mobile', 'Watch', 'GameConsole', 'SmartTV', 'Unknown')));
        $this->define_databaseAttribute('DeviceBrand',          'Text',         '');
        $this->define_databaseAttribute('DeviceModel',          'Text',         '');
        $this->define_databaseAttribute('DeviceYear',           'Integer',      0, array('Max'=>2050));
        $this->define_databaseAttribute('DeviceSystemName',     'Text',         '');
        $this->define_databaseAttribute('DeviceSystemVersion',  'Text',         '');

        /* Define application attributes for visitor/admin clients and robots */
        $this->define_databaseAttribute('ApplicationName',      'Text',         '');
        $this->define_databaseAttribute('ApplicationVersion',   'Text',         '');
        $this->define_databaseAttribute('ApplicationType',      'ListItem',     '', array('Values'=> array(
            /* client types */ 'Browser', 'HeadlessBrowser', 'CLI App', 'Mailer', 'Search App', 'Gaming Platform', 'Media Player', 'News Feed', 'Download Manager',
            /* robot types */ 'Spider', 'Crowler', 'Request Library', 'Programming Language', 'Validation Tool', 'Monitoring Tool', 'Translation', 'DataMining', 'Email Harvester', 'Form Spammer',
            /* generic types */ 'Unknown'
        )));

        /* Define generic client attributes */
        $this->define_databaseAttribute('RefererURL',           'Text',         '', array('Length'=>255));
        $this->define_databaseAttribute('RefererWebsite',       'Text',         '', array('Length'=>255));
        $this->define_databaseAttribute('RefererSearch',        'Text',         '', array('Length'=>255));
        $this->define_databaseAttribute('IsReComer',            'Boolean',      0);
        $this->define_databaseAttribute('ClientCountry',        'Country',      '');

        /* Define visitor attributes */
        $this->define_databaseAttribute('VisitorLanguage',      'Language',     '');
        $this->define_databaseAttribute('VisitorCurrentUser',   'ObjectId',      0);

        /* Define admin attributes */
        $this->define_databaseAttribute('AdminStatus',          'ListItem',     '', array('Values'=>array('Off', 'NotLogged', 'LoggedIn', 'LoggedOut'))); // Off : admin not accessed at all | NotLogged : admin accessed but not logged | LoggedIn : admin logged in | LoggedOut : admin was connected, and is no more
        $this->define_databaseAttribute('AdminLanguage',        'Language',     '');
        $this->define_databaseAttribute('AdminCurrentUser',     'ObjectId',     0);
        $this->define_databaseAttribute('AdminLoggedOutUser',   'ObjectId',     0);
        $this->define_databaseAttribute('AdminLoginAttemps',    'Integer',      array('Max'=>255));

        /* Define robot attributes */
        $this->define_databaseAttribute('RobotReadRobotTxt',    'Boolean',      0);
        $this->define_databaseAttribute('RobotReadSiteMap',     'Boolean',      0);

        /* Define other attributes */
        $this->define_databaseAttribute('UseVPN',               'Boolean',      0);
    }
}
?>
