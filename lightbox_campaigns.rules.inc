<?php

/**
 * @file
 * Rules API hooks for the Lightbox Campaigns module.
 */

/**
 * Implements hook_rules_action_info().
 *
 * @see hook_rules_action_info()
 */
function lightbox_campaigns_rules_action_info() {
  return array(

    /* @see lightbox_campaigns_rules_action_display_campaign() */
    'lightbox_campaigns_rules_action_display_campaign' => array(
      'label' => t('Display a campaign'),
      'parameter' => array(
        'lightbox_campaign' => array(
          'type' => 'integer',
          'label' => t('Lightbox Campaign'),
          'description' => t('Select the Lightbox Campaign to display.
            <strong>Campaign display settings will still apply</strong>. For 
            example, if a campaign has a date range set, this Rule will only
            display the campaign within that date range.'),
          /* @see lightbox_campaigns_campaign_options_list() */
          'options list' => 'lightbox_campaigns_campaign_options_list',
        ),
      ),
      'module' => 'lightbox_campaigns',
      'group' => t('Lightbox Campaigns'),

    ),
  );
}

/**
 * Campaigns options list callback for the "Display a campaign" Rules action.
 *
 * @see lightbox_campaigns_rules_action_info()
 */
function lightbox_campaigns_campaign_options_list() {
  $options = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'lightbox_campaigns_campaign')
    ->propertyCondition('enable', TRUE);
  $results = $query->execute();
  if (!empty($results) && !empty($results['lightbox_campaigns_campaign'])) {
    $campaigns = entity_load(
      'lightbox_campaigns_campaign',
      array_keys($results['lightbox_campaigns_campaign'])
    );

    /* @var \LightboxCampaignEntity $campaign */
    foreach ($campaigns as $campaign) {
      $options[$campaign->identifier()] = $campaign->label();
    }
  }

  return $options;
}

/**
 * Action: Display a campaign.
 *
 * Assigns an array in the session and adds the Campaign ID. This session array
 * is evaluated in lightbox_campaigns_page_build() so it may not be picked up
 * until the _next_ page load if the triggering event fires this action after
 * that hook is executed.
 *
 * @param string $campaign_id
 *   A string representation of the Campaign ID to display.
 *
 * @see lightbox_campaigns_page_build()
 */
function lightbox_campaigns_rules_action_display_campaign($campaign_id) {
  if (!isset($_SESSION[__FUNCTION__])) {
    $_SESSION[__FUNCTION__] = array();
  }
  $_SESSION[__FUNCTION__][$campaign_id] = $campaign_id;
}
