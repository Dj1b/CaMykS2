<?php
/**
 * @brief HTML Form.
 * @details Engine / Object Library
 * @author CaMykS Team <CaMykS.contact@gmail.com>
 * @version 1.0.1
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS Team
 * @note This program is distributed as is - WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

namespace CaMykS2;

/**
 * HTMLForm class, can be extended.
 */
class HTMLForm extends CaMykSLibrary {
    /**
     * @var string $status
     * @brief Form status
     */
    protected $status = 'initiated'; // opened, closed

    /**
     * @var boolean $useJS
     * @brief Do the form use client side scripts to add controls on inputs.
     */
    protected $useJS = false;

    /**
     * @var integer $uploadLimit
     * @brief Files upload limit.
     */
    protected $uploadLimit = 0;

    /**
     * @var Object $contentObject
     * @brief Link to content object for automatic default values.
     */
    protected $contentObject;

    /**
     * @var array $hiddens
     * @brief Form hidden value list.
     */
    protected $hiddens = array();

    /**
     * @var integer $inputsCount
     * @brief Form inputs counter.
     */
    protected $inputsCount = 0;

    /**
     * @var integer $inputsLimit
     * @brief Form input limit.
     */
    protected $inputsLimit;

    /**
     * @var array $inputsLimitUniques
     * @brief Form stored unique input names.
     */
    protected $inputsLimitUniques = array();

    /**
     * @var boolean $inputsLimitReached
     * @brief Form input limit reached.
     */
    protected $inputsLimitReached = false;

    /**
     * @var integer $tabIndex
     * @brief Form tabulation index.
     */
    protected $tabIndex = -1;

    /**
     * Class constructor.
     * @param array $params
     * @return void
     */
    public function __construct($params=array()) {
        /* Default params */
        $defaultParams = array('Id'=>'', 'Name'=>'form', 'Action'=>'', 'Method'=>'POST', 'OnSubmit'=>'', 'Target'=>'');
        $params = array_merge($defaultParams, $params);

        /* Parent construct */
        parent::__construct($params);

        /* Set up inputs limit */
        $this->inputsLimit = ini_get('max_input_vars');
    }

    /* Settings methods */

    /**
     * Define file upload limit.
     * @param mixed $limit
     * @return void
     */
    public function set_uploadLimit($limit=0) {
        /* Read integer value */

        /* Read text value */

        /* Check limit with PHP limit */

        /* Try to raise PHP limit if necessary */

        /* Update form limit */

        /* Add hidden input form */

    }

    /**
     * Define content object.
     * @param Object $content
     * @return void
     */
    public function set_contentObject($content) {
        $this->contentObject = &$content;
    }

    /**
     * Insert hidden value.
     * @param string $name
     * @param string $value
     * @param array $attributes
     */
    public function add_hidden($name, $value, $attributes=array()) {
        $this->hiddens[$name] = array_merge(array('name'=>$name, 'default'=>$value), $attributes);
    }

    /**
     * Enable client side scripts for input controls.
     * @return void
     */
    public function enable_JSControls() {
        global $CaMykS;

        /* Set up internal variable */
        $this->useJS = true;

        /* Load master script file */
        $CaMykS->HTMLPage->add_styleFile($CaMykS->get_assetFileURL('Scripts/Objects/HTMLForm.js'));

        /* Declare form : to do */
    }

    /* Data gathering methods */

    /**
     * Return form name.
     * @return string
     */
    public function get_name() {
        return $this->params['Name'];
    }

    /**
     * Return current form, HTML submit link.
     * @return string
     */
    public function get_HTMLSubmitLink() {
        return 'javascript:document.'.$this->get_name().'.submit();';
    }

    /**
     * Return current form, javascript submit link.
     * @return string
     */
    public function get_JSSubmitLink() {
        return 'document.'.$this->get_name().'.submit();';
    }

    /* Main display methods */

    /**
     * Display form header.
     * @return void
     */
    public function display_header() {
        /* Check status */
        if ($this->status != 'initiated')
            return '';

        /* Build tag */
        $tag = '<form';
        $tag .= ' id="'.$this->get_id().'"';
        $tag .= ' name="'.$this->params['Name'].'"';
        $tag .= ' action="'.$this->params['Action'].'"';
        $tag .= ' method="'.$this->params['Method'].'"';
        if ($this->uploadLimit > 0)
            $tag .= ' enctype="multipart/form-data"';
        if ($this->params['Target'] != '')
            $tag .= ' target="'.$this->params['Target'].'"';
        if ($this->params['OnSubmit'] != '')
            $tag .= ' onsubmit="'.$this->params['OnSubmit'].'"';
        $tag .= '>';

        /* Display tag */
        echo $tag;

        /* Update status */
        $this->status = 'opened';
    }

    /**
     * Display form hiddens.
     * @return string
     */
    public function display_hiddens() {
        /* Check status */
        if ($this->status != 'opened')
            return '';

        /* Display all hidden inputs */
        foreach ($this->hiddens as $hidden) {
            $this->display_hiddenInput($hidden);
            echo "\n";
        }
    }

    /**
     * Display form footer.
     * @return string
     */
    public function display_footer() {
        /* Check status */
        if ($this->status != 'opened')
            return '';

        /* Display tag */
        echo '</form>';

        /* Update status */
        $this->status = 'closed';
    }

    /* Basic inputs display methods */

    /**
     * Display input.
     * @param array $params
     * @return void
     */
    public function display_input($params=array()) {
        /* Load new input */
        $input = new HTMLFormInput($params, $this);

        /* Check inputs limit */
        if ($this->check_inputsLimit(1))
            $input->display();
    }

    /**
     * Display hidden input.
     * @param array $params
     */
    public function display_hiddenInput($params=array()) {
        $this->display_input(array_merge(array('type'=>'hidden'), $params));
    }

    /**
     * Display text input.
     * @param array $params
     */
    public function display_textInput($params=array()) {
        $this->display_input(array_merge(array('type'=>'text'), $params));
    }

    /**
     * Display password input.
     * @param array $params
     */
    public function display_passwordInput($params=array()) {
        $this->display_input(array_merge(array('type'=>'password'), $params));
    }

    /* Advanced inputs display methods */


    /* TabIndex related methods */

    /**
     * Start tabulation index.
     * @return void
     */
    public function start_tabIndex() {
        $this->tabIndex = 0;
    }

    /**
     * Return next tabulation index value
     * @return integer
     */
    public function get_nextTabIndex() {
        if ($this->tabIndex > 0)
            return ++$this->tabIndex;
        return 0;
    }

    /* Protected methods */

    /**
     * Return form id.
     * @return string
     */
    protected function get_id() {
        return ($this->params['Id'] != '' ? $this->params['Id'] : $this->params['Name']);
    }

    /**
     * Check max input vars limit.
     * @param integer $items
     * @return boolean result
     */
    protected function check_inputsLimit($items=1, $name=false) {
        global $camyks;

        /* Check if limit has already been reached */
        if ($this->inputsLimitReached)
            return false;


        /* Update count value */
        $this->inputsCount += $items;

        /* Check count value */
        if ($this->inputsLimit !== false and $this->inputsCount >= $this->inputsLimit) {
            /* Send error */

            return false;
        }

        /* Return success */
        return true;
    }
}
?>
