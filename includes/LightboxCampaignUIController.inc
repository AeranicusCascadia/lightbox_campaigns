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

  /**
   * {@inheritdoc}
   */
  public function operationForm($form, &$form_state, $entity, $op) {
    $form = parent::operationForm($form, $form_state, $entity, $op);

    // Check for Rules configuration dependencies and prevent entity delete if
    // any exist.
    /* @var \LightboxCampaignEntity $entity */
    if ($op == 'delete' && !empty($entity->rules)) {
      $form['rules'] = array(
        '#type' => 'fieldset',
        '#title' => t('Related Rules configurations'),
        '#description' => t('This Campaign is configured to be displayed by
          the Rules configurations listed below. The Campaign 
          <strong>cannot be deleted</strong> until it has been removed from all
          Rules configurations.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#weight' => -1,
      );
      $form['rules']['items'] = $entity->getRulesListRenderArray();
      unset($form['description']);
      $form['actions']['submit']['#disabled'] = TRUE;
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function operationFormValidate($form, &$form_state) {
    if ($form_state['op'] == 'delete') {
      $entity = $form_state[$this->entityType];
      if (!empty($entity->rules)) {
        form_set_error('form', t(
          'This Campaign cannot be deleted because it is associated with Rules 
          configurations.'
        ));
      }
    }
    parent::operationFormValidate($form, $form_state);
  }

}
