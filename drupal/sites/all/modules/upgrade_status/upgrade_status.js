// $Id: upgrade_status.js,v 1.3 2010/09/04 23:53:07 sun Exp $

/**
 * Attach collapsible behaviour.
 */
Drupal.behaviors.upgradeStatus = function (context) {
  $('table.upgrade-status .collapse-icon:not(.processed)', context).each(function () {
    $(this).addClass('upgrade-status-processed')
      .click(function () {
        this.src = (this.src.match(/collapsed.png$/) ? this.src.replace(/collapsed.png$/, 'expanded.png') : this.src.replace(/expanded.png$/, 'collapsed.png'));
        $('.details-wrapper', this.parentNode).slideToggle('fast');
      })
      .parent().children('.details-wrapper').hide();
  });
};

