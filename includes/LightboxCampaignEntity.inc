<?php

/**
 * @file
 * Lightbox Campaign entity class.
 */

/**
 * Class LightboxCampaignEntity.
 */
class LightboxCampaignEntity extends Entity {

  // @codingStandardsIgnoreStart

  /**
   * Campaign title (internal only).
   *
   * @var string
   */
  public $title;

  /**
   * Campaign enabled indicator.
   *
   * @var bool
   */
  public $enable;

  /**
   * Campaign content to be display in a lightbox.
   *
   * @var string
   */
  public $content;

  /**
   * Text format of the Campaign content.
   *
   * @var string
   */
  public $content_format;

  /**
   * The reset timer (in seconds) for the Campaign.
   *
   * @var int
   */
  public $reset;

  /**
   * Campaign start date and time (unix timestamp).
   *
   * @var int
   */
  public $date_start;

  /**
   * Campaign end date and time (unix timestamp).
   *
   * @var int
   */
  public $date_end;

  /**
   * Visibility settings data.
   *
   * This value is serialized as a string in the database so it may be a string
   * or array depending on when it is being used.
   *
   * @var string|array
   *
   * @see setUp()
   * @see getVisibilitySettings()
   */
  public $visibility;

  /**
   * Entity status flag. Required for exportable entities.
   *
   * @var int
   */
  public $status;

  /**
   * Entity module definition. Required for exportable entities.
   *
   * @var string
   */
  public $module;

  /**
   * All Rules config entities that depend on the Campaign.
   *
   * @var array
   */
  public $rules;

  // @codingStandardsIgnoreEnd

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();
    $this->rules = $this->getDependantRules();
    $this->visibility = $this->getVisibilitySettings();
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return NULL;
  }

  /**
   * Convenience method to check if the Campaign is enabled.
   *
   * @return bool
   *   The value of the Campaign "enable" property.
   */
  public function enabled() {
    return (bool) $this->enable;
  }

  /**
   * Check if the Campaign is within its date constraints.
   *
   * @param null|int $timestamp
   *   (optional) A timestamp to check against. Defaults to time().
   *
   * @return bool
   *   TRUE if no date constraints are set or the value of $timestamp is within
   *   the constraints. FALSE otherwise.
   */
  public function withinDateConstraints($timestamp = NULL) {
    if (empty($timestamp)) {
      $timestamp = time();
    }

    $start = $this->date_start;
    if (is_null($start)) {
      $start = $timestamp;
    }
    $end = $this->date_end;
    if (is_null($end)) {
      $end = $timestamp;
    }

    return $timestamp >= $start && $timestamp <= $end;
  }

  /**
   * Determine if the campaign should be displayed.
   *
   * The Campaign will be displayed if it is:
   *  - enabled,
   *  - within configured date constraints,
   *  - passing all visibility settings, and
   *  - not attached to any Rules configurations (when $ignore_rules is FALSE).
   *
   * @param bool $ignore_rules
   *   If TRUE, do not check for Rules configurations dependencies.
   *
   * @return bool
   *   TRUE if the campaign should be displayed, FALSE otherwise.
   */
  public function shouldDisplay($ignore_rules = FALSE) {
    $display = $this->enabled()
      && $this->withinDateConstraints()
      && (empty($this->rules) || $ignore_rules);

    // Check visibility settings.
    if ($display) {
      $visibility = $this->getVisibilitySettings();
      global $user;
      $account = user_load($user->uid);
      $alias = request_path();
      $node = menu_get_object('node', 1, current_path());

      // Check user roles.
      if (!empty($visibility['role']['roles'])) {
        $roles = array_intersect_key(
          $account->roles,
          $visibility['role']['roles']
        );
        $display = !empty($roles);
      }

      // Check node type.
      if ($display && !empty($visibility['node_type']['types'])) {
        if (!empty($node)) {
          if (!isset($visibility['node_type']['types'][$node->type])) {
            $display = FALSE;
          }
        }
        else {
          $display = FALSE;
        }
      }

      // Check path.
      if (!empty($visibility['path']['pages']) && !empty($alias)) {
        if (drupal_match_path($alias, $visibility['path']['pages'])) {
          if (!$visibility['path']['only']) {
            $display = FALSE;
          }
        }
        elseif ($visibility['path']['only']) {
          $display = FALSE;
        }
      }
    }

    return $display;
  }

  /**
   * Retrieve all active Rules config entities that depend on the Campaign.
   *
   * @return array
   *   An array of active Rules config entities by ID.
   */
  public function getDependantRules() {
    $rules = lightbox_campaigns_active_rules();
    if (isset($rules[$this->identifier()])) {
      return $rules[$this->identifier()];
    }
    return array();
  }

  /**
   * Retrieve visibility settings or default as an array.
   *
   * @return array
   *   Visibility settings data.
   */
  public function getVisibilitySettings() {
    $visibility = $this->visibility;

    if (empty($visibility)) {
      $visibility = array(
        'path' => array('only' => 0, 'pages' => ''),
        'node_type' => array('types' => array()),
        'role' => array('roles' => array()),
      );
    }
    elseif (!is_array($visibility)) {
      $visibility = unserialize($this->visibility);
    }

    return $visibility;
  }

}
