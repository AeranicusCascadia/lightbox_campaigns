<?php

/**
 * @file
 * Entity controller for the Lightbox Campaign entity.
 */

class LightboxCampaignEntityController extends EntityAPIControllerExportable {

  /**
   * @inheritdoc
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    return $build;
  }

  /**
   * @inheritdoc
   */
  public function load($ids = array(), $conditions = array()) {
    $entities = parent::load($ids, $conditions);
    return $entities;
  }

  /**
   * @inheritdoc
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    // Serialize visibility data.
    if (is_array($entity->visibility)) {
      $entity->visibility = serialize($entity->visibility);
    }

    return parent::save($entity, $transaction);
  }

  /**
   * @inheritdoc
   */
  public function export($entity, $prefix = '') {
    // Remove calculated "rules" field to prevent export.
    unset($entity->rules);
    return parent::export($entity, $prefix);
  }

}
