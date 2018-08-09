<?php

/**
 * @file
 * Entity UI controller for the Lightbox Campaign entity.
 */

/**
 * Class LightboxCampaignUIController.
 */
class LightboxCampaignUIController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   */
  public function entityFormSubmitBuildEntity($form, &$form_state) {
    $campaign = parent::entityFormSubmitBuildEntity($form, $form_state);

    // Define default exportable entity fields.
    $campaign->status = ENTITY_CUSTOM;
    $campaign->module = 'lightbox_campaigns';

    // Separate content and format data.
    $campaign->content = $form_state['values']['content']['value'];
    $campaign->content_format = $form_state['values']['content']['format'];

    // Convert dates to timestamps.
    if (!empty($campaign->date_start)) {
      $campaign->date_start = strtotime($campaign->date_start);
    }
    if (!empty($campaign->date_end)) {
      $campaign->date_end = strtotime($campaign->date_end);
    }

    // Remove unselected items from visibility settings.
    $campaign->visibility['node_type']['types'] = array_filter(
      $form_state['values']['visibility']['node_type']['types'],
      'is_string'
    );
    $campaign->visibility['role']['roles'] = array_filter(
      $form_state['values']['visibility']['role']['roles'],
      'is_string'
    );

    return $campaign;
  }

}
