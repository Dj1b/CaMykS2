<?php
/**
 * @brief HTML Form Input.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0pre3
 * @date Creation: Mar 2020
 * @date Modification: Jan 2022
 * @copyright 2020 - 2022 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * HTMLFormInput class, can be extended.
 */
class HTMLFormInput extends CaMykSLibrary {

    /**
     * @var Object $parent
     * @brief Link to parent form object.
     */
    protected $parent = null;

    /**
     * @var array $params
     * @brief Parameters list for input.
     */
    protected $params = array(
        'type'          => '',
        'name'          => '',
        'language'      => null,
        'default'       => null,
        'allowMultiple' => false,
        'tabIndex'      => 'auto',
    );

    /**
     * @var array $genericAttributes
     * @brief List of generic attributes
     */
    private $genericAttributes = array(
        'id'            => array('type'=>'str', 'default'=>''),
        'class'         => array('type'=>'str', 'default'=>''),
        'style'         => array('type'=>'str', 'default'=>''),
        'onchange'      => array('type'=>'str', 'default'=>''),
        'onfocus'       => array('type'=>'str', 'default'=>''),
        'onblur'        => array('type'=>'str', 'default'=>''),
        'oninput'       => array('type'=>'str', 'default'=>''),
        'onkeydown'     => array('type'=>'str', 'default'=>''),
        'onkeyup'       => array('type'=>'str', 'default'=>''),
        'disabled'      => array('type'=>'str', 'default'=>''),
        'pattern'       => array('type'=>'str', 'default'=>''),
        'list'          => array('type'=>'str', 'default'=>''),
        'autocomplete'  => array('type'=>'str', 'default'=>''),
        'readonly'      => array('type'=>'special', 'default'=>''),
        'data'          => array('type'=>'special', 'default'=>array()),
    );

    /**
     * @var array $specificAttributes
     * @brief List of all specific attributes for inputs.
     */
    private $specificAttributes = array(
        'text' => array(
            'maxlength'             => array('type'=>'int', 'default'=>32),
            'minlength'             => array('type'=>'int', 'default'=>null),
            'size'                  => array('type'=>'int', 'default'=>null),
            'placeholder'           => array('type'=>'str', 'default'=>''),
        ),
        'password' => array(
            'maxlength'             => array('type'=>'int', 'default'=>32),
            'minlength'             => array('type'=>'int', 'default'=>null),
            'size'                  => array('type'=>'int', 'default'=>null),
        ),
        'textarea' => array(
            'cols'                  => array('type'=>'int', 'default'=>40),
            'rows'                  => array('type'=>'int', 'default'=>4),
        ),
        'select' => array(
            'values'                => array('type'=>'params', 'default'=>array()),
            'hasEmptyFirstOption'   => array('type'=>'params', 'default'=>false),
        ),
        'checkbox' => array(
            'value'                 => array('type'=>'str', 'default'=>''),
            'labelTitle'            => array('type'=>'params', 'default'=>''),
            'labelClass'            => array('type'=>'params', 'default'=>''),
            'labelStyle'            => array('type'=>'params', 'default'=>''),
        ),
        'radio' => array(
            'value'                 => array('type'=>'str', 'default'=>''),
            'labelTitle'            => array('type'=>'params', 'default'=>''),
            'labelClass'            => array('type'=>'params', 'default'=>''),
            'labelStyle'            => array('type'=>'params', 'default'=>''),
        ),
        'number' => array(
            'maxlength'             => array('type'=>'int', 'default'=>4),
            'minlength'             => array('type'=>'int', 'default'=>null),
            'size'                  => array('type'=>'int', 'default'=>null),
            'step'                  => array('type'=>'int', 'default'=>1),
            'min'                   => array('type'=>'int', 'default'=>null),
            'max'                   => array('type'=>'int', 'default'=>null),
        ),
    );

    /**
     * Class constructor.
     * @param array $params
     * @param Object $parent
     * @return void
     */
    public function __construct($params=array(), &$parent=null) {
        /* Merge generic attributes default values in params */
        foreach ($this->genericAttributes as $name => $attribute)
            $this->params[$name] = $attribute['default'];

        /* Merge specific attributes default values in params */
        if (isset($params['type']) and isset($this->specificAttributes[$params['type']]))
            foreach ($this->specificAttributes[$params['type']] as $name => $attribute)
                $this->params[$name] = $attribute['default'];

        /* Merge given params */
        $this->params = array_merge($this->params, $params);

        /* Attach parent */
        if (!is_null($parent)) $this->parent = $parent;
    }

    /* Main methods */

    /**
     * Set default value.
     * @param mixed $value
     * @return void
     */
    public function set_default($value=null) {
        $this->params['default'] = $value;
    }

    /**
     * Display input.
     * @return void
     */
    public function display() {
        /* Check name */
        if ($this->params['name'] == '')
            return;

        /* Dispatch display regarding type */
        switch ($this->params['type']) {
            case 'select': return $this->display_select();
            case 'textarea': return $this->display_textArea();
            case 'checkbox':
            case 'radio': return $this->display_tickBox();
            default: return $this->display_generic();
        }
    }

    /* Tool methods */

    /**
     * Return input full name.
     * @return string
     */
    public function get_fullName() {
        $name = $this->params['name'];
        if (!is_null($this->params['language']))
            $name = '_'.$this->params['language'];
        if ($this->params['allowMultiple'] === true)
            $name .= '[]';
        return $name;
    }

    /* Display sub methods */

    /**
     * Display generic input.
     * @return void
     */
    protected function display_generic() {
        /* Build html */
        $html = '<input type="'.$this->params['type'].'"';
        $html .= $this->build_attributes();
        $html .= ' value="'.$this->params['default'].'"';
        $html .= ' />';

        /* Write html */
        echo $html;
    }

