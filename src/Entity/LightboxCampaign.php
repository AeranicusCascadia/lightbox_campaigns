<?php

namespace Drupal\lightbox_campaigns\Entity;

use Drupal\Core\Datetime\DrupalDateTime;
use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityChangedTrait;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\Node\Entity\Node;
use Drupal\Node\Entity\NodeType;
use Drupal\lightbox_campaigns\LightboxCampaignInterface;
use Drupal\user\UserInterface;

/**
 * Defines the lightbox_campaign entity.
 *
 * @ingroup lightbox_campaigns
 *
 * @ContentEntityType(
 *   id = "lightbox_campaign",
 *   label = @Translation("Lightbox Campaign"),
 *   handlers = {
 *     "access" = "Drupal\lightbox_campaigns\LightboxCampaignAccessControlHandler",
 *     "view_builder" = "Drupal\lightbox_campaigns\LightboxCampaignViewBuilder",
 *     "list_builder" = "Drupal\lightbox_campaigns\LightboxCampaignListBuilder",
 *     "form" = {
 *       "add" = "Drupal\lightbox_campaigns\Form\LightboxCampaignForm",
 *       "edit" = "Drupal\lightbox_campaigns\Form\LightboxCampaignForm",
 *       "delete" = "Drupal\lightbox_campaigns\Form\LightboxCampaignDeleteForm"
 *     }
 *   },
 *   list_cache_contexts = { "theme", "user.roles", "url" },
 *   base_table = "lightbox_campaigns",
 *   admin_permission = "administer lightbox campaigns",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *     "uuid" = "uuid"
 *   },
 *   links = {
 *     "canonical" = "/lightbox_campaigns/{lightbox_campaign}",
 *     "edit-form" = "/admin/config/system/lightbox_campaigns/{lightbox_campaign}/edit",
 *     "delete-form" = "/admin/config/system/lightbox_campaigns/{lightbox_campaign}/delete",
 *     "collection" = "/admin/config/system/lightbox_campaigns/list"
 *   },
 *   field_ui_base_route = "lightbox_campaigns.lightbox_campaign_settings",
 * )
 */
class LightboxCampaign extends ContentEntityBase implements LightboxCampaignInterface {

  use EntityChangedTrait;

