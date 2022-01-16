<?php
/**
 * @brief PricePluginLibraryAttribute object.
 * @details Engine / Object Library
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
 * PricePluginLibraryAttribute class.
 */
class PricePluginLibraryAttribute extends FloatPluginLibraryAttribute {

    /**
     * @var constant array SUBPARAMS
     * @brief Attribute level 3 default parameters.
     */
    protected const SUBPARAMS = array('Decimals'=>2);

    /* Override PluginLibraryAttribute methods */

    /**
     * Export for table creation in database.
     * @return array
     */
    public function export_forTableCreation() {
        $params = array(
            /* Generic values */
            'Type'      => 'price',
            'Default'   => $this->defaultValue,

            /* Primary key and index settings */
            'IndexName'     => $this->params['IndexName'],
            'IndexLength'   => $this->params['IndexLength'],
        );

        /* Check minimum value */
        if (!is_null($this->params['Min']))
            $params['Min'] = $this->params['Min'];

        /* Check maximum value */
        if (!is_null($this->params['Max']))
            $params['Max'] = $this->params['Max'];

        /* Check unsigned param */
        if (is_bool($this->params['Unsigned']))
            $params['Unsigned'] = $this->params['Unsigned'];

        /* Return params */
        return $params;
    }
}
?>
