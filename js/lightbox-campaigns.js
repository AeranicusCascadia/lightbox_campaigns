/**
 * @file
 * Lightbox Campaigns module featherlight handling.
 */

(function ($, Drupal, drupalSettings) {

  'use strict';

  /**
   * Loads a campaign lightbox, if it exists.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attaches featherlight campaign load on page load.
   */

  Drupal.behaviors.lightboxCampaignsDisplay = {
    attach: function (context) {
      $('[data-lightbox-campaigns-entity-id]', context).once('lightboxCampaignsDisplay').each(function () {
        var $id = $(this).data('lightbox-campaigns-entity-id');

        if (drupalSettings.lightbox_campaigns[$id]) {
          var $resetTimer = drupalSettings.lightbox_campaigns[$id].reset_timer;
          var $key = 'lightboxCampaignsCampignShown-' + $id;
          var $last = localStorage.getItem($key);

          if ($.now() - $last > $resetTimer * 1000) {
            $.featherlight($(this), {
              'afterOpen': function () {
                localStorage.setItem($key, $.now())
              }
            });
          }
        }
      });
    }
  };

})(jQuery, Drupal, drupalSettings);
