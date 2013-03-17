<?php

/**
 * @file
 */

function theme_stormtask_tasks($form) {
  if (count($form['tasks'])>0 && stormproject_access('update', $form['project']['#value'])) {
    drupal_add_tabledrag('tasks', 'match', 'parent', 'task-parent-nid', 'task-parent-nid', 'task-nid', TRUE);
    drupal_add_tabledrag('tasks', 'order', 'sibling', 'task-weight');
  }

  $header = array();
  $row = array();

  $header = $form['header']['#value'];
  unset($form['header']);

  foreach (element_children($form['tasks']) as $key) {
    $form['tasks'][$key]['task_parent-nid_'. $key]['#attributes']['class'] = 'task-parent-nid';
    $form['tasks'][$key]['task_nid_'. $key]['#attributes']['class'] = 'task-nid';
    $form['tasks'][$key]['task_weight_'. $key]['#attributes']['class'] = 'task-weight';
    $durationunit = $form['tasks'][$key]['task_durationunit_'. $key]['#value'];

    $data = array(
      theme('indentation', $form['tasks'][$key]['task_depth_'. $key]['#value']) . drupal_render($form['tasks'][$key]['task_description_'. $key]),
      drupal_render($form['tasks'][$key]['task_category_'. $key]),
      drupal_render($form['tasks'][$key]['task_status_'. $key]),
      drupal_render($form['tasks'][$key]['task_priority_'. $key]),
      array(
        'data' => sprintf('%.2f', drupal_render($form['tasks'][$key]['task_duration_'. $key])) .' '. substr($durationunit, 0, 1),
        'style' => 'text-align: right'),
      drupal_render($form['tasks'][$key]['task_parent-nid_'. $key]) . drupal_render($form['tasks'][$key]['task_nid_'. $key]),
      drupal_render($form['tasks'][$key]['task_weight_'. $key]),
      array(
        'data' => drupal_render($form['tasks'][$key]['task_operations_'. $key]),
        'class' => 'storm_list_operations',
      ),
    );
    if (!stormproject_access('update', $form['project']['#value'])) {
      unset($data[5]);
      unset($data[6]);
    }
    $row['data'] = $data;
    if (stormproject_access('update', $form['project']['#value'])) {
      $row['class'] = empty($row['class']) ? 'draggable' : $row['class'] .' draggable';
    }
    $rows[] = $row;
  }
  $o = theme('table', $header, $rows, array('id' => 'tasks'));
  return $o;
}

function theme_stormtask_list($header, $tasks) {
  $rows = array();
  foreach ($tasks as $task) {
    $rows[] = array(
      // Add classes to each row in table.
      // Allows customisation of row via css depending on the task's attributes (category, status, priority)
      'class' =>
        'storm_category_'. preg_replace('/[^a-zA-Z]/', '', $task->taskcategory) . ' ' .
        'storm_status_'. preg_replace('/[^a-zA-Z]/', '', $task->taskstatus) . ' ' .
        'storm_priority_'. preg_replace('/[^a-zA-Z]/', '', $task->taskpriority),
      'data' => array(
        l($task->organization_title, 'node/'. $task->organization_nid),
        l($task->project_title, 'node/'. $task->project_nid),
        l($task->title, 'node/'. $task->nid) . theme('mark', node_mark($task->nid, $task->changed)),
        storm_icon('category_'. $task->taskcategory, storm_attribute_value('Task category', $task->taskcategory)),
        storm_icon('status_'. $task->taskstatus, storm_attribute_value('Task status', $task->taskstatus)),
        storm_icon('priority_'. $task->taskpriority, storm_attribute_value('Task priority', $task->taskpriority)),
        array(
          'data' => storm_icon_edit_node($task, $_GET) .'&nbsp;'. storm_icon_delete_node($task, $_GET),
          'class' => 'storm_list_operations',
        ),
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormtask_view($node, $teaser = FALSE, $page = FALSE) {
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node = node_prepare($node, $teaser);
  $l_pos = 1; // Used to increase the link position number (see issue 814820)

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks">',
    '#suffix' => '</div>',
    '#weight' => -25,
  );

  $node->content['links']['expenses'] = theme('storm_link', 'stormtask', 'stormexpense', $node->nid, $l_pos++);
  $node->content['links']['notes'] = theme('storm_link', 'stormtask', 'stormnote', $node->nid, $l_pos++);
  $node->content['links']['tickets'] = theme('storm_link', 'stormtask', 'stormticket', $node->nid, $l_pos++);
  $node->content['links']['timetrackings'] = theme('storm_link', 'stormtask', 'stormtimetracking', $node->nid, $l_pos++);

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

  $node->content['group1']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Project'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => 2,
  );

  $node->content['group1']['stepno'] = array(
    '#prefix' => '<div class="stepno">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Step no.'), check_plain($node->stepno)),
    '#weight' => 3,
  );

  $node->content['group2'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -19,
  );

  $node->content['group2']['category'] = array(
    '#prefix' => '<div class="category">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Category'), check_plain(storm_attribute_value('Task category', $node->taskcategory))),
    '#weight' => 1,
  );

  $node->content['group2']['status'] = array(
    '#prefix' => '<div class="storm_status">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Status'), check_plain(storm_attribute_value('Task status', $node->taskstatus))),
    '#weight' => 2,
  );

  $node->content['group2']['priority'] = array(
    '#prefix' => '<div class="priority">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Priority'), check_plain(storm_attribute_value('Task priority', $node->taskpriority))),
    '#weight' => 3,
  );

  $node->content['group5'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group5') : -16,
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
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : -15,
  );
  unset($node->content['body']);

  return $node;
}
