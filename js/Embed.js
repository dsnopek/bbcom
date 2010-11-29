
require.def(
    ['jquery','lingwo_dictionary/annotation/Reader2'],
    function ($, Reader) {
        var BiblioBird = {},
            configDefaults = {
                url: 'http://<lang>.bibliobird.com',
                lang: 'en',
                showLinks: true,
                // TODO: this should be false or not even in this code!
                //ga: false
                ga: true,

                // can be '#' (remove hash), '?' (remove query string and hash) or '' (raw)
                pageUrlRemove: '#',
                getPageUrl: function () {
                    var url = ''+window.location.href;
                    // We do a switch so '?' does both characters
                    switch (BiblioBird.pageUrlRemove) {
                        case '?':
                            url = url.split('?')[0];
                        case '#':
                            url = url.split('#')[0];
                    };
                    return url;
                },
            };

        function extend(obj, vals, preserve) {
            var name;
            for(name in vals) {
                if (!preserve || typeof obj[name] == 'undefined') {
                    obj[name] = vals[name];
                }
            }
        }

        function bburl(path) {
            var url = BiblioBird.url.replace('<lang>', BiblioBird.lang);
            if (path) {
                return url+'/'+path;
            }
            return url;
        }

        // copy config if it is set!
        if (typeof window.BiblioBird != 'undefined') {
            BiblioBird = window.BiblioBird;
        }
        // setup config defaults
        extend(BiblioBird, configDefaults, true);
        // define the BiblioBird object
        extend(BiblioBird, {
            initialized: false,
            contentAreas: [],
            embedWindow: null,
            embedIframe: null,
            embedWindowShown: false,
            username: null,

            // this is called at the beginning, but doesn't necessarily cause the object
            // to initialize itself.  That only happens if a .bibliobird-content node is
            // found on the page.
            start: function () {
                var self = this;
                $('.bibliobird-content').each(function () {
                    self.contentAreas.push(new ContentArea(this));
                });
            },

            initialize: function (username) {
                if (this.initialized) return;

                var embedWindow = $('<div class="clear-block"><div class="bibliobird-embed-titlebar"><span class="bibliobird-embed-title">BiblioBird</span> <a href="#" class="bibliobird-embed-close">close</a></div></div>'),
                    embedIframe = $('<iframe height="100%" width="100%" border="0" id="bibliobirdEmbedIframe" name="bibliobirdEmbedIframe"></iframe>').appendTo(embedWindow);

                // make the embedWindow / embedIframe
                embedWindow
                    .css({
                        width: 400,
                        height: 200,
                        position: 'fixed',
                        border: '1px solid black',
                        background: 'white',
                        display: 'none'
                    })
                    .appendTo($('body'));
                $('.bibliobird-embed-close', embedWindow).click(function () {
                    BiblioBird.hideEmbedWindow();
                    return false;
                });
                function positionEmbedWindow() {
                    var left = ($(window).width() - 400) / 2,
                        top  = ($(window).height() - 200) / 2;
                    embedWindow.css({ top: top, left: left });
                }
                $(window).resize(positionEmbedWindow);
                setTimeout(positionEmbedWindow, 0);

                // initialize the Reader object
                Reader.setup({ layout: 'popup', skipHoverEvents: true });
                $('body').click(function (evt) {
                    BiblioBird.hideEmbedWindow();
                    return Reader.handleClick(evt.target);
                });

                // copy values onto the object
                extend(this, {
                    embedWindow: embedWindow,
                    embedIframe: embedIframe,
                    username:    username,
                    initialized: true
                });
            },

            receiveMessage: function (msg) {
                var parts;
                if (msg.indexOf('#login-successful') == 0) {
                    parts = msg.split(':');
                    if (parts[1]) {
                        this.username = parts[1];
                    }

                    this.hideEmbedWindow();
                }
            },

            openEmbedWindow: function (url) {
                if (this.embedWindow !== null && !this.embedWindowShown) {
                    this.embedIframe.attr('src', url);
                    this.embedWindow.show();
                    this.embedWindowShown = true;
                }
            },

            hideEmbedWindow: function () {
                if (this.embedWindow !== null && this.embedWindowShown) {
                    this.embedWindow.hide();
                    this.embedWindowShown = false;
                    this.refreshAll();
                }
            },

            logout: function() {
                var self = this;
                $.ajax({
                    url: bburl('remote/logout'),
                    dataType: 'jsonp',
                    success: function (res) {
                        self.username = null;
                        self.refreshAll();
                    }
                });
            },

            refreshAll: function () {
                $.each(this.contentAreas, function () {
                    this.refresh();
                });
            }
        });

        Reader.onLoad = function (target) {
            var parts = (target.attr('data-entry') || '').split('#'),
                hash  = parts[0],
                sense = parts[1];

            Reader.setBubbleLoading();

            // lookup the entry on the server
            $.ajax({
                url: bburl('embed-reader/lookup_entry'),
                dataType: 'jsonp',
                data: { hash: hash, lang: BiblioBird.lang },
                success: function (res) {
                    Reader.setBubbleContent(res.content);

                    // TODO: this shouldn't be here, but on LinguaTrek somehow!
                    if (BiblioBird.ga && typeof window['_gaq'] != 'undefined') {
                        // TODO: determine from the res if we are anonymous or not
                        _gaq.push(['_trackEvent', 'BiblioBird', 'Anonymous Lookup', BiblioBird.lang]);
                    }

                    $('.node', Reader.contentNode)
                        .removeClass('clear-block')
                        .removeAttr('id');

                    // TODO: for now, we want to drop the node links
                    $('ul.links.inline', Reader.contentNode).remove();

                    if (sense) {
                        $('.lingwo-sense-id-'+sense, Reader.contentNode).addClass('selected');
                    }

                    // TODO: how to handle this?
                    // hack to integrate the flag module
                    /*
                    if (Drupal.flagLink) {
                        Drupal.flagLink(Reader.contentNode);
                    }
                    */

                    // make links relative to bibliobird
                    $('a', Reader.contentNode).each(function (i, node) {
                        var href = node.href, localHost;
                        if (href) {
                            localHost = window.location.protocol + '//' + window.location.host;
                            node.href = href.replace(localHost, bburl());
                            node.target = '_blank';
                        }
                    });
                }
            });
        }

        ContentArea = function (x) {
            this.node      = x;
            this.url       = $(x).attr('data-url') || BiblioBird.getPageUrl();
            this.teaser    = $(x).attr('data-teaser') == 'true';
            this.links     = $('<div class="bibliobird-links"></div>').insertBefore(x),
            this.data      = {};


            this.lookupContent();
        }
        extend(ContentArea.prototype, {
            lookupContent: function () {
                var self = this;

                $.ajax({
                    url: bburl('embed-reader/lookup_content'),
                    dataType: 'jsonp',
                    data: { url: this.url },
                    success: function (res) {
                        // we only initialize after we have a username..
                        BiblioBird.initialize(res.username);

                        self.data = res;
                        self.rebuildLinks();

                        if (res.content) {
                            // assign the content
                            self.node.innerHTML = res.content;
                            Reader.setupHoverEvents();
                        }
                    }
                });
            },
            rebuildLinks: function () {
                if (!BiblioBird.showLinks) return;

                var links = this.links,
                    data  = this.data,
                    self  = this;

                links.html('');

                if (BiblioBird.username) {
                    links.append('Logged into BiblioBird as '+BiblioBird.username+' ');
                    links.append($('<a></a>')
                        .html('Logout')
                        .attr('href', bburl('logout'))
                        // TODO: I think we need a special JSONP logout function, because we need to know when
                        // its finished
                        .click(function () { BiblioBird.logout(); return false; })
                    );
                }
                else {
                    links.append('Not logged into BiblioBird ');
                    links.append($('<a></a>')
                        .html('Login')
                        .attr('href', bburl('remote/login') +
                            (BiblioBird.localRelayUrl ? '?relay='+BiblioBird.localRelayUrl : ''))
                        .click(function (evt) { BiblioBird.openEmbedWindow(evt.target.href); return false; })
                    );
                    links.append(' ');
                    links.append($('<a></a>')
                        .html('Join BiblioBird')
                        .attr({
                            href: bburl('user/register'),
                            target: '_blank'
                        })
                    );
                }
                links.append(' ');

                if (data.not_found) {
                    links.append($('<a></a>')
                        .html('Add to Bibliobird')
                        // TODO: we need some configuration, so we can point the user to the
                        // correct language site!
                        .attr({
                            href: bburl('node/add/content?remote_url='+escape(this.url)),
                            target: '_blank'
                        })
                    );
                }
            },
            refresh: function () {
                this.rebuildLinks();
            }
        });

        // attach a content area to each found on the page
        // leave for other scripts
        window.BiblioBird = BiblioBird;

        // return so that the loading code can call .start()
        return BiblioBird;
    }
);

