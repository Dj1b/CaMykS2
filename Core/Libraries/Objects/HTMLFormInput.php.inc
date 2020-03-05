<?php
/**
 * @brief HTML Form Input.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.0
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
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
     * @brief List of all params for inputs.
     */
    protected $params = array(
        'type'          => '',
        'id'            => '',
        'name'          => '',
        'language'      => null,
        'default'       => null,
        'class'         => '',
        'style'         => '',
        'onchange'      => '',
        'onfocus'       => '',
        'onblur'        => '',
        'oninput'       => '',
        'onkeydown'     => '',
        'onkeyup'       => '',
        'disabled'      => '',
        'data'          => array(),
        'tabIndex'      => 'auto',
        'placeholder'   => '',
        'maxlength'     => 0,
        'minlength'     => 0,
        'pattern'       => '',
        'readonly'      => '',
        'list'          => '',
        'autocomplete'  => '',
        'allowMultiple' => false,
        'size'          => 0,
    );

    /**
     * @var array $specificAttributes
     * @brief List of all specific attributes for inputs.
     */
    protected $specificAttributes = array(
        'text' => array(
            'maxlength'             => 32,
        ),
        'password' => array(
            'maxlength'             => 32,
        ),
        'textarea' => array(
            'cols'                  => 40,
            'rows'                  => 4,
        ),
        'select' => array(
            'values'                => array(),
            'hasEmptyFirstOption'   => false,
        ),
        'checkbox' => array(
            'value'                 => '',
            'labelTitle'            => '',
            'labelClass'            => '',
            'labelStyle'            => '',
        ),
    );

    /**
     * Class constructor.
     * @param array $params
     * @param Object $parent
     * @return void
     */
    public function __construct($params=array(), &$parent=null) {
        /* Merge specific attributes */
        if (isset($params['type']) and isset($this->specificAttributes[$params['type']]))
            $this->params = array_merge($this->params, $this->specificAttributes[$params['type']]);

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
     * @eturn void
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
        $html .= ' cols="'.$this->params['cols'].'"';
        $html .= ' rows="'.$this->params['rows'].'"';
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
        if ($this->params['label'] != '' and $this->params['id'] == '') {
            if (!is_null($this->parent))
                $this->params['id'] = $this->parent->get_name();
            $this->params['id'] .= '_'.$this->params['name'].($this->params['language'] != '' ? '_'.$this->params['language']:'').'_'.$this->params['value'];
        }

        /* Build tick box HTML */
        $html = '<input type="'.$this->params['type'].'"';
        $html .= $this->build_attributes();
        $html .= ' value="'.$this->params['value'].'"';
        if ($this->params['default'] === $pthis->arams['value'])
            $html .= ' checked="checked"';
        elseif (is_array($this->params['default']) and in_array($this->params['value'], $this->params['default']))
            $html .= ' checked="checked"';
        $html .= ' />';

        /* Write HTML */
        echo $html;

        /* Check for label */
        if ($this->params['label'] === '')
            return;

        /* Build HTML */
        $html .= '<label for="'.$this->params['id'].'"';
        if ($this->params['labelClass'] != '')
            $html .= ' class="'.$this->params['labelClass'].'"';
        if ($this->params['labelStyle'] != '')
            $html .= ' style="'.$this->params['labelStyle'].'"';
        $html .= '>'.$this->params['label'].'</label>';

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

        /* Add id */
        if ($this->params['id'] != '')
            $html .= ' id="'.$this->params['id'].'"';

        /* Add class */
        if ($this->params['class'] != '')
            $html .= ' class="'.$this->params['class'].'"';

        /* Add style */
        if ($this->params['style'] != '')
            $html .= ' style="'.$this->params['style'].'"';

        /* Add max length */
        if ($this->params['maxlength'] > 0)
            $html .= ' maxlength="'.$this->params['maxlength'].'"';

        /* Add min length */
        if ($this->params['minlength'] > 0)
            $html .= ' minlength="'.$this->params['minlength'].'"';

        /* Add on change event */
        if ($this->params['onchange'] != '')
            $html .= ' onchange="'.$this->params['onchange'].'"';

        /* Add on key down event */
        if ($this->params['onkeydown'] != '')
            $html .= ' onkeydown="'.$this->params['onkeydown'].'"';

        /* Add on key up event */
        if ($this->params['onkeyup'] != '')
            $html .= ' onkeyup="'.$this->params['onkeyup'].'"';

        /* Add on focus event */
        if ($this->params['onfocus'] != '')
            $html .= ' onfocus="'.$this->params['onfocus'].'"';

        /* Add on blur event */
        if ($this->params['onblur'] != '')
            $html .= ' onblur="'.$this->params['onblur'].'"';

        /* Add on input event */
        if ($this->params['oninput'] != '')
            $html .= ' oninput="'.$this->params['oninput'].'"';

        /* Add disabled */
        if ($this->params['disabled'] != '')
            $html .= ' disabled="disabled"';

        /* Add placeholder */
        if ($this->params['placeholder'] != '')
            $html .= ' placeholder="'.$this->params['placeholder'].'"';

        /* Add readonly */
        if ($this->params['readonly'] == 'readonly' or $this->params['readonly'] == true)
            $html .= ' readonly="readonly"';

        /* Add list */
        if ($this->params['list'] != '')
            $html .= ' list="'.$this->params['list'].'"';

        /* Add autocomplete */
        if ($this->params['autocomplete'] != '')
            $html .= ' autocomplete="'.$this->params['autocomplete'].'"';

        /* Add pattern */
        if ($this->params['pattern'] != '')
            $html .= ' pattern="'.$this->params['pattern'].'"';

        /* Add data */
        if (is_array($this->params['data'])) {
            foreach ($this->params['data'] as $name => $value)
                $html .= ' data-'.$name.'="'.$value.'"';
        }

        /* Add size */
        if ($this->params['size'] > 0)
            $html .= ' size="'.$this->params['size'].'"';

        /* Add tabIndex */
        if ($this->params['tabIndex'] > 0)
            $html .= ' tabindex="'.$this->params['tabIndex'].'"';
        elseif ($this->params['tabIndex'] == 'auto' and !is_null($this->parent) and ($index = $this->parent->get_nextTabIndex()) > 0)
            $html .= ' tabindex="'.$index.'"';

        /* Return HTML */
        return $html;
    }
}
