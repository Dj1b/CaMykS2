<?php
/**
 * @brief CaMykS Database query, where condition builder library.
 * @details Engine / Object Library
 * @author CaMykS Team <camyks.contact@gmail.com>
 * @version 1.0pre2
 * @date Creation: Feb 2022
 * @date Modification: Oct 2026
 * @copyright 2022 - 2026 CaMykS Team
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
    private string $identifier;

    /**
     * @var string $identifierTable
     * @brief Identifier table name or alias.
     */
    private string $identifierTable = '';

    /**
     * @var mixed $value
     * @brief Value or column name.
     */
    private mixed $value;

    /**
     * @var string $valueTable
     * @brief Table name or alias for value as a column name.
     */
    private string $valueTable = '';

    /**
     * @var string $operator
     * @brief Operator to apply between identifier and value.
     */
    private string $operator;

    /**
     * @var boolean $isNot
     * @brief Operator is negative.
     */
    private bool $isNot = false;

    /**
     * Class constructor.
     * @param string $identifier
     * @param mixed $value
     * @param string $operator
     * @param boolean $isNot
     * @return void
     */
    public function __construct(string $identifier='', mixed $value='', string $operator='=', bool $isNot=false) {
        $this->define_identifier($identifier);
        $this->define_value($value);
        $this->define_operator($operator);
        $this->define_isNot($isNot);
    }

    /* Mark: Override magic methods */

    /**
     * Makes some variables accessible in read-only
     * @param string $variableName
     * @return mixed
     */
    public function __get(string $variableName): mixed {
        if (isset($this->$variableName))
            return $this->$variableName;
        return null;
    }

    /* Mark: Variable defining methods */

    /**
     * Define identifier.
     * @param string|null $identifier
     * @param string|null $table
     * @return void
     */
    public function define_identifier(?string $identifier=null, ?string $table=null): void {
        if (!is_null($identifier))
            $this->identifier = $identifier;
        if (!is_null($table))
            $this->identifierTable = $table;
    }

    /**
     * Define value.
     * @param mixed $value
     * @param string|null $table
     * @return void
     */
    public function define_value(mixed $value=null, ?string $table=null): void {
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
    public function define_operator(string $operator): void {
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
     * @param boolean $isNot
     * @return void
     */
    public function define_isNot(bool $isNot): void {
        if (is_bool($isNot))
            $this->isNot = $isNot;
    }
}
