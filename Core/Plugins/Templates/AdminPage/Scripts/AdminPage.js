/**
 * @brief AdminPage template, client side scripts.
 * @details Plugin / Input Javascripts
 * @author CaMykS Team
 * @version 1.0.0
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS
 */
var AdminPage = {
    name: 'AdminPage',
    loaded: false,
    params: {},

    /**
     * add parameter
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
     * return param value from name
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
     * Initialise object
     * @return void
     */
    initialise: function() {

        /* Finalise initialisation */
        this.loaded = true;
    },

    /**
     * Toggle admin menu display.
     * @return void
     */
    toggle_adminMenu: function() {
        if (!this.loaded)
            return;
        document.getElementById('AdminMenu').classList.toggle('opened');
    },
}
