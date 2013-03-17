<?php

/**
 * @file
 */

function theme_stormtimetracking_list($header, $timetrackings, $billing_duration) {
  $rows = array();
  foreach ($timetrackings as $timetracking) {
    $rows[] = array(
      l($timetracking->organization_title, 'node/'. $timetracking->organization_nid),
      l($timetracking->project_title, 'node/'. $timetracking->project_nid),
      l($timetracking->title, 'node/'. $timetracking->nid),
      format_date($timetracking->trackingdate, 'small'),
      array('data' => sprintf('%.2f', $timetracking->billing_duration), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($timetracking, $_GET) .'&nbsp;'. storm_icon_delete_node($timetracking, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  $o .= '<span style="font-weight: bold;">'. t('Total duration') .' : '. sprintf('%.2f', $billing_duration) .'</span> (hours)';
  return $o;
}

function theme_stormtimetracking_view($node, $teaser = FALSE, $page = FALSE) {
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node = node_prepare($node, $teaser);
  $l_pos = 1; // Used to increase the link position number (see issue 814820)

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );

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

  $node->content['group1']['task'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Task'), l($node->task_title, 'node/'. $node->task_nid)),
    '#weight' => 3,
  );

  $node->content['group1']['ticket'] = array(
    '#prefix' => '<div class="ticket">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Ticket'), l($node->ticket_title, 'node/'. $node->ticket_nid)),
    '#weight' => 4,
  );

  $node->content['body_field'] = array(
    '#prefix' => '<div class="stormbody">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Description'), $node->content['body']['#value']),
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : 0,
  );

  unset($node->content['body']);

  return $node;
}

function theme_stormtimetracking_list_form_report_reports() {
  $reports = array(
      'std' => t('Standard'),
      'for_organization' => t('For an organization'),
      'for_organization_w_task' => t('For an org. with tasks'),
      'for_project' => t('For a project'),
  );

  return $reports;
}

function theme_stormtimetracking_list_report($report, $language, $timetrackings) {
  switch ($report) {
    case 'std':
      $header = array(
        t('Organization', array(), $language),
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Billing duration (hours)', array(), $language));
      $total_billing_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          check_plain($timetracking->organization_title),
          check_plain($timetracking->project_title),
          check_plain($timetracking->title),
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->billing_duration), 'align' => 'right'),
        );
        $total_billing_duration += $timetracking->billing_duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
    break;
    case 'for_organization':
      $organization = node_load($_SESSION['stormtimetracking_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Billing duration (hours)', array(), $language));
      $total_billing_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          check_plain($timetracking->project_title),
          check_plain($timetracking->title),
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->billing_duration), 'align' => 'right'),
        );
        $total_billing_duration += $timetracking->billing_duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_organization_w_task':
      $organization = node_load($_SESSION['stormtimetracking_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Task', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Billing duration (hours)', array(), $language));
      $total_billing_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          check_plain($timetracking->project_title),
          check_plain($timetracking->task_title),
          check_plain($timetracking->title),
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->billing_duration), 'align' => 'right'),
        );
        $total_billing_duration += $timetracking->billing_duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_project':
      $organization = node_load($_SESSION['stormtimetracking_list_filter']['organization_nid']);
      $project = node_load($_SESSION['stormtimetracking_list_filter']['project_nid']);

      $header = array(
        t('Task', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_billing_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          check_plain($timetracking->task_title),
          check_plain($timetracking->title),
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->billing_duration), 'align' => 'right'),
        );
        $total_billing_duration += $timetracking->billing_duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
      $title .= t('Project : @project', array('@project' => $project->title), $language) .'<br />';
    break;
  }
  $footer = '<h3>'. t('Total billing duration : %total_billing_duration', array('%total_billing_duration' => format_plural($total_billing_duration, '1 hour', '@count hours', array(), $language)), $language) .'</h3>';
  return theme('storm_list_report', $header, $rows, $title, $footer);
}
