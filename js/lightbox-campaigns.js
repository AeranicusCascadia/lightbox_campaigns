/**
 * @file
 * Lightbox Campaigns module featherlight handling.
 */

(function ($) {

  /**
   * Loads a campaign lightbox, if it exists.
   *
   * @type {Drupal~behavior}
   *
   * @prop {Drupal~behaviorAttach} attach
   *   Attaches featherlight campaign load on page load.
   */

  Drupal.behaviors.lightboxCampaignsDisplay = {
    attach: function (context, settings) {
      $('[data-lightbox-campaigns-entity-id]', context).once('lightboxCampaignsDisplay').each(function () {
        let $id = $(this).data('lightbox-campaigns-entity-id');

        if (settings.lightbox_campaigns[$id]) {
          let $resetTimer = settings.lightbox_campaigns[$id].reset_timer;
          let $key = 'lightboxCampaignsCampignShown-' + $id;
          let $last = localStorage.getItem($key);

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
}(jQuery));
