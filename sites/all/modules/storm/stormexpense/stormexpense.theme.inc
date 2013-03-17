<?php

/**
 * @file
 */

function theme_stormexpense_list($header, $expenses, $totals) {
  $rows = array();
  foreach ($expenses as $expense) {
    $rows[] = array(
      l($expense->organization_title, 'node/'. $expense->organization_nid),
      l($expense->project_title, 'node/'. $expense->project_nid),
      l($expense->title, 'node/'. $expense->nid),
      format_date($expense->expensedate, 'custom', 'Y-m-d'),
      array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($expense, $_GET) .'&nbsp;'. storm_icon_delete_node($expense, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);

  $header = array(
    array(
      'data' => t('Amount'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => variable_get('storm_tax1_name', 'VAT'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => variable_get('storm_tax2_name', 'Tax 2'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => t('Total'),
      'style' => 'text-align: right;',
    ),
  );

  $rows = array();
  $rows[] = array(
    array(
      'data' => sprintf('%.2f', $totals->t_amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->t_tax1),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->t_tax2),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->t_total),
      'style' => 'text-align: right;',
    ),
  );

  $o .= theme('table', $header, $rows);

  return $o;
}

function theme_stormexpense_view($node, $teaser = FALSE, $page = FALSE) {
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');

  $node = node_prepare($node, $teaser);
  $l_pos = 1; // Used to increase the link position number (see issue 814820)

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );
  // Code to create invoice auto_add link
  if (module_exists('storminvoice') && user_access('Storm invoice: add')) {
    $v = '';
    $v .= '<strong>'. t('Invoice this item:') .'</strong>';
    $v .= '<ul>';
    $v .= '<li>'. l(t('Add to new invoice'), 'storm/invoice/auto_add/new/'. $node->nid) .'</li>';
    $v .= '<li>'. l(t('Add to existing invoice'), 'storm/invoice/auto_add/existing/'. $node->nid) .'</li>';
    $v .= '</ul>';

    $node->content['links']['auto_invoice'] = array(
      '#prefix' => '<dt id="storminvoices" class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => $v,
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

  $node->content['group2'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -19,
  );

  $node->content['group2']['expensedate'] = array(
    '#prefix' => '<div class="expensedate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Date'), format_date($node->expensedate, 'custom', 'Y-m-d')),
    '#weight' => 1,
  );

  $node->content['group2']['provider'] = array(
    '#prefix' => '<div class="provider">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Provider'), $node->provider_title),
    '#weight' => 2,
  );

  $node->content['group3'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group3') : -18,
  );

  $node->content['group3']['amount'] = array(
    '#prefix' => '<div class="amount">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Amount'), sprintf('%.2f', $node->amount)),
    '#weight' => 1,
  );

  $node->content['group4'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group4') : -17,
  );

  if ($node->tax1) {
    $node->content['group4']['tax1'] = array(
      '#prefix' => '<div class="tax">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', variable_get('storm_tax1_name', 'VAT'), sprintf('%.2f', $node->tax1)),
      '#weight' => 1,
    );
  }

  if ($node->tax2) {
    $node->content['group4']['tax2'] = array(
      '#prefix' => '<div class="tax">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', variable_get('storm_tax2_name', 'Tax 2'), sprintf('%.2f', $node->tax2)),
      '#weight' => 2,
    );
  }

  if ($node->tax1 || $node->tax2) {
    $node->content['group4']['total'] = array(
      '#prefix' => '<div class="total">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', t('Total'), sprintf('%.2f', $node->total)),
      '#weight' => 3,
    );
  }

  $node->content['body_field'] = array(
    '#prefix' => '<div class="stormbody">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Description'), $node->content['body']['#value']),
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : -16,
  );
  unset($node->content['body']);

  return $node;
}

function theme_stormexpense_list_form_report_reports() {
  $reports = array(
      'std' => t('Standard'),
      'for_organization' => t('For an organization'),
      'for_organization_w_task' => t('For an org. with tasks'),
      'for_project' => t('For a project'),
  );

  return $reports;
}

function theme_stormexpense_list_report($report, $language, $expenses) {
  switch ($report) {
    case 'std':
      $header = array(
        t('Organization', array(), $language),
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Total', array(), $language));
      $expenses_total = 0;
      $rows = array();
      foreach ($expenses as $expense) {
        $rows[] = array(
          check_plain($expense->organization_title),
          check_plain($expense->project_title),
          check_plain($expense->title),
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. t('Expenses report', array(), $language) .'</h2><br />';
    break;
    case 'for_organization':
      $organization = node_load($_SESSION['stormexpense_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Total', array(), $language));
      $expenses_total = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          check_plain($expense->project_title),
          check_plain($expense->title),
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. t('Expenses report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_organization_w_task':
      $organization = node_load($_SESSION['stormexpense_list_filter']['organization_nid']);
      $header = array(
        t('Project', 'expense', array(), $language),
        t('Task', 'expense', array(), $language),
        t('Title', 'expense', array(), $language),
        t('Date', 'expense', array(), $language),
        t('Total', 'expense', array(), $language));
      $total_duration = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          check_plain($expense->project_title),
          check_plain($expense->task_title),
          check_plain($expense->title),
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. t('Expenses report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_project':
      $organization = node_load($_SESSION['stormexpense_list_filter']['organization_nid']);
      $project = node_load($_SESSION['stormexpense_list_filter']['project_nid']);

      $header = array(
        t('Task', 'expense', array(), $language),
        t('Title', 'expense', array(), $language),
        t('Date', 'expense', array(), $language),
        t('Total', 'expense', array(), $language));
      $total_duration = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          check_plain($expense->task_title),
          check_plain($expense->title),
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. t('Expenses report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
      $title .= t('Project : @project', array('@project' => $project->title), $language) .'<br />';
    break;
  }
  $footer = '<h3>'. t('Expenses total : %expenses_total' , array('%expenses_total' => format_plural($expenses_total, '1', '@count', array(), $language)), $language) .'</h3>';
  return theme('storm_list_report', $header, $rows, $title, $footer);
}
