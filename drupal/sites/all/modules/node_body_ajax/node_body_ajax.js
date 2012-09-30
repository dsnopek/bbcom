
Drupal.behaviors.node_body_ajax = function (context) {
  $('.node-body-ajax', context).each(function () {
    var elem = this,
        conf = Drupal.settings.node_body_ajax[elem.id],
        url  = Drupal.settings.node_body_ajax.url + '/' + conf.nid;

    $.ajax({
      url: url,
      data: {teaser: conf.teaser ? 1 : 0, page: conf.page ? 1 : 0},
      dataType: 'json',
      success: function (data) {
        $(elem).html(data.content)
        Drupal.attachBehaviors(elem);
      }
    });
  });

  // attempt to make text unselectable
  if (Drupal.settings.node_body_ajax.disable_selection) {
    $('body', context)
      .attr('unselectable', 'on')
      .css('user-select', 'none')
      .css('MozUserSelect', 'none')
      .css('webkitUserSelect', 'none')
      .bind('selectstart', function () { return false; });
  }
};

