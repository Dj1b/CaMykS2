<?php
/**
 * @brief CaMykS Database query, where condition builder library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre0
 * @date Creation: Feb 2022
 * @date Modification: Feb 2022
 * @copyright 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * @todo Creation of where conditions from the parsing from a SQL query fragment
 */

namespace CaMykS2;

/**
 * DbQueryWhereCondition class.
 */
final class DbQueryWhereCondition {
    /**
     * @var string $identifier
     * @brief Identifier name.
     */
    private $identifier;

    /**
     * @var string $identifierTable
     * @brief Identifier table name or alias.
     */
    private $identifierTable = '';

    /**
     * @var mixed $value
     * @brief Value or column name.
     */
    private $value;

    /**
     * @var string $valueTable
     * @brief Table name or alias for value as a column name.
     */
    private $valueTable = '';

    /**
     * @var string $operator
     * @brief Operator to apply between identifier and value.
     */
    private $operator;

    /**
     * @var boolean $isNot
     * @brief Operator is negative.
     */
    private $isNot = false;

    /**
     * Class constructor.
     * @param string $identifier
     * @param mixed $value
     * @param string $operator
     * @param boolean $isNot
     */
    public function __construct($identifier='', $value='', $operator='=', $isNot=false) {
        $this->define_identifier($identifier);
        $this->define_value($value);
        $this->define_operator($operator);
        $this->define_isNot($isNot);
    }

    /* Override magic methods */

    /**
     * Makes some variables accessible in read-only
     * @param string $variableName
     * @return mixed
     */
    public function __get($variableName) {
        if (isset($this->$variableName))
            return $this->$variableName;
        return null;
    }

    /* Variable defining methods */

    /**
     * Define identifier.
     * @param string $identifier
     * @param string $table
     * @return void
     */
    public function define_identifier($identifier=null, $table=null) {
        if (!is_null($identifier))
            $this->identifier = $identifier;
        if (!is_null($table))
            $this->identifierTable = $table;
    }

    /**
     * Define value.
     * @param string $value
     * @param string $table
     * @return void
     */
    public function define_value($value=null, $table=null) {
        if (!is_null($value))
            $this->value = $value;
        if (!is_null($table))
            $this->valueTable = $table;
    }

    /**
     * Define operator.
     * @param string $operator
     * @return void
     */
    public function define_operator($operator) {
        switch ($operator) {
            /* Mathematic operators */
            case '>':
            case 'gt':
                $this->operator = '>'; break;
            case '<':
            case 'lt':
                $this->operator = '<'; break;
            case '>=':
            case 'gte':
                $this->operator = '>='; break;
            case '<=':
            case 'lte':
                $this->operator = '<='; break;
            case '<>':
            case '!=':
                $this->operator = '!='; break;
            case 'between':
                $this->operator = 'between'; break;

            /* String operators */
            case 'start':
            case 'end':
            case 'contain':
                $this->operator = $operator; break;
            case 'like':
                $this->operator = 'contain';

            /* Sub select operators *
            case 'all':
            case 'any':
            case 'exists':
            case 'some':
            */

            /* List operator */
            case 'in':
                $this->operator = $operator; break;

            /* Default operator */
            case '=':
            default:
                $this->operator = '='; break;
        }
    }

    /**
     * Define negative condition.
     * @param boolean $iNot
     * @return void
     */
    public function define_isNot($isNot) {
        if (is_bool($isNot))
            $this->isNot = $isNot;
    }
}
