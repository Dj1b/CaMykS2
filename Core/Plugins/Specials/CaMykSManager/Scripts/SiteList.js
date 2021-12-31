/**
 * @brief CaMykSManager plugin, site list mode, client side scripts.
 * @details Plugin / Plugin Javascripts
 * @author CaMykS Team
 * @version 1.0pre3
 * @date Creation: Mar 2020
 * @date Modification: Dec 2021
 * @copyright 2020 - 2021 CaMykS
 */
var SiteList = {
    name: 'SiteList',
    loaded: false,
    params: {},
    locales: {},

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
     * Load multiple locales.
     * @param array locales
     * @return void
     */
    load_locales: function(locales) {
        for (i in locales)
            this.add_locale(i, locales[i]);
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

        /* Load locales */
        if (SiteListLocales) {
            this.load_locales(SiteListLocales);
        } else {
            //CaMykS.log('Locales not found.');
        }

        /* Finalise initialisation */
        this.loaded = true;
    },

    /**
     * Toggle folder content display.
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
    },

    /**
     * Toggle website information display.
     * @return void
     */
    toggle_websiteInfoPanelDisplay: function() {
        if (!this.loaded)
            return;
        document.getElementById('WebsiteInfoPanel').classList.toggle('opened');
    },

    /**
     * Open prompt popup for new folder.
     * @param string path
     * @return void
     */
    open_newFolderPromptPopup: function(path) {
        /* Check CaMykS is available */
        if (!CaMykS)
            return;

        /* Check AdminPage is available */
        if (!CaMykS.check_object('AdminPage')) {
            // CaMykS.log('AdminPage object not available');
            return;
        }

        /* Send request using dedicated form */
        form = document.getElementById('NewFolderForm');
        form.Path.value = path;

        /* Open prompt popup */
        CaMykS.AdminPage.open_promptPopup({'content':this.get_locale('NewFolderMessage'), 'acceptLink':'javascript:'+this.name+'.send_newFolderRequest();', 'inputName':'FolderName', 'inputDefault':'', 'inputMaxlength':32, 'inputPlaceholder':'', 'inputOnInput':this.name+'.check_folderName(this);'});
    },

    /**
     * Send new folder request.
     * @return void
     */
    send_newFolderRequest: function() {
        /* Check input is available */
        if (!document.getElementById('FolderName')) {
            // CaMykS.log('FolderName input not available');
            return;
        }

        /* Load input */
        input = document.getElementById('FolderName');
        if (!this.check_folderName(input))
            return;

        /* Send request using dedicated form */
        form = document.getElementById('NewFolderForm');
        form.FolderName.value = input.value;
        form.submit();
    },

    /**
     * Open prompt popup for new CaMykS 2 site.
     * @param string path
     * @return void
     */
    open_newCaMykS2SitePromptPopup: function(path) {
        /* Check CaMykS is available */
        if (!CaMykS)
            return;

        /* Check AdminPage is available */
        if (!CaMykS.check_object('AdminPage')) {
            // CaMykS.log('AdminPage object not available');
            return;
        }

        /* Send request using dedicated form */
        form = document.getElementById('NewC2SiteForm');
        form.Path.value = path;

        /* Open prompt popup */
        CaMykS.AdminPage.open_promptPopup({'content':this.get_locale('NewC2SiteMessage'), 'acceptLink':'javascript:'+this.name+'.send_newCaMykS2SiteRequest();', 'inputName':'SiteName', 'inputDefault':'', 'inputMaxlength':32, 'inputPlaceholder':'', 'inputOnInput':this.name+'.check_folderName(this);'});
    },

    /**
     * Send new CaMykS 2 site request.
     * @return void
     */
    send_newCaMykS2SiteRequest: function() {
        /* Check input is available */
        if (!document.getElementById('SiteName')) {
            // CaMykS.log('FolderName form not available');
            return;
        }

        /* Load input */
        input = document.getElementById('SiteName');
        if (!this.check_folderName(input))
            return;

        /* Send request using dedicated form */
        form = document.getElementById('NewC2SiteForm');
        form.SiteName.value = input.value;
        form.submit();
    },

    /**
     * Check folder name.
     * @param object input
     * @return void
     */
    check_folderName: function(input) {
        value = input.value;
        if (value.match(/^[A-Za-z][A-Za-z0-9_\-]*[A-Za-z0-9]$/)) {
            input.className = 'isValid';
            return true;
        } else {
            input.className = 'isNotValid';
            return false;
        }
    },
}
