<?php

/**
 * @file
 */

function theme_stormproject_list($header, $projects) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  foreach ($projects as $key => $project) {
    $rows[] = array(
      storm_icon('projectcategory_'. $project->projectcategory, storm_attribute_value('Project category', $project->projectcategory)),
      l($project->organization_title, 'node/'. $project->organization_nid),
      l($project->title, 'node/'. $project->nid) . theme('mark', node_mark($project->nid, $project->changed)),
      storm_icon('status_'. $project->projectstatus, storm_attribute_value('Project status', $project->projectstatus)),
      storm_icon('priority_'. $project->projectpriority, storm_attribute_value('Project priority', $project->projectpriority)),
      array(
        'data' => storm_icon_edit_node($project, $_GET) .'&nbsp;'. storm_icon_delete_node($project, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormproject_view($node, $teaser = FALSE, $page = FALSE) {
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node = node_prepare($node, $teaser);
  $l_pos = 1; // Used to increase the link position number (see issue 814820)

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );

  $node->content['links']['expenses'] = theme('storm_link', 'stormproject', 'stormexpense', $node->nid, $l_pos++);
  $node->content['links']['invoices'] = theme('storm_link', 'stormproject', 'storminvoice', $node->nid, $l_pos++);
  $node->content['links']['notes'] = theme('storm_link', 'stormproject', 'stormnote', $node->nid, $l_pos++);
  $node->content['links']['tasks'] = theme('storm_link', 'stormproject', 'stormtask', $node->nid, $l_pos++);
  $node->content['links']['tickets'] = theme('storm_link', 'stormproject', 'stormticket', $node->nid, $l_pos++);
  $node->content['links']['timetrackings'] = theme('storm_link', 'stormproject', 'stormtimetracking', $node->nid, $l_pos++);

  // Code to create invoice auto_add link
  if (module_exists('storminvoice')) {

    $node->content['links']['auto_invoice'] = array(
      '#prefix' => variable_get('storm_icons_display', TRUE) ? '<dt id="storminvoices" class="stormcomponent">' : '<dt class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => theme('storminvoice_autoadd_links', $node->nid, $node->billable, $node->billed),
      '#weight' => $l_pos++,
    );
  }


  $node->content['group1'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group1') : -20,
  );

  $node->content['group1']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Organization'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => 1,
  );

  $node->content['group2'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -19,
  );

  $node->content['group2']['projectcategory'] = array(
    '#prefix' => '<div class="projectcategory">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Category'), check_plain(storm_attribute_value('Project category', $node->projectcategory))),
    '#weight' => 1,
  );

  $node->content['group2']['projectstatus'] = array(
    '#prefix' => '<div class="projectstatus">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Status'), check_plain(storm_attribute_value('Project status', $node->projectstatus))),
    '#weight' => 2,
  );

  $node->content['group2']['projectpriority'] = array(
    '#prefix' => '<div class="projectpriority">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Priority'), check_plain(storm_attribute_value('Project priority', $node->projectpriority))),
    '#weight' => 3,
  );

  $node->content['group5'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group5') : -16,
  );

  $node->content['group5']['manager'] = array(
    '#prefix' => '<div class="manager">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Project Manager'), l($node->manager_title, 'node/'. $node->manager_nid)),
    '#weight' => 1,
  );

  $node->content['group5']['assigned'] = array(
    '#prefix' => '<div class="assigned">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Assigned to'), l($node->assigned_title, 'node/'. $node->assigned_nid)),
    '#weight' => 2,
  );

  $node->content['body_field'] = array(
    '#prefix' => '<div class="stormbody">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Description'), $node->content['body']['#value']),
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : -18,
  );
  unset($node->content['body']);

  return $node;
}
