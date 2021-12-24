/**
 * @brief CaMykS client side master script
 * @details Engine / Javascript
 * @author CaMykS Team
 * @version 1.0pre2
 * @date Creation: Mar 2020
 * @date Modification: Dec 2021
 * @copyright 2020 - 2021 Ideogram Design
 */
var CaMykS = {
    name: 'CaMykS',
    loaded: false,
    params: {},
    locales: {},

    /**
     * Set parameter.
     * @param string name
     * @param mixed value
     * @return void
     */
    set_param: function(param, value, subvalue) {
        if (subvalue != undefined && this.params[param])
            this.params[param][value] = subvalue;
        else
            this.params[param] = value;
    },

    /**
     * Return param value from name.
     * @param mixed param
     * @return mixed
     */
    get_param: function(param, value) {
        if (value != undefined)
            if (this.params[param][value] || this.params[param][value] === 0 || this.params[param][value] === false)
                return this.params[param][value];
            else
                return false;
        if (this.params[param])
            return this.params[param];
        return false;
    },

    /**
     * Add locale.
     * @param string name
     * @param mixed value
     * @return void
     */
    add_locale: function(name, value) {
        this.locales[name] = value;
    },

    /**
     * Return locale value from name.
     * @param mixed param
     * @return mixed
     */
    get_locale: function(name) {
        if (this.locales[name])
            return this.locales[name];
        return name;
    },

    /**
     * Initialise object
     * @return void
     */
    initialise: function() {
        /* Set object as loaded */
        this.loaded = true;
    },

    /* Objects related methods */

    /**
     * Register an object in CaMykS.
     * @param string name
     * @param object object
     * @return void
     */
    register_object: function(name, object) {
        if (eval('this.'+name))
          return false;
        eval('this.'+name+' = object');
        return true;
    },

    /**
     * Check object is available.
     * @param string name
     * @return boolean result
     */
    check_object: function(name) {
        if (eval('this.'+name))
            return true;
        return false;
    },

    /**
     * Return object.
     * @param string name
     * @return object
     */
    get_object: function(name) {
        if (eval('this.'+name))
            return eval('this.'+name);
        return false;
    },

    /* DOM related tool methods */

    /**
     * Check if given element is HTMLElement.
     * @param HTMLElement element
     * @return boolean result
     */
    is_HTMLElement: function(element) {
        return element instanceof HTMLElement;
    },

    /**
     * Add element to body.
     * @param HTMLElement element
     * @return boolean success
     */
    add_HTMLElementToBody: function(element) {
        if (this.is_HTMLElement(element)) {
            document.getElementsByTagName('body')[0].appendChild(element);
            return true;
        }
        return false;
    },
}
