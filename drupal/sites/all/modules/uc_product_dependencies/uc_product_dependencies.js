// $Id: uc_product_dependencies.js,v 1.1.2.2 2010/04/09 21:40:25 joelstein Exp $

Drupal.behaviors.uc_product_dependencies_settings_form = function(context) {
  
  // When that value changes, use animation for transition
  $('input[id^=edit-uc-product-dependencies-behavior-type]').click(function() {
    if ($('input[id^=edit-uc-product-dependencies-behavior-type]:checked').val() > 0) {
      $('#edit-uc-product-dependencies-status-field-wrapper').show();
    } else {
      $('#edit-uc-product-dependencies-status-field-wrapper').hide();
    }
  });
  
};
