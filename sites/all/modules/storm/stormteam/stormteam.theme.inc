<?php

/**
 * @file
 * Theme functions for the Storm Team module
 */

/**
 * @function
 * Theme function for the Storm Team node view
 */
function theme_stormteam_view($node, $teaser = FALSE, $page = FALSE) {
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node = node_prepare($node, $teaser);
  $type = node_get_types('type', $node);

  /* $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );*/

  $node->content['group1'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -20,
  );

  // Add links to each team member title
  if (isset($node->members_array) && is_array($node->members_array)) {
    foreach ($node->members_array as $nid => $name) {
      $member = node_load($nid);
      if (($member->type == 'stormperson' && stormperson_access('view', $nid)) || ($member->type == 'stormorganization' && stormorganization_access('view', $nid))) {
        if (!empty($node->members_deactivated_array) && array_key_exists($nid, $node->members_deactivated_array)) {
          $name .= " (". t('disabled') .")";
        }
        $node->members_array[$nid] = l($name, 'node/'. $nid);
    }
  }

    $node->content['group1']['members'] = array(
      '#prefix' => '<div class="members">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', t('Members'), implode(", ", $node->members_array)),
    );
  }

  $node->content['body_field'] = array(
    '#prefix' => '<div class="stormbody">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', $type->body_label, $node->content['body']['#value']),
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : 0,
  );
  unset($node->content['body']);

  return $node;
}
