/**
 * @brief MediaPopup component, client side scripts.
 * @details Plugin / Component Javascripts
 * @author CaMykS Team
 * @version 1.0pre0
 * @date Creation: Dec 2021
 * @date Modification: Dec 2021
 * @copyright 2021 CaMykS
 */
var MediaPopup = {
    name: 'MediaPopup',
    loaded: false,
    params: {},

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
     * Initialise object
     * @return void
     */
    initialise: function() {
        /* Check object is already loaded */
        if (this.loaded)
            return;

        /* Register in CaMykS */
        if (!CaMykS)
            return;
        CaMykS.register_object(this.name, this);

        /* Define some variables */
        this.set_param('popupStatus', 0);
        this.set_param('currentMediaType', '');
        this.set_param('currentMediaWidth', '');
        this.set_param('currentMediaHeight', '');

        /* Build background box */
        this.build_background();

        /* Check for keyboard enabling */


        /* Finalise initialisation */
        this.loaded = true;
    },

    /**
     * Open popup with given media and parameters.
     * @param string media
     * @param string title
     * @param integer width
     * @param integer height
     * @param boolean displayCloseButton
     * @return void
     */
    open_popup: function(media, title, width, height, displayCloseButton) {
        if (!this.loaded)
            return;

        /* Create popup content box */
        if (this.get_param('popupBox') === false) {
            popupBox = document.createElement('div').cloneNode(true);
            popupBox.id = 'MediaPopupContent';
            this.set_param('popupBox', popupBox);
        }
        popupBox.innerHTML = '';
        popupBox.style.display = 'none';
        CaMykS.add_HTMLElementToBody(popupBox);

        /* Store size */
        this.set_param('currentMediaWidth', width);
        this.set_param('currentMediaHeight', height);

        /* Add close button */


        /* Add diaporama buttons */


        /* Load prefix and/or extension */
        mediaExtension = media.split('.').pop().toLowerCase();
        mediaPrefix = media.split(':').shift().toLowerCase();

        /* Build media box */
        mediaBox = document.createElement('div').cloneNode(true);
        popupBox.appendChild(mediaBox);
        this.set_param('mediaBox', mediaBox);

        /* Check media is a picture */
        if (mediaExtension === 'jpg' || mediaExtension === 'gif' || mediaExtension === 'png' || mediaExtension === 'svg') {

            /* Load image */
            var mediaImage = new Image();
            mediaImage.name = media;
            mediaImage.onload = function () {MediaPopup.on_imageLoadSuccess(this)};
            mediaImage.onerror = function () {MediaPopup.on_imageLoadFailure(this)};
            mediaImage.src = media;

            /* Define media type */
            this.set_param('currentMediaType', 'Image');

        /* Check media is a video */
        } else if (mediaExtension === 'mp4' || mediaExtension === 'm4v') {
            mediaBox.innerHTML = '<video controls="" width="'+width+'" height="'+height+'" preload="" src="'+media+'">&nbsp;</video>';

            /* Define media type */
            this.set_param('currentMediaType', 'Video');

        /* Check media is a Youtube video */
        } else if (media.match(/https?:\/\/(www\.)?youtu\.be\//) != null || media.match(/https?:\/\/(www\.)?youtube.com\/watch?v=\//) != null) {
            if (this.get_param('cookieAcceptedForVideos') === 1) {
                /* Get video id from URL */
                if (media.match(/https?:\/\/(www\.)?youtu\.be\//) != null)
                    video = media.substring(file.lastIndexOf('/'));
                else
                    video = media.substring(file.lastIndexOf('='));

                /* Build frame with video */
                mediaBox.innerHTML = '<iframe width="'+width+'" height="'+height+'"'
                    + ' src="https://www.youtube.com/embed/' + video + '"'
                    + ' frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>';
            } else {
                /* Check for cookie message
                if (CookieAgreementMessage !== undefined) {
                    contentBox.innerHTML = '<div> &nbsp; <br /> </div>';
                    contentBox.innerHTML += '<div class="eTitle4">'+CookieAgreementMessage.get_unavailabilityMessageTitle()+'</div>';
                    contentBox.innerHTML += '<div class="eContent1" style="width:350px; max-width:100%; margin:0 auto;">'+CookieAgreementMessage.get_unavailabilityMessageContent('Media Popup')+'</div>';
                } */
            }

            /* Define media type */
            this.set_param('currentMediaType', 'Youtube');

        /* Check media is a Dailymotion video */
        } else if (media.match(/https?:\/\/(www\.)?dailymotion.com\/video\//) != null || media.match(/https?:\/\/dai.ly\//) != null) {
            if (this.get_param('cookieAcceptedForVideos') === 1) {
                /* Get video id from URL */
                video = media.substring(file.lastIndexOf('/'));
                if (video.indexOf('_') > 0)
                    video = video.substring(0, video.indexOf('_'));

                /* Build frame with video */
                mediaBox.innerHTML = '<iframe width="'+width+'" height="'+height+'" src="https://www.dailymotion.com/embed/video/'+video+'"></iframe>';
            } else {

            }

            /* Define media type */
            this.set_param('currentMediaType', 'Dailymotion');

        /* Check media is direct content */
        } else if (mediaPrefix === 'content') {
            mediaBox.innerHTML = media.substring(8);

            /* Define media type */
            this.set_param('currentMediaType', 'Content');

        /* Check media is a web page */
        } else if (mediaExtension == 'html' || mediaExtension == 'htm' || mediaPrefix === 'url') {
            /* Check URL */
            if (mediaPrefix === 'url')
                media = media.substring(4);

            /* Build object */
            html = document.createElement('object').cloneNode(true);
            html.setAttribute('data', media);
            html.setAttribute('type', 'text/html');
            html.style.width = width+'px';
            html.style.height = height+'px';
            mediaBox.appendChild(html);

            /* Define media type */
            this.set_param('currentMediaType', 'Webpage');

        /* Check a media has been loaded */
        } else {
            /* No media found */
            // CaMykS.log_error(this.name, 'Incompatible media');
            console.log('Incompatible media');

            /* Stop creating a popup */
            return;
        }

        /* Build title box */
        if (title !== '') {
            titleBox = document.createElement('div').cloneNode(true);
            titleBox.className = 'mediaTitle';
            titleBox.innerHTML = title;
            popupBox.appendChild(titleBox);
        }

        /* Display popup */
        if (this.get_param('popupStatus') == 0 && this.set_param('currentMediaType') !== 'Image')
            this.display_popup();
    },

    /**
     * Close popup.
     * @return void
     */
    close_popup: function() {
        if (!this.loaded)
            return;

        /* Hide HTML elements */
        this.get_param('popupBkgd').style.display = 'none';
        this.get_param('popupBox').style.display = 'none';
        this.get_param('popupBox').innerHTML = '';

        /* Update variables */
        this.set_param('popupStatus', 0);
        this.set_param('currentMediaType', '');
    },

    /* Image specific methods */

    /**
     * Image load with success event.
     * @return void
     */
    on_imageLoadSuccess: function(image) {
        /* Create image to popup */
        img = document.createElement('img').cloneNode(true);
        img.src = image.src;
        img.style.border = 'none';
        img.style.maxWidth = '100%';

        /* Check sizes */
        if (!(this.get_param('currentMediaWidth') > 0)) {
            this.set_param('currentMediaWidth', image.width);
            this.set_param('currentMediaHeight', image.height);
        }

        /* Apply sizes */
        img.style.width = this.set_param('currentMediaWidth')+'px';
        img.style.height = this.set_param('currentMediaHeight')+'px';

        /* Attach close action */
        img.setAttribute('onclick', this.name+'.close_media()');

        /* Attach picture to media box */
        this.get_param('mediaBox').appendChild(img);

        /* Check for diaporama */


        /* Display popup */
        this.display_popup();
    },

    /**
     * Image load with failure event.
     * @return void
     */
    on_imageLoadFailure: function(image) {
        /* Image loading failed */
        // CaMykS.log_error(this.name, 'Image loading failed');
    },

    /* Private methods */

    /**
     * Build background.
     * @return void
     */
    build_background: function() {
        /* Prepare background */
        bkgd = document.createElement('div').cloneNode(true);
        bkgd.id = 'MediaPopupBkgd';
        bkgd.style.display = 'none';

        /* Check for close action */
        if (this.get_param('EnableBackgroundAction') == 1)
            bkgd.setAttribute('onclick', this.name + '.close_popup();');

        /* Register background object */
        this.set_param('popupBkgd', bkgd);
        CaMykS.add_HTMLElementToBody(bkgd);
    },

    /**
     * Display popup.
     * @return void
     */
    display_popup: function() {
        /* Apply higher z-index to boxes */
        maxZ = dom_getMaxZIndex();
        this.get_param('popupBkgd').style.zIndex = Math.max(maxZ, 10000);
        this.get_param('popupBox').style.zIndex = Math.max(maxZ, 10000)+1;

        /* Update size */
        this.update_mediaSize();

        /* Display background */
        this.get_param('popupBkgd').style.display = 'block';
        this.get_param('popupBox').style.display = 'block';

        /* Update status */
        this.set_param('popupStatus', 1);
    },

    /**
     * Update media size.
     * @return void
     */
    update_mediaSize: function() {
        if (!this.loaded || this.get_param('popupStatus') === 0)
            return;

        /* Load sizes */
        width = this.get_param('currentMediaWidth');
        height = this.get_param('currentMediaHeight');

        /* Update media box width */
        if (width && width > window.innerWidth)
            this.get_param('mediaBox').style.width = '100%';
        else if (width)
            this.get_param('mediaBox').style.width = width+'px';
        else
            this.get_param('mediaBox').style.width = 'auto';

        /* Update media box height */
        if (height && height > window.innerHeight) {
            this.get_param('mediaBox').style.height = '100%';
            this.get_param('popupBox').className = 'largeContent';
        } else {
            this.get_param('popupBox').className = 'smallContent';
            if (width)
                this.get_param('mediaBox').style.height = height+'px';
            else
                this.get_param('mediaBox').style.height = 'auto';
        }
    },
}
