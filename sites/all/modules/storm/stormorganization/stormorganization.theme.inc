<?php

/**
 * @file
 */

function theme_stormorganization_list($header, $organizations) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  $countries = storm_attributes_bydomain('Country');
  foreach ($organizations as $key => $organization) {
    $rows[] = array(
      l($organization->title, 'node/'. $organization->nid),
      check_plain($countries['values'][$organization->country]),
      array(
        'data' => storm_icon_edit_node($organization, $_GET) .'&nbsp;'. storm_icon_delete_node($organization, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormorganization_view($node, $teaser = FALSE, $page = FALSE) {
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node = node_prepare($node, $teaser);
  $l_pos = 1; // Used to increase the link position number (see issue 814820)

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );

  $node->content['links']['expenses'] = theme('storm_link', 'stormorganization', 'stormexpense', $node->nid, $l_pos++);
  $node->content['links']['invoices'] = theme('storm_link', 'stormorganization', 'storminvoice', $node->nid, $l_pos++);
  $node->content['links']['notes'] = theme('storm_link', 'stormorganization', 'stormnote', $node->nid, $l_pos++);
  $node->content['links']['people'] = theme('storm_link', 'stormorganization', 'stormperson', $node->nid, $l_pos++);
  $node->content['links']['projects'] = theme('storm_link', 'stormorganization', 'stormproject', $node->nid, $l_pos++);
  $node->content['links']['tasks'] = theme('storm_link', 'stormorganization', 'stormtask', $node->nid, $l_pos++);
  $node->content['links']['tickets'] = theme('storm_link', 'stormorganization', 'stormticket', $node->nid, $l_pos++);
  $node->content['links']['timetrackings'] = theme('storm_link', 'stormorganization', 'stormtimetracking', $node->nid, $l_pos++);

  $node->content['group2'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -19,
  );

  $node->content['group3'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group3') : -18,
  );

  $node->content['group3']['address'] = array(
    '#prefix' => '<div class="address">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Address'), check_plain($node->address)),
    '#weight' => 1,
  );

  $node->content['group3']['city'] = array(
    '#prefix' => '<div class="city">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('City'), check_plain($node->city)),
    '#weight' => 2,
  );

  $node->content['group3']['provstate'] = array(
    '#prefix' => '<div class="provstate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Province / State'), check_plain($node->provstate)),
    '#weight' => 3,
  );

  $node->content['group3'] ['country'] = array(
    '#prefix' => '<div class="country">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Country'), check_plain($node->country)),
    '#weight' => 4,
  );

  $node->content['group3'] ['zip'] = array(
    '#prefix' => '<div class="zip">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Zip'), check_plain($node->zip)),
    '#weight' => 5,
  );

  $node->content['group4'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group4') : -17,
  );

  $node->content['group4']['phone'] = array(
    '#prefix' => '<div class="phone">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Phone'), check_plain($node->phone)),
    '#weight' => 1,
  );

  $node->content['group4']['www'] = array(
    '#prefix' => '<div class="www">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('WWW'), l($node->www, 'http://'. $node->www, array('absolute' => TRUE))),
    '#weight' => 2,
  );

  $node->content['group4']['email'] = array(
    '#prefix' => '<div class="email">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Email'), l($node->email, 'mailto:'. $node->email, array('absolute' => TRUE))),
    '#weight' => 3,
  );

  $node->content['group5'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group5') : -16,
  );

  $node->content['group5']['currency'] = array(
    '#prefix' => '<div class="currency">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Currency'), check_plain($node->currency)),
    '#weight' => 1,
  );

  $languages = language_list('language', TRUE);
  $languages_options = array();
  foreach ($languages as $language_code => $language) {
    $languages_options[$language_code] = $language->name;
  }

  $node->content['group5']['language'] = array(
    '#prefix' => '<div class="language">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Language'), check_plain($languages_options[$node->orglanguage])),
    '#weight' => 2,
  );

  $node->content['group5'] ['taxid'] = array(
    '#prefix' => '<div class="taxid">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Tax ID'), check_plain($node->taxid)),
    '#weight' => 3,
  );

  $node->content['body_field'] = array(
    '#prefix' => '<div class="stormbody">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Note'), $node->content['body']['#value']),
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : 0,
  );

  unset($node->content['body']);

  return $node;
}
