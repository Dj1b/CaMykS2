/**
 * @brief CaMykSManager plugin, site list mode, client side scripts.
 * @details Plugin / Input Javascripts
 * @author CaMykS Team
 * @version 1.0.0
 * @date Creation: Mar 2020
 * @date Modification: Mar 2020
 * @copyright 2020 CaMykS
 */
var SiteList = {
    name: 'SiteList',
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
        /* Load form */
        if (!document.getElementById(this.get_param('form')))
            return;
        this.set_param('form', document.getElementById(this.get_param('form')));

        /* Finalise initialisation */
        this.loaded = true;
    },

    /**
     * Toggle admin menu display.
     * @return void
     */
    toggle_folderContentDisplay: function(folder) {
        if (!this.loaded)
            return;

        if (document.getElementById(folder))
            document.getElementById(folder).classList.toggle('opened');
        else
            console.log('Error opening folder '+ folder);
    },

    /**
     * Open external link.
     * @param string action
     * @param string folder
     * @return void
     */
    open_externalLink: function(action, folder) {
        if (!this.loaded)
            return;

        this.get_param('form').ManagerAction.value = action;
        this.get_param('form').Folder.value = folder;
        this.get_param('form').submit();
    }
}
