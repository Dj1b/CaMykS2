/**
 * @brief AdminPage template, client side scripts.
 * @details Plugin / Template Javascripts
 * @author CaMykS Team
 * @version 1.0pre1
 * @date Creation: Mar 2020
 * @date Modification: Dec 2021
 * @copyright 2020 - 2021 CaMykS
 */
var AdminPage = {
    name: 'AdminPage',
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
        /* Register in CaMykS */
        if (!CaMykS)
            return;
        CaMykS.register_object(this.name, this);

        /* Load locales */
        if (AdminPageLocales) {
            this.load_locales(AdminPageLocales);
        } else {
            //CaMykS.log('Locales not found.');
        }

        /* Finalise initialisation */
        this.loaded = true;
    },

    /* UI related methods */

    /**
     * Toggle admin menu display.
     * @return void
     */
    toggle_adminMenu: function() {
        if (!this.loaded)
            return;
        document.getElementById('AdminMenu').classList.toggle('opened');
    },

    /* UI methods */

    /**
     * Open generic popup
     * @param array params
     * @return void
     */
    open_popup: function(params) {
        /* Check is loaded */
        if (!this.loaded)
            return;

        /* Check params */
        defaultParams = {'content':'', 'width':500, 'height':300};
        for (i in params)
            defaultParams[i] = params[i];
        params = defaultParams;

        /* Open media popup */
        CaMykS.MediaPopup.open_popup('content:'+params['content'], '', params['width'], params['height'], false);
    },

    /**
     * Close generic popup
     * @return void
     */
    close_popup: function() {
        /* Check is loaded */
        if (!this.loaded)
            return;

        /* Close media popup */
        CaMykS.MediaPopup.close_popup();
    },

    /**
     * Open confirmation popup with close and accept buttons.
     * @param array params
     * @return void
     */
    open_confirmationPopup: function(params) {
        /* Check is loaded */
        if (!this.loaded)
            return;

        /* Check params */
        defaultParams = {'content':'', 'class':'', 'width':600, 'height':100, 'closeText':this.get_locale('CancelButton'), 'acceptLink':'', 'acceptText':this.get_locale('ConfirmButton')};
        for (i in params)
            defaultParams[i] = params[i];
        params = defaultParams;

        /* Check has accept link */
        if (params['acceptLink'] === '') {
            if (params['acceptText'] !== '')
                params['buttonText'] = params['acceptText'];
            else if (params['closeText'] !== this.get_locale('CancelButton'))
                params['buttonText'] = params['closeText'];
            this.open_alertPopup(params);
        }

        /* Check MediaPopup component is available */
        if (CaMykS && CaMykS.check_object('MediaPopup')) {
            /* Build content frame */
            content = '<div class="modalPopup">';
            content += '<p class="message' + (params['class'] != '' ? ' '+params['class'] : '') + '">' + params['content'] + '</p>';
            content += '<p class="buttons">';
            content += '<a href="javascript:'+this.name+'.close_popup();" class="redButton">' + params['closeText'] + '</a>';
            content += '<a href="' + params['acceptLink'] + '" class="greenButton">' + params['acceptText'] + '</a>';
            content += '</p></div>';
            params['content'] = content;

            /* Open popup */
            this.open_popup(params);
        } else {
            /* MediaPopup unavailable, use browser features */
            if (confirm(params['content'])) {
                if (params['acceptLink'].substring(11) === 'javascript:') {
                    action = params['acceptLink'].substring(11, params['acceptLink'].length);
                    eval(action);
                } else {
                    /* Consider accept link as a URL */
                    document.location.href = params['acceptLink'];
                }
            } else {
                /* Popup closed, do nothing */
            }
        }
    },

    /**
     * Open alert popup with one button.
     * @param array params
     * @return void
     */
    open_alertPopup: function(params) {
        /* Check is loaded */
        if (!this.loaded)
            return;

        /* Check params */
        defaultParams = {'content':'', 'class':'', 'width':600, 'height':100, 'buttonText':this.get_locale('CloseButton'), 'buttonLink':''};
        for (i in params)
            defaultParams[i] = params[i];
        params = defaultParams;

        /* Check button link */
        if (params['buttonLink'] === '')
            params['buttonLink'] = this.name+'.close_popup();';

        /* Check MediaPopup component is available */
        if (CaMykS && CaMykS.check_object('MediaPopup')) {
            /* Build content frame */
            content = '<div class="modalPopup">';
            content += '<p class="message' + (params['class'] != '' ? ' '+params['class'] : '') + '">' + params['content'] + '</p>';
            content += '<p class="buttons">';
            content += '<a href="' + params['buttonLink'] + '" class="greenButton">' + params['buttonText'] + '</a>';
            content += '</p></div>';
            params['content'] = content;

            /* Open popup */
            this.open_popup(params);
        } else {
            /* MediaPopup unavailable, use browser features */
            if (alert(params['content'])) {
                if (params['acceptLink'].substring(11) === 'javascript:') {
                    action = params['acceptLink'].substring(11, params['acceptLink'].length);
                    eval(action);
                } else {
                    /* Consider accept link as a URL */
                    document.location.href = params['acceptLink'];
                }
            } else {
                /* Popup closed, do nothing */
            }
        }
    },

    /**
     * Open prompt popup.
     * @param array params
     * @return void
     */
    open_promptPopup: function(params) {
        /* Check is loaded */
        if (!this.loaded)
            return;

        /* Check params */
        defaultParams = {'content':'', 'class':'', 'width':600, 'height':100, 'closeText':this.get_locale('CancelButton'), 'acceptLink':'', 'acceptText':this.get_locale('ConfirmButton'), 'inputName':'', 'inputDefault':'', 'inputMaxlength':64, 'inputPlaceholder':'', 'inputOnChange':''};
        for (i in params)
            defaultParams[i] = params[i];
        params = defaultParams;

        /* Check has accept link */
        if (params['acceptLink'] === '') {
            if (params['acceptText'] !== '')
                params['buttonText'] = params['acceptText'];
            else if (params['closeText'] !== this.get_locale('CancelButton'))
                params['buttonText'] = params['closeText'];
            this.open_alertPopup(params);
        }

        /* Check MediaPopup component is available */
        if (CaMykS && CaMykS.check_object('MediaPopup')) {
            /* Build content frame */
            content = '<div class="modalPopup">';
            content += '<form name="promptForm" action="' + params['acceptLink'] + '" method="POST">';
            content += '<p class="message' + (params['class'] != '' ? ' '+params['class'] : '') + '">' + params['content'] + '</p>';
            content += '<p class="inputBox"><input id="' + params['inputName'] + '" type="text" name="' + params['inputName'] + '" value="' + params['inputDefault'] + '" maxlength="' + params['inputMaxlength'] + '"';
            if (params['inputPlaceholder'] !== '')
                content += ' placeholder="' + params['inputPlaceholder'] + '"';
            if (params['inputOnChange'] !== '')
                content += ' onchange="' + params['inputOnChange'] + '"';
            if (params['inputOnInput'] !== '')
                content += ' oninput="' + params['inputOnInput'] + '"';
            content += ' /></p>';
            content += '<p class="buttons">';
            content += '<a href="javascript:'+this.name+'.close_popup();" class="redButton">' + params['closeText'] + '</a>';
            content += '<a href="javascript:document.promptForm.submit();" class="greenButton">' + params['acceptText'] + '</a>';
            content += '</p></form></div>';
            params['content'] = content;

            /* Open popup */
            this.open_popup(params);
        } else {
            /* MediaPopup unavailable, use browser features */

            /* Accept link is an url */
            if (params['acceptLink'].match(/https?:\/\//)) {
                if (params['acceptLink'].indexOf('?') === -1)
                    document.location.href = params['acceptLink']+'?'+params['inputName']+'='+encodeURIComponent(value);
                else
                    document.location.href = params['acceptLink']+'&'+params['inputName']+'='+encodeURIComponent(value);
                return;
            }

            /* Consider accept link is a script action */
            if (acceptLink.substring(11) === 'javascript:')
                acceptLink = acceptLink.substring(11, acceptLink.length);

            if (document.getElementById(params['inputName']))
                input = document.getElementById(params['inputName']);
            else {
                input = docuement.createElement('input').cloneNode(true);
                input.id = params['inputName'];
                input.name = params['inputName'];
                input.type = 'hidden';
                CaMykS.add_HTMLElementToBody(input);
            }
            input.value = prompt(params['content'], params['inputDefault']);
            eval(action);
        }
    },

    /**
     * Open logout confirmation popup.
     * @return void
     */
    open_logoutPopup: function() {
        params = {'content': this.get_locale('LogoutMessage'), 'acceptLink': this.get_param('LogoutURL')};
        this.open_confirmationPopup(params);
    },
}
