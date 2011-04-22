// $Id: views_filters_reset.js,v 1.1.2.2 2010/05/23 12:59:01 kratib Exp $
(function ($) {
// START jQuery

Drupal.vfr = Drupal.vfr || {};

Drupal.behaviors.vfr = function(context) {
  $('form#'+Drupal.settings.vfr.form_id+' input#edit-reset', context).click(function() {
    if (Drupal.settings.vfr.url) {
      window.location = Drupal.settings.vfr.url;
    }
    else {
      $('form#'+Drupal.settings.vfr.form_id, context).clearForm();
      $('form#'+Drupal.settings.vfr.form_id, context).submit();
    }
  });
}

Drupal.vfr.ajaxViewResponse = function(target, response) {
  $('form#'+Drupal.settings.vfr.form_id).replaceWith(response.exposed_form);
  Drupal.attachBehaviors($('form#'+Drupal.settings.vfr.form_id).parent());
}

// END jQuery
})(jQuery);

