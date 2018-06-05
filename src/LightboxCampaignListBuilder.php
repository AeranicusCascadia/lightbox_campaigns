<?php

namespace Drupal\lightbox_campaigns;

use Drupal\Core\Datetime\DrupalDateTime;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\lightbox_campaigns\Entity\LightboxCampaign;

/**
 * Provides a listing of lightbox campaign entities.
 *
 * @ingroup lightbox_campaigns
 */
class LightboxCampaignListBuilder extends EntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['label'] = $this->t('Campaign');
    $header['enabled'] = $this->t('Enabled');
    $header['start'] = $this->t('Start');
    $header['end'] = $this->t('End');

    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   *
   * @var LightboxCampaign $entity
   */
  public function buildRow(EntityInterface $entity) {
    /* @var DrupalDateTime $start */
    $start = $entity->start->date;
    if (!is_null($start)) {
      $start = \Drupal::service('date.formatter')
        ->format($start->getTimestamp());
    }
    /* @var DrupalDateTime $end */
    $end = $entity->end->date;
    if (!is_null($end)) {
      $end = \Drupal::service('date.formatter')
        ->format($end->getTimestamp());
    }

    if ($entity->enable->value) {
      $enabled = t('Yes');
    }
    else {
      $enabled = t('No');
    }

    $row['label'] = $entity->label();
    $row['enabled'] = $enabled;
    $row['start'] = $start;
    $row['end'] = $end;

    return $row + parent::buildRow($entity);
  }

}
