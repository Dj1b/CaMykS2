<?php
/**
 * @brief SystemUniqueIdManager admin module.
 * <br />Module to manage unique ids.
 * @details Plugin / Module Engine
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
 * SystemUniqueIdManager class.
 */
final class SystemUniqueIdManager extends AdminModule {
    /**
     * Class constructor.
     * @param string $location
     * @return void
     */
    public function __construct($location) {
        /* Execute parent construct */
        parent::__construct($location);
    }
}
?>