  /**
   * {@inheritdoc}
   *
   * When a new entity instance is added, set the user_id entity reference to
   * the current user as the creator of the instance.
   */
  public static function preCreate(EntityStorageInterface $storage_controller, array &$values) {
    parent::preCreate($storage_controller, $values);
    $values += [
      'user_id' => \Drupal::currentUser()->id(),
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getCreatedTime() {
    return $this->get('created')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getChangedTime() {
    return $this->get('changed')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getOwner() {
    return $this->get('user_id')->entity;
  }

  /**
   * {@inheritdoc}
   */
  public function getOwnerId() {
    return $this->get('user_id')->target_id;
  }

  /**
   * {@inheritdoc}
   */
  public function setOwnerId($uid) {
    $this->set('user_id', $uid);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function setOwner(UserInterface $account) {
    $this->set('user_id', $account->id());
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields['id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('ID'))
      ->setDescription(t('The ID of the Lightbox Campaign entity.'))
      ->setReadOnly(TRUE);
    $fields['uuid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The UUID of the Lightbox Campaign entity.'))
      ->setReadOnly(TRUE);

    // Campaign name.
    $fields['label'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Campaign name'))
      ->setDescription(t('The name of the Lightbox Campaign.'))
      ->setRequired(TRUE)
      ->setTranslatable(TRUE)
      ->setRevisionable(TRUE)
      ->setSettings([
        'max_length' => 255,
        'text_processing' => 0,
      ])
      ->setDefaultValue(NULL)
      ->setDisplayOptions('form', [
        'type' => 'string_textfield',
        'weight' => -6,
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Enabled.
    $fields['enable'] = BaseFieldDefinition::create('boolean')
      ->setLabel(t('Enabled'))
      ->setDescription(t('Basic campaign lightbox activation.'))
      ->setRevisionable(TRUE)
      ->setDefaultValue(TRUE)
      ->setDisplayOptions('form', [
        'type' => 'boolean_checkbox',
        'settings' => ['display_label' => TRUE]
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Body.
    $fields['body'] = BaseFieldDefinition::create('text_long')
      ->setLabel(t('Lightbox content'))
      ->setDescription(t('The content to be displayed on the lightbox.'))
      ->setRevisionable(TRUE)
      ->setTranslatable(TRUE)
      ->setDefaultValue(NULL)
      ->setDisplayOptions('form', array(
        'type'   => 'text_textarea'
      ))
      ->setDisplayOptions('view', [
        'label' => 'hidden',
        'type' => 'text_default',
      ])
      ->setRequired(TRUE)
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Reset timer.
    $intervals = [1800, 3600, 10800, 21600, 43200, 86400, 604800];
    $fields['reset'] = BaseFieldDefinition::create('list_integer')
      ->setLabel(t('Reset timer'))
      ->setDescription(t('Reset timer for the lightbox.'))
      ->setRevisionable(TRUE)
      ->setDefaultValue(NULL)
      ->setSettings([
        'allowed_values' => array_map(
          [\Drupal::service('date.formatter'), 'formatInterval'],
          array_combine($intervals, $intervals)
        ),
      ])
      ->setDisplayOptions('form', [
        'type' => 'options_select',
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Start date.
    $fields['start'] = BaseFieldDefinition::create('datetime')
      ->setLabel(t('Start date/time'))
      ->setDescription(t('The campaign start date and time.'))
      ->setRevisionable(TRUE)
      ->setDefaultValue(NULL)
      ->setDisplayOptions('form', [
        'type' => 'datetime_default',
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // End date.
    $fields['end'] = BaseFieldDefinition::create('datetime')
      ->setLabel(t('End date/time'))
      ->setDescription(t('The campaign end date and time.'))
      ->setRevisionable(TRUE)
      ->setDefaultValue(NULL)
      ->setDisplayOptions('form', [
        'type' => 'datetime_default',
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Visibility: Node Types.
    $node_types = \Drupal::service('entity.manager')
      ->getStorage('node_type')
      ->loadMultiple();
    $node_types_options = [];
    /* @var NodeType $node_type */
    foreach ($node_types as $node_type) {
      $node_types_options[$node_type->id()] = $node_type->label();
    }

    $fields['node_types'] = BaseFieldDefinition::create('list_string')
      ->setLabel(t('Content Types'))
      ->setDescription(t('Show only on the following content types:'))
      ->setRevisionable(TRUE)
      ->setDefaultValue('_none')
      ->setSettings(['allowed_values' => $node_types_options])
      ->setCardinality(-1)
      ->setDisplayOptions('form', ['type' => 'options_buttons'])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Visibility: Roles.
    $fields['roles'] = BaseFieldDefinition::create('list_string')
      ->setLabel(t('Roles'))
      ->setDescription(t('Show only to users with the following roles:'))
      ->setRevisionable(TRUE)
      ->setDefaultValue('_none')
      ->setSettings(['allowed_values' => user_role_names()])
      ->setCardinality(-1)
      ->setDisplayOptions('form', ['type' => 'options_buttons'])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Visibility: Pages.
    $fields['paths'] = BaseFieldDefinition::create('string_long')
      ->setLabel(t('Paths'))
      ->setDescription(t('Specify pages by using their paths. Enter one 
        path per line. The \' * \' character is a wildcard. An example path is 
        /user/* for every user page. <front> is the front page.'))
      ->setRevisionable(TRUE)
      ->setDefaultValue(NULL)
      ->setDisplayOptions('form', array(
        'type'   => 'textarea'
      ))
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    // Visibility: Negate Paths.
    $fields['paths_negate'] = BaseFieldDefinition::create('boolean')
      ->setLabel(t('Negate Paths'))
      ->setDescription(t('Show or hide for the specified paths.'))
      ->setRevisionable(TRUE)
      ->setDefaultValue(0)
      ->setSettings([
        'off_label' => t('Show only for the specified paths'),
        'on_label' => t('Hide for the specified paths')
      ])
      ->setDisplayOptions('form', ['type' => 'options_buttons'])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    $fields['created'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Created'))
      ->setDescription(t('The time that the entity was created.'));
    $fields['changed'] = BaseFieldDefinition::create('changed')
      ->setLabel(t('Changed'))
      ->setDescription(t('The time that the entity was last edited.'));

    return $fields;
  }

  /**
   * Create a clean array of visibility options data.
   *
   * @return array
   */
  public function getVisibilitySettings() {
    $node_types = [];
    foreach ($this->get('node_types')->getValue() as $type) {
      $node_types[$type['value']] = $type['value'];
    }

    $roles = [];
    foreach ($this->get('roles')->getValue() as $role) {
      $roles[$role['value']] = $role['value'];
    }

    $visibility = [
      'node_types' => $node_types,
      'roles' => $roles,
      'paths' => [
        'list' => explode("\r\n", $this->get('paths')->value),
        'negate' => (bool) $this->get('paths_negate')->value
      ]
    ];
    return $visibility;
  }

  /**
   * Determine if the campaign should be displayed.
   *
   * If the entity is enabled and within specified date constraints, this method
   * will check any other visibility settings (user roles, node types, and
   * paths) based on the current context.
   *
   * @return bool $display
   *   TRUE if the campaign should be displayed, FALSE otherwise.
   */
  public function shouldDisplay() {
    // Never load on admin routes.
    if (\Drupal::service('router.admin_context')->isAdminRoute()) {
      return FALSE;
    }

    // Check enabled.
    $enabled = (bool) $this->get('enable')->value;

    // Check date/time constraints.
    $now = new DrupalDateTime('now');
    $start = $this->get('start')->date;
    if (is_null($start)) {
      $start = $now;
    }
    $end = $this->get('end')->date;
    if (is_null($end)) {
      $end = $now;
    }
    $within_date_constraints = $now >= $start && $now <= $end;

    $display = $enabled && $within_date_constraints;
    if ($display) {
      $visibility = $this->getVisibilitySettings();

      // Check current user roles.
      if (!empty($visibility['roles'])) {
        $roles = array_intersect(
          \Drupal::service('current_user')->getRoles(),
          $visibility['roles']
        );
        $display = !empty($roles);
      }

      // Check current node type.
      if ($display && !empty($visibility['node_types'])) {
        /* @var Node $node */
        $node = \Drupal::service('current_route_match')->getParameter('node');
        if (!empty($node)) {
          if (!isset($visibility['node_types'][$node->getType()])) {
            $display = FALSE;
          }
        }
        else {
          $display = FALSE;
        }
      }

      // Check path.
      if (!empty($visibility['paths']['list'])) {
        $list = array_unique($visibility['paths']['list']);
        $matches = implode("\r\n", $list);

        // Matches may be empty e.g. if a user enters blank lines in the form.
        if (!empty($matches)) {
          $path = \Drupal::service('router.request_context')->getPathInfo();
          if (\Drupal::service('path.matcher')->matchPath($path, $matches)) {
            if ($visibility['paths']['negate']) {
              $display = FALSE;
            }
          }
          elseif ($display) {
            $display = FALSE;
          }
        }
      }
    }

    return $display;
  }

}
