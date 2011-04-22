
// This function called by facebook's javascript when it is loaded.
window.fbAsyncInit = function() {
  //debugger; // debug
  // @TODO - make these settings completely customizable.
  var settings = {xfbml: true};
  if (Drupal.settings.fb.apikey) {
    settings.apiKey = Drupal.settings.fb.apikey;
    settings.status = true;
    settings.cookie = true;
  }

  FB.init(settings);

  // Async function to complete init.
  FB.getLoginStatus(function(response) {
    var status = {'session' : response.session, 'response': response};
    jQuery.event.trigger('fb_init', status);  // Trigger event for third-party modules.

    FB_JS.sessionChange(response);

    // Use FB.Event to detect Connect login/logout.
    FB.Event.subscribe('auth.sessionChange', FB_JS.sessionChange);

    // Q: what the heck is "edge.create"? A: the like button was clicked.
    FB.Event.subscribe('edge.create', FB_JS.edgeCreate);

    // Other events that may be of interest...
    //FB.Event.subscribe('auth.login', FB_JS.debugHandler);
    //FB.Event.subscribe('auth.logout', FB_JS.debugHandler);
    //FB.Event.subscribe('auth.statusChange', FB_JS.debugHandler);
    //FB.Event.subscribe('auth.sessionChange', FB_JS.debugHandler);
  });
};

FB_JS = function(){};

/**
 * Helper parses URL params.
 *
 * http://jquery-howto.blogspot.com/2009/09/get-url-parameters-values-with-jquery.html
 */
FB_JS.getUrlVars = function(href)
{
  var vars = [], hash;
  var hashes = href.slice(href.indexOf('?') + 1).split('&');
  for(var i = 0; i < hashes.length; i++)
  {
    hash = hashes[i].split('=');
    vars[hash[0]] = hash[1];
    if (hash[0] != '_fb_js_fbu')
      vars.push(hashes[i]); // i.e. "foo=bar"
  }
  return vars;
}

/**
 * Reload the current page, whether on canvas page or facebook connect.
 */
FB_JS.reload = function(destination) {
  // Determine fbu.
  var session = FB.getSession();
  var fbu;
  if (session != null)
    fbu = session.uid;
  else
    fbu = 0;

  // Avoid infinite reloads
  ///@TODO - does not work on iframe because facebook does not pass url args to canvas frame when cookies not accepted.  http://forum.developers.facebook.net/viewtopic.php?id=77236
  var vars = FB_JS.getUrlVars(window.location.href);
  if (vars._fb_js_fbu == fbu) {
    return; // Do not reload (again)
  }

  // Determine where to send user.
  if (typeof(destination) != 'undefined' && destination) {
    // Use destination passed in.
  }
  else if (typeof(Drupal.settings.fb.reload_url) != 'undefined') {
    destination = Drupal.settings.fb.reload_url;
  }
  else {
    destination = window.location.href;
  }

  // Split and parse destination
  var path;
  if (destination.indexOf('?') == -1) {
    vars = [];
    path = destination;
  }
  else {
    vars = FB_JS.getUrlVars(destination);
    path = destination.substr(0, destination.indexOf('?'));
  }

  // Add _fb_js_fbu to params before reload.
  vars.push('_fb_js_fbu=' + fbu);

  // Use window.top for iframe canvas pages.
  destination = path + '?' + vars.join('&');
  window.top.location = destination;
};

// Facebook pseudo-event handlers.
FB_JS.sessionChange = function(response) {
  if (response.status == 'unknown') {
    // @TODO can we test if third-party cookies are disabled?
  }

  var status = {'changed': false, 'fbu': null, 'session': response.session, 'response' : response};

  if (response.session) {
    status.fbu = response.session.uid;
    if (Drupal.settings.fb.fbu != status.fbu) {
      // A user has logged in.
      status.changed = true;
    }
  }
  else if (Drupal.settings.fb.fbu) {
    // A user has logged out.
    status.changed = true;

    // Sometimes Facebook's invalid cookies are left around.  Let's try to clean up their crap.
    // Commented out because facebook's JS apparently does this now.
    // FB_JS.deleteCookie('fbs_' + Drupal.settings.fb.apikey, '/', '');
  }

  if (status.changed) {
    // fbu has changed since server built the page.
    jQuery.event.trigger('fb_session_change', status);

    // Remember the fbu.
    Drupal.settings.fb.fbu = status.fbu;
  }

};

