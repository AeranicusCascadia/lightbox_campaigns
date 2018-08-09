<?php

/**
 * @file
 * Administrative functions for the Lightbox Campaigns module.
 */

/**
 * Entity add/edit form for the Lightbox Campaign entity.
 *
 * @param array $form
 *   The form render array.
 * @param array $form_state
 *   Form state data.
 * @param \LightboxCampaignEntity $campaign
 *   The Lightbox Campaign entity being modified.
 * @param string $op
 *   The operation being performed.
 *
 * @return array
 *   A form render array.
 */
function lightbox_campaigns_campaign_form(array $form, array &$form_state, \LightboxCampaignEntity $campaign, $op = 'edit') {
  $form = array();

  if (!empty($campaign->rules)) {
    $items = array();

    /* @var \RulesReactionRule $rule */
    foreach ($campaign->rules as $rule) {
      // The "path" property is a custom addition.
      /* @see lightbox_campaigns_active_rules() */
      $items[] = l($rule->label(), $rule->path);
    }

    $form['rules'] = array(
      '#type' => 'fieldset',
      '#title' => t('Related Rules configurations'),
      '#description' => t('This Campaign is configured to be displayed by
        the Rules configurations listed below. The Campaign will only be 
        displayed by these Rules configurations.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['rules']['items'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The name of the Campaign (internal only).'),
    '#default_value' => $campaign->label(),
    '#required' => TRUE,
  );

  $form['enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Basic campaign activation.'),
    '#default_value' => $campaign->enable,
  );

  $form['content'] = array(
    '#type' => 'text_format',
    '#title' => t('Lightbox content'),
    '#description' => t('The campaign content to be displayed in the 
      lightbox.'),
    '#default_value' => $campaign->content,
    '#format' => $campaign->content_format,
    '#required' => TRUE,
  );

  $intervals = array(
    60 * 5, 60 * 10, 60 * 15, 60 * 30,
    3600, 3600 * 2, 3600 * 3, 3600 * 4, 3600 * 5, 3600 * 6, 3600 * 12,
    86400, 86400 * 2, 86400 * 3, 86400 * 4, 86400 * 5, 86400 * 6,
    604800,
  );
  $options = array();
  foreach ($intervals as $interval) {
    $options[$interval] = format_interval($interval);
  }

  $form['reset'] = array(
    '#type' => 'select',
    '#title' => t('Reset timer'),
    '#description' => t('Time to wait before showing the lightbox again to
      the same user.'),
    '#default_value' => $campaign->reset,
    '#empty_value' => 0,
    '#options' => $options,
  );

  $form['date_start'] = array(
    '#type' => 'date_select',
    '#title' => t('Start date'),
    '#description' => t('Date and time to start running the campaign.'),
    '#default_value' => '',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => 'm/d/Y h:i a',
    '#date_label_position' => 'none',
    '#date_increment' => 1,
    '#date_year_range' => '-3:+3',
  );
  if (!empty($campaign->date_start)) {
    $form['date_start']['#default_value'] = format_date(
      $campaign->date_start,
      'custom',
      DATE_FORMAT_DATETIME
    );
  }

  $form['date_end'] = array(
    '#type' => 'date_select',
    '#title' => t('End date'),
    '#description' => t('Date and time to stop running the campaign.'),
    '#default_value' => '',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => 'm/d/Y h:i a',
    '#date_label_position' => 'none',
    '#date_increment' => 1,
    '#date_year_range' => '-3:+3',
  );
  if (!empty($campaign->date_end)) {
    $form['date_end']['#default_value'] = format_date(
      $campaign->date_end,
      'custom',
      DATE_FORMAT_DATETIME
    );
  }

  // Use date_popup for date fields if the module is enabled.
  if (module_exists('date_popup')) {
    $form['date_start']['#type'] = 'date_popup';
    $form['date_end']['#type'] = 'date_popup';
  }

  // Visibility settings.
  // This is mostly taken from the core block and node modules.
  /* @see block_admin_configure() */
  /* @see node_form_block_admin_configure_alter() */
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );

  // Per-path visibility.
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 0,
  );

  $form['visibility']['path']['only'] = array(
    '#type' => 'radios',
    '#title' => t('Show Campaign on specific pages'),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
    '#default_value' => $campaign->visibility['path']['only'],
  );
  $form['visibility']['path']['pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#default_value' => $campaign->visibility['path']['pages'],
    '#description' => t("Specify pages by using their paths. Enter one path 
      per line. The '*' character is a wildcard. Example paths are %blog for the
      blog page and %blog-wildcard for every personal blog. %front is the front 
      page.", array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>',
      )
    ),
  );

  // Per content-type visibility.
  $form['visibility']['node_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 5,
  );
  $form['visibility']['node_type']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show Campaign for specific content types'),
    '#default_value' => $campaign->visibility['node_type']['types'],
    '#options' => node_type_get_names(),
    '#description' => t('Show this block only on pages that display 
      content of the given type(s). If you select no types, there will be no 
      type-specific limitation.'),
  );

  // Per-role visibility.
  $form['visibility']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 10,
  );
  $form['visibility']['role']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show Campaign for specific roles'),
    '#default_value' => $campaign->visibility['role']['roles'],
    '#options' => array_map('check_plain', user_roles()),
    '#description' => t('Show this block only for the selected role(s). If 
      you select no roles, the block will be visible to all users.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for lightbox_campaigns_campaign_form().
 *
 * @param array $form
 *   The form render array.
 * @param array $form_state
 *   Form state data.
 *
 * @see lightbox_campaigns_campaign_form()
 */
function lightbox_campaigns_campaign_form_submit(array &$form, array &$form_state) {
  $campaign = entity_ui_form_submit_build_entity($form, $form_state);
  $campaign->save();
  drupal_set_message(t('Lightbox campaign %label saved!', array(
    '%label' => $campaign->label(),
  )));
  $form_state['redirect'] = 'admin/content/lightbox_campaigns';
}
