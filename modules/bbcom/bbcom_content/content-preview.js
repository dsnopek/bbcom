
Drupal.behaviors.bbcom_content_preview = function (context) {
  if (!Drupal.onBeforeUnload.callbackExists('bbcom_content_preview')) {
    Drupal.onBeforeUnload.addCallback('bbcom_content_preview', function () {
      return Drupal.t('This is just a preview!  You are about to leave this page without saving.');
    });
  }

  // if the form is submitted, remove this onbeforeunload callback
  $('#node-form').submit(function (evt) {
    Drupal.onBeforeUnload.removeCallback('bbcom_content_preview');
  });
}