    /**
     * Display textarea input.
     * @return void
     */
    protected function display_textArea() {
        /* Build HTML */
        $html = '<textarea';
        $html .= $this->build_attributes();
        $html .= ' >';
        $html .= string_text2html($this->params['default']);
        $html .= '</textarea>';

        /* Write HTML */
        echo $html;
    }

    /**
     * Display tickbox input.
     * @return void
     */
    protected function display_tickBox() {
        /* Check label for automatic id */
        if ($this->params['labelTitle'] != '' and $this->params['id'] == '') {
            if (!is_null($this->parent))
                $this->params['id'] = $this->parent->get_name();
            $this->params['id'] .= '_'.$this->params['name'].($this->params['language'] != '' ? '_'.$this->params['language']:'').'_'.$this->params['value'];
        }

        /* Build tick box HTML */
        $html = '<input type="'.$this->params['type'].'"';
        $html .= $this->build_attributes();
        if ($this->params['default'] === $this->params['value'])
            $html .= ' checked="checked"';
        elseif (is_array($this->params['default']) and in_array($this->params['value'], $this->params['default']))
            $html .= ' checked="checked"';
        $html .= ' />';

        /* Write HTML */
        echo $html;

        /* Check for label */
        if ($this->params['labelTitle'] === '')
            return;

        /* Build HTML */
        $html = '<label for="'.$this->params['id'].'"';
        if ($this->params['labelClass'] != '')
            $html .= ' class="'.$this->params['labelClass'].'"';
        if ($this->params['labelStyle'] != '')
            $html .= ' style="'.$this->params['labelStyle'].'"';
        $html .= '>'.$this->params['labelTitle'].'</label>';

        /* Write HTML */
        echo $html;
    }

    /**
     * Display select input.
     * @return void
     */
    protected function display_select() {
        /* Check for first empty item */
        if ($this->params['hasEmptyFirstOption'] === true)
            $this->params['values'] = array(''=>' ') + $this->params['values'];
        elseif (is_array($this->params['hasEmptyFirstOption']))
            $this->params['values'] = $this->params['hasEmptyFirstOption'] + $this->params['values'];
        elseif ($this->params['hasEmptyFirstOption'] !== false)
            $this->params['values'] = array(''=>$this->params['hasEmptyFirstOption']) + $this->params['values'];

        /* Build HTML */
        $html = '<select';
        $html .= $this->build_attributes();
        $html .= '>' . "\n";

        /* Add options */
        foreach ($this->params['values'] as $name => $value) {
            if (is_array($value)) {
                $dAttr = array('value'=>'', 'class'=>'', 'style'=>'', 'disabled'=>'');
                $value = array_merge($dAttr, $value);

                $html .= '<option value="'.htmlentities($name, ENT_COMPAT, 'UTF-8').'"';
                if ($value['class'] != '')
                    $html .= ' class="'.$value['class'].'"';
                if ($value['style'] != '')
                    $html .= ' style="'.$value['style'].'"';
                if ($value['disabled'] != '')
                    $html .= ' disabled="'.$value['disabled'].'"';
                else
                    $html .= ($name == $this->params['default'] ? ' selected="selected"' : '');
                $html .= '>';
                $html .= $value['value'];
                $html .= '</option>';
            } else {
                $html .= '<option value="'.htmlentities($name, ENT_COMPAT, 'UTF-8').'"';
                $html .= ($name == $this->params['default'] ? ' selected="selected"' : '').'>';
                $html .= $value;
                $html .= '</option>';
            }
            $html .= "\n";
        }

        /* Add select footer */
        $html .= '</select>';

        /* Write HTML */
        echo $html;
    }

    /* Protected methods */

    /**
     * Build html for default attributes.
     * @return string
     */
    protected function build_attributes() {
        $html = '';

        /* Add name */
        $html .= ' name="'.$this->get_fullName().'"';

        /* Add generic attributes */
        foreach ($this->genericAttributes as $name => $attribute) {
            if ($attribute['type'] == 'str') {
                if ($this->params[$name] != '')
                    $html .= ' '.$name.'="'.$this->params[$name].'"';
            } elseif ($attribute['type'] == 'int') {
                if ((is_string($this->params[$name]) and $this->params[$name] != '') or (is_int($this->params[$name]) and $this->params[$name] > 0))
                    $html .= ' '.$name.'="'.$this->params[$name].'"';
            }
        }

        /* Add specific attributes */
        if (isset($this->params['type']) and isset($this->specificAttributes[$this->params['type']])) {
            foreach ($this->specificAttributes[$this->params['type']] as $name => $attribute) {
                if ($attribute['type'] == 'str') {
                    if ($this->params[$name] !== '')
                        $html .= ' '.$name.'="'.$this->params[$name].'"';
                } elseif ($attribute['type'] == 'int') {
                    if (is_int($this->params[$name]))
                        $html .= ' '.$name.'="'.$this->params[$name].'"';
                }
            }
        }

        /* Add readonly */
        if ($this->params['readonly'] == 'readonly' or $this->params['readonly'] == true)
            $html .= ' readonly="readonly"';

        /* Add data */
        if (is_array($this->params['data'])) {
            foreach ($this->params['data'] as $name => $value)
                $html .= ' data-'.$name.'="'.$value.'"';
        }

        /* Add tabIndex */
        if ($this->params['tabIndex'] > 0)
            $html .= ' tabindex="'.$this->params['tabIndex'].'"';
        elseif ($this->params['tabIndex'] == 'auto' and !is_null($this->parent) and ($index = $this->parent->get_nextTabIndex()) > 0)
            $html .= ' tabindex="'.$index.'"';

        /* Return HTML */
        return $html;
    }
}
