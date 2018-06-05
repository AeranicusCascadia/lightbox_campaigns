<?php

namespace Drupal\lightbox_campaigns\Form;

use Drupal\Core\Entity\ContentEntityForm;
use Drupal\Core\Language\Language;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class LightboxCampaignFormBase.
 *
 * @ingroup lightbox_campaigns
 */
class LightboxCampaignForm extends ContentEntityForm {

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    /* @var $entity \Drupal\lightbox_campaigns\Entity\LightboxCampaign */
    $form = parent::buildForm($form, $form_state);
    $entity = $this->entity;

    $this->formatVisibilitySettingsForm($form);

    $form['langcode'] = [
      '#title' => $this->t('Language'),
      '#type' => 'language_select',
      '#default_value' => $entity->getUntranslated()->language()->getId(),
      '#languages' => Language::STATE_ALL,
    ];

    return $form;
  }

  /**
   * Place the campaign visibility settings in a vertical tabs group.
   *
   * @param array $form
   */
  private function formatVisibilitySettingsForm(array &$form) {
    $form['visibility_tabs'] = [
      '#type' => 'vertical_tabs',
      '#title' => $this->t('Visibility'),
      '#parents' => ['visibility_tabs'],
    ];

    $form['node_types_details'] = [
      '#type' => 'details',
      '#title' => $form['node_types']['widget']['#title'],
      '#description' => $form['node_types']['widget']['#description'],
      '#group' => 'visibility_tabs',
    ];
    unset($form['node_types']['widget']['#description']);
    $form['node_types_details']['node_types'] = $form['node_types'];
    unset($form['node_types']);

    $form['roles_details'] = [
      '#type' => 'details',
      '#title' => $form['roles']['widget']['#title'],
      '#description' => $form['roles']['widget']['#description'],
      '#group' => 'visibility_tabs',
    ];
    unset($form['roles']['widget']['#description']);
    $form['roles_details']['roles'] = $form['roles'];
    unset($form['roles']);

    $form['paths_details'] = [
      '#type' => 'details',
      '#title' => $form['paths']['widget']['#title'],
      '#group' => 'visibility_tabs',
    ];
    $form['paths_details']['paths'] = $form['paths'];

    unset(
      $form['paths_negate']['widget']['#title'],
      $form['paths_negate']['widget']['#description'],
      // Remove N/A option.
      $form['paths_negate']['widget']['#options']['_none']
    );
    $form['paths_details']['paths_negate'] = $form['paths_negate'];
    unset($form['paths'], $form['paths_negate']);
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $entity = $this->getEntity();
    $status = $entity->save();

    if ($status == SAVED_UPDATED) {
      \Drupal::service('messenger')->addMessage(
        $this->t(
          '%label has been updated.',
          ['%label' => $this->entity->label(),]
        )
      );
    }
    else {
      \Drupal::service('messenger')->addMessage(
        $this->t(
          '%label has been added.',
          ['%label' => $this->entity->label(),]
        )
      );
    }

    $form_state->setRedirect('entity.lightbox_campaign.collection');
  }

}
