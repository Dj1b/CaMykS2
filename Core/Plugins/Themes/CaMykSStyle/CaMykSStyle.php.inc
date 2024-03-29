<?php
/**
 * @brief CaMykSStyle theme.
 * @details Plugin / Theme engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Feb 2020
 * @date Modification: Jan 2022
 * @copyright 2020 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * CaMykSStyle class.
 */
final class CaMykSStyle extends Theme {

    /**
     * @var array $params
     * @brief Object "public" parameters.
     */
    protected $params = array('FileMode'=>'Attached');

    /**
     * @var boolean $supportAdminMode
     * @brief Is theme capable to design admin mode.
     */
    protected $supportAdminMode = true;

    /**
     * @var boolean $supportManagerMode
     * @brief Is theme capable to design manager mode.
     */
    protected $supportManagerMode = true;

    /**
     * @var boolean $supportVisitMode
     * @brief Is theme capable to design visit mode.
     */
    protected $supportVisitMode = false;

    /**
     * @var array $adminStyleFiles.
     * @brief List of styles to be attached in admin mode.
     */
    protected $adminStyleFiles = array('admin.css?v=1.0pre15'=>'');

    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }

    /* Override Plugin methods */

    /**
     * Initialise theme.
     * @param array $params
     * @return void
     */
    public function initialise($params=array()) {
        parent::initialise($params);
    }
}
