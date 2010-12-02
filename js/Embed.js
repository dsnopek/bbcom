
define(
    ['jquery',
     'lingwo_dictionary/annotation/Reader2',
     'lingwo_dictionary/layout/BottomDock',
     'lingwo_dictionary/util/declare'
    ],
    function ($, Reader, BottomDockLayout, declare) {
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
                    var url = ''+window.self.location.href;
                    // We do a switch so '?' does both characters
                    switch (BiblioBird.pageUrlRemove) {
                        case '?':
                            url = url.split('?')[0];
                        case '#':
                            url = url.split('#')[0];
                    };
                    return url;
                }
            },
            supportedLanguages = {'en': 'English', 'pl': 'Polski' },
            trans = {'en': {}},
            Dock;

        // TODO: make this better (with the require NLS stuff)
        trans['pl'] = {
            'Translate to': 'Tłumacz na',
            'Words I Am Learning': 'Mój osobisty słowniczek',
            'Hello': 'Cześć',
            'Login': 'Zaloguj się',
            'or': 'lub',
            'register': 'utwórz konto',
            'on BiblioBird': 'w systemie BiblioBird',
            'not me!': 'to nie ja!'
        };
        function t(s) {
            var v = trans[BiblioBird.lang][s];
            return v ? v : s;
        }

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

        Dock = declare({
            _constructor: function () {
                var size = 35;

                this.innerNode = $('<div id="bibliobird-dock" class="clear-block"></div>').get(0);
                this.node = $('<div></div>').css('zIndex', '101').append(this.innerNode).get(0);
                this.userNode = $('<div></div>').css({float: 'left', 'padding-left': 10 }).get(0);

                this._rebuild();

                this._layout = new BottomDockLayout({ node: this.node, size: size });
                this._layout.layout();
            },

            /*
             * TODO: I couldn't imagine uglier code!  This should be replace with a template and an
             * external CSS file..
             */
            _rebuild: function () {
                var langCode, languageSwitcher, option, wialButton, self=this;

                // clear existing data
                this.innerNode.innerHTML = '';

                // build language switcher
                languageSwitcher = $('<select id="bibliobird-language-switcher"></select>').get(0);
                for (langCode in supportedLanguages) {
                    option = $('<option>'+supportedLanguages[langCode]+'</option>');
                    option.attr('value', langCode);
                    if (langCode == BiblioBird.lang) {
                        option.attr('selected', 'selected');
                    }
                    $(languageSwitcher).append(option);
                }
                $(languageSwitcher).bind('change', function (evt) {
                    BiblioBird.lang = $('option:selected', languageSwitcher).val();
                    self._rebuild();
                });

                // create the WIAL button
                wialButton = $('<a id="bibliobird-wial-button"></a>')
                    .css({
                        display: 'block',
                        float: 'right',
                        background: "transparent url('http://en.bibliobird.com/sites/all/themes/lingwoorg_theme/images/join-btn-left.png') left no-repeat",
                        'padding-left': 11,
                        height: 27,
                        'margin-left': 10,
                        'margin-top': 2,
                        'color': 'white',
                        'font-size': '12px',
                        'font-weight': 'bold',
                        'text-decoration': 'none',
                        'cursor': 'pointer'
                    })
                    .attr({
                        href: bburl('wial'),
                        target: '_blank'
                    })
                    .append(
                        $('<span></span>')
                            .css({
                                display: 'block',
                                float: 'left',
                                background: "transparent url('http://en.bibliobird.com/sites/all/themes/lingwoorg_theme/images/join-btn-right.png') right no-repeat",
                                'padding-right': 14,
                                height: 27,
                                'cursor': 'pointer'
                            })
                            .append(
                                $('<span></span>')
                                    .css({
                                        display: 'block',
                                        float: 'left',
                                        background: "transparent url('http://en.bibliobird.com/sites/all/themes/lingwoorg_theme/images/join-btn-middle.png') center repeat-x",
                                        'padding-top': 4,
                                        height: 23,
                                        'cursor': 'pointer'
                                    })
                                    .text(t('Words I Am Learning'))
                            )
                    );

                $(this.innerNode)
                    .css({
                        height: '100%',
                        background: '#e5e5f9',
                        'border-top': '1px solid black',
                        'font-size': '14pt'
                    })
                    .append(
                        $('<div>BiblioBird</div>')
                            .css({
                                float: 'left',
                                background: '#07104C',
                                'font-weight': 'bold',
                                //height: size,
                                height: '100%',
                                color: 'white',
                                'font-size': '16pt',
                                padding: '0 10px',
                                'border-right': '1px solid white'
                            })
                    )
                    .append(this.userNode)
                    .append(
                        $('<div></div>')
                            .css({
                                float: 'right',
                                'padding-right': '10px'
                            })
                            .append('<label for="bibliobird-language-switcher">'+t('Translate to')+': </label>')
                            .append(languageSwitcher)
                            .append(wialButton)
                    );

                this.rebuildLinks();
            },

            rebuildLinks: function () {
                var links = $(this.userNode);

                // TODO: this is a hack!
                $('#bibliobird-wial-button', this.node)[BiblioBird.username ? 'show' : 'hide']();

                links.html('');

                if (BiblioBird.username) {
                    links.append(t('Hello')+', '+BiblioBird.username+'!  (');
                    links.append($('<a></a>')
                        .html(t('not me!'))
                        .attr('href', bburl('logout'))
                        .click(function () { BiblioBird.logout(); return false; })
                    );
                    links.append(')');
                }
                else {
                    links.append(t('Hello')+'!  ');
                    links.append($('<a></a>')
                        .html(t('Login'))
                        .attr('href', bburl('remote/login') +
                            (BiblioBird.localRelayUrl ? '?relay='+BiblioBird.localRelayUrl : ''))
                        .click(function (evt) { BiblioBird.openEmbedWindow(evt.target.href); return false; })
                    );
                    links.append(' '+t('or')+' ');
                    links.append($('<a></a>')
                        .html(t('register'))
                        .attr({
                            href: bburl('user/register'),
                            target: '_blank'
                        })
                    );
                    links.append(' '+t('on BiblioBird')+'.');
                }
            }
        });
        
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
            isRemoteIframe: false,
            bottomDock: null,

            // this is called at the beginning, but doesn't necessarily cause the object
            // to initialize itself.  That only happens if a .bibliobird-content node is
            // found on the page.
            start: function () {
                if (window.self !== window.top && window.self.location.hash) {
                    this._readHashOptions();
                }

                $('.bibliobird-content').each(function () {
                    BiblioBird.contentAreas.push(new ContentArea(this));
                });
            },

            _readHashOptions: function () {
                  var parts = (''+window.self.location.hash).split('&'),
                      values = {},
                      i, tmp;

                  for(i = 0; i < parts.length; i++) {
                      tmp = parts[i].split('=');
                      values[tmp[0]] = tmp[1];
                  }

                  if (values['#iam'] == 'bibliobird') {
                      // mark as living in the remote iframe on BiblioBird
                      this.isRemoteIframe = true;
                      // read language from the parent.
                      // TODO: this is what we want in the end, but right now, maybe not.
                      //this.lang = values.lang;
                  }
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

                if (!this.isRemoteIframe) {
                    this.bottomDock = new Dock();
                }
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
                /*
                $.each(this.contentAreas, function () {
                    this.refresh();
                });
                */
                if (this.bottomDock) {
                    this.bottomDock.rebuildLinks();
                }
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
                            localHost = window.self.location.protocol + '//' + window.self.location.host;
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