FB_JS.edgeCreate = function(href, widget) {
  var status = {'href': href};
  FB_JS.ajaxEvent('edge.create', status);
};

// Helper function for developers.
FB_JS.debugHandler = function(response) {
  debugger;
};

// JQuery pseudo-event handler.
FB_JS.sessionChangeHandler = function(context, status) {
  // Pass data to ajax event.
  var data = {
    'event_type': 'session_change'
  };

  if (status.session) {
    data.fbu = status.session.uid;
    // Suppress facebook-controlled session.
    data.fb_session_handoff = true;
  }
  FB_JS.ajaxEvent(data.event_type, data);
  // No need to call window.location.reload().  It will be called from ajaxEvent, if needed.
};


// Helper to pass events via AJAX.
// A list of javascript functions to be evaluated is returned.
FB_JS.ajaxEvent = function(event_type, data) {
  if (Drupal.settings.fb.ajax_event_url) {

    // Session data helpful on canvas pages (but tricks fb_settings.inc on connect pages).
    if (Drupal.settings.fb.page_type &&
        Drupal.settings.fb.page_type != 'connect') {
      data.session = JSON.stringify(FB.getSession());
    }

    // Other values to always include.
    data.apikey = FB._apiKey;
    if (Drupal.settings.fb.controls) {
      data.fb_controls = Drupal.settings.fb.controls;
    }

    jQuery.post(Drupal.settings.fb.ajax_event_url + '/' + event_type, data,
                function(js_array, textStatus, XMLHttpRequest) {
                  //debugger; // debug
                  if (js_array.length > 0) {
                    for (var i = 0; i < js_array.length; i++) {
                      eval(js_array[i]);
                    }
                  }
                  else {
                    if (event_type == 'session_change') {
                      // No instructions from ajax, reload entire page.
                      FB_JS.reload();
                    }
                  }
                }, 'json');
  }
};

// Delete a cookie.
// ??? Still needed?  Facebook's JS SDK may take care of this now.
FB_JS.deleteCookie = function( name, path, domain ) {
  document.cookie = name + "=" +
    ( ( path ) ? ";path=" + path : "") +
    ( ( domain ) ? ";domain=" + domain : "" ) +
    ";expires=Thu, 01-Jan-1970 00:00:01 GMT";
};

// Test the FB settings to see if we are still truly connected to facebook.
FB_JS.sessionSanityCheck = function() {
  if (!Drupal.settings.fb.checkSemaphore) {
    Drupal.settings.fb.checkSemaphore=true;
    FB.api('/me', function(response) {
      if (response.id != Drupal.settings.fb.fbu) {
        // We are no longer connected.
        var status = {'changed': true, 'fbu': null, 'check_failed': true};
        jQuery.event.trigger('fb_session_change', status);
      }
      Drupal.settings.fb.checkSemaphore=null;
    });
  }
};




Drupal.behaviors.fb = function(context) {
  // Respond to our jquery pseudo-events
  var events = jQuery(document).data('events');
  if (!events || !events.fb_session_change) {
    jQuery(document).bind('fb_session_change', FB_JS.sessionChangeHandler);
  }

  if (typeof(FB) == 'undefined') {
    // Include facebook's javascript.  @TODO - determine locale dynamically.
    jQuery.getScript(Drupal.settings.fb.js_sdk_url);
  }
  else {
    $(context).each(function() {
      var elem = $(this).get(0);
      FB.XFBML.parse(elem);
    });

    FB_JS.sessionSanityCheck();
  }

  // Markup with class .fb_show should be visible if javascript is enabled.  .fb_hide should be hidden.
  jQuery('.fb_hide', context).hide();
  jQuery('.fb_show', context).show();

};

