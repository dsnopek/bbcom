// $Id: views_filters_autosubmit.js,v 1.1.2.2 2010/05/24 16:36:13 kratib Exp $
(function ($) {
// START jQuery

Drupal.vfas = Drupal.vfas || {};

Drupal.behaviors.vfas = function(context) {
  $('form#'+Drupal.settings.vfas.form_id+':not(.vfas-processed)', context).each(function() {
    var self = this;
    $(self).addClass('vfas-processed');
    $('#'+Drupal.settings.vfas.submit_id, self).hide();
    $('div.views-exposed-widget input:not('+Drupal.settings.vfas.exceptions+')', self).change(function() {
      $(self).submit();
    });
    $('div.views-exposed-widget select:not('+Drupal.settings.vfas.exceptions+')', self).change(function() {
      $(self).submit();
    });
  });
}

// END jQuery
})(jQuery);

