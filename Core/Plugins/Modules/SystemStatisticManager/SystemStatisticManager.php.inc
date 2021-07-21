<?php
/**
 * @brief SystemStatisticManager admin module.
 * <br />Module to manage statistics.
 * @details Plugin / Module Engine
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Jun 2021
 * @date Modification: Jun 2021
 * @copyright 2021 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * SystemStatisticManager class.
 */
final class SystemStatisticManager extends AdminModule {
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
