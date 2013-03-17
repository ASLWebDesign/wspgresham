<?php

/**
 * @file
 */

function stormexpense_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormexpense_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormexpense_list_filter']['project_nid']);
    unset($_SESSION['stormexpense_list_filter']['task_nid']);
    unset($_SESSION['stormexpense_list_filter']['ticket_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormexpense_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormexpense_list_filter']['organization_nid'] = $p->organization_nid;

    unset($_SESSION['stormexpense_list_filter']['task_nid']);
    unset($_SESSION['stormexpense_list_filter']['ticket_nid']);
  }

  if (array_key_exists('task_nid', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['task_nid'] != $_GET['task_nid']) {
      $_SESSION['stormexpense_list_filter']['task_nid'] = $_GET['task_nid'];
    }

    $t = node_load($_GET['task_nid']);
    $_SESSION['stormexpense_list_filter']['organization_nid'] = $t->organization_nid;
    $_SESSION['stormexpense_list_filter']['project_nid'] = $t->project_nid;
    unset($_SESSION['stormexpense_list_filter']['ticket_nid']);
  }

  if (array_key_exists('ticket_nid', $_GET)) {
    if ($_SESSION['stormexpense_list_filter']['ticket_nid'] != $_GET['ticket_nid']) {
      $_SESSION['stormexpense_list_filter']['ticket_nid'] = $_GET['ticket_nid'];
    }

    $ti = node_load($_GET['ticket_nid']);
    $_SESSION['stormexpense_list_filter']['organization_nid'] = $ti->organization_nid;
    $_SESSION['stormexpense_list_filter']['project_nid'] = $ti->project_nid;
    $_SESSION['stormexpense_list_filter']['task_nid'] = $ti->task_nid;
  }

  $i = new stdClass();
  $i->type = 'stormexpense';

  $report_attrs = array(
    'onclick' => "storm_popup(this, 'expense_list_form_report', 'Expenses', 300, 150, 'stormexpense_list_form_report', 'l'); return false;",
  );

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'sex.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sex.project_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Date'),
      'field' => 'sex.expensedate',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Total'),
      'style' => 'text-align: right;'
    ),
    array(
      'data' => storm_icon_l('report', '', t('Reports'), '', array(), $report_attrs) .'&nbsp;'. storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.*,  sex.*, nre.format FROM {node} AS n
    INNER JOIN {stormexpense} AS sex ON n.vid=sex.vid
    INNER JOIN {node_revisions} AS nre ON n.vid = nre.vid
    WHERE n.status=1 AND n.type='stormexpense' ";

  $s_totals = "SELECT SUM(amount) AS t_amount, SUM(tax1) AS t_tax1, SUM(tax2) AS t_tax2, SUM(total) AS t_total FROM
  {node} AS n INNER JOIN {stormexpense} AS sex ON n.vid=sex.vid WHERE n.status=1 AND n.type='stormexpense' ";

  $where = array();
  $args = array();
  $filterfields = array();

  if (isset($_SESSION['stormexpense_list_filter']['organization_nid']) && $_SESSION['stormexpense_list_filter']['organization_nid'] != 0) {
    $where[] = 'sex.organization_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['organization_nid'];
    $filterfields[] = 'Organization';
  }
  if (isset($_SESSION['stormexpense_list_filter']['project_nid']) && $_SESSION['stormexpense_list_filter']['project_nid'] != 0) {
    $where[] = 'sex.project_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['project_nid'];
    $filterfields[] = 'Project';
  }
  if (isset($_SESSION['stormexpense_list_filter']['task_nid']) && $_SESSION['stormexpense_list_filter']['task_nid'] != 0) {
    $where[] = 'sex.task_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['task_nid'];
    $filterfields[] = 'Task';
  }
  if (isset($_SESSION['stormexpense_list_filter']['ticket_nid']) && $_SESSION['stormexpense_list_filter']['ticket_nid'] != 0) {
    $where[] = 'sex.ticket_nid=%d';
    $args[] = $_SESSION['stormexpense_list_filter']['ticket_nid'];
    $filterfields[] = 'Ticket';
  }
  if (isset($_SESSION['stormexpense_list_filter']['datefrom']) && $_SESSION['stormexpense_list_filter']['datefrom']['day'] != -1) {
    $datefrom = $_SESSION['stormexpense_list_filter']['datefrom'];
    $datefrom['hour'] = 0;
    $datefrom['minute'] = 0;
    $where[] = 'sex.expensedate>=%d';
    $args[] = _storm_datetime_to_gmtimestamp($datefrom);
    $filterfields[] = 'Date';
  }
  if (isset($_SESSION['stormexpense_list_filter']['title']) && $_SESSION['stormexpense_list_filter']['title'] != '') {
    $where[] = "LOWER(n.title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormexpense_list_filter']['title'];
    $filterfields[] = 'Title';
  }
  if (isset($_SESSION['stormexpense_list_filter']['provider_title']) && $_SESSION['stormexpense_list_filter']['provider_title'] != '') {
    $where[] = "LOWER(sex.provider_title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormexpense_list_filter']['provider_title'];
    $filterfields[] = 'Provider';
  }

  $itemsperpage = isset($_SESSION['stormexpense_list_filter']['itemsperpage']) ? $_SESSION['stormexpense_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);

  if (isset($_SESSION['stormexpense_list_filter']['user']) && $_SESSION['stormexpense_list_filter']['user'] != '') {
    $expenseuser = user_load(array('name' => $_SESSION['stormexpense_list_filter']['user']));
    if ($expenseuser === FALSE) $expenseuser->uid = -1;
    $where[] = 'n.uid=%d';
    $args[] = $expenseuser->uid;
    $filterfields[] = 'User';
  }

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $o = drupal_get_form('stormexpense_list_filter', $filterdesc);

  $s = stormexpense_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $_SESSION['stormexpense_list_filter']['sql'] = $s . $tablesort;
  $_SESSION['stormexpense_list_filter']['args'] = $args;

  $expenses = array();
  while ($item = db_fetch_object($r)) {
    $expenses[] = $item;
  }

  $s_totals = stormexpense_access_sql($s_totals, $where);
  $s_totals = db_rewrite_sql($s_totals);
  $r = db_query($s_totals, $args);
  $totals = db_fetch_object($r);

  $o .= theme('stormexpense_list', $header, $expenses, $totals);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  $o .= drupal_get_form('stormexpense_list_form_report');
  print theme('page', $o);
}

function stormexpense_list_filter(&$form_state, $filterdesc = 'Filter') {
  $organization_nid = isset($_SESSION['stormexpense_list_filter']['organization_nid']) ? $_SESSION['stormexpense_list_filter']['organization_nid'] : 0;
  $project_nid = isset($_SESSION['stormexpense_list_filter']['project_nid']) ? $_SESSION['stormexpense_list_filter']['project_nid'] : 0;
  $task_nid = isset($_SESSION['stormexpense_list_filter']['task_nid']) ? $_SESSION['stormexpense_list_filter']['task_nid'] : 0;
  $ticket_nid = isset($_SESSION['stormexpense_list_filter']['ticket_nid']) ? $_SESSION['stormexpense_list_filter']['ticket_nid'] : 0;
  $itemsperpage = isset($_SESSION['stormexpense_list_filter']['itemsperpage']) ? $_SESSION['stormexpense_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['stormexpense_list_filter']['itemsperpage'] = $itemsperpage;
  $title = isset($_SESSION['stormexpense_list_filter']['title']) ? $_SESSION['stormexpense_list_filter']['title'] : '';
  $provider_title = isset($_SESSION['stormexpense_list_filter']['provider_title']) ? $_SESSION['stormexpense_list_filter']['provider_title'] : '';

  $datefrom = isset($_SESSION['stormexpense_list_filter']['datefrom']) ? $_SESSION['stormexpense_list_filter']['datefrom'] : NULL;

  $expenseuser = isset($_SESSION['stormexpense_list_filter']['user']) ? $_SESSION['stormexpense_list_filter']['user'] : '';

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => $filterdesc,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#theme' => 'stormexpense_list_filter',
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON
  sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => t('All')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormticket_organization_project_task_tickets(this, 'edit-project-nid', 'edit-task-nid', 'edit-ticket-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormproject} AS spr ON spr.vid=n.vid
  WHERE n.status=1 AND spr.organization_nid=%d AND n.type='stormproject' ORDER BY n.title";
  $s = stormproject_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid);
  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[$project->nid] = $project->title;
  }
  $projects = array(0 => t('All')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormticket_project_task_tickets(this, 'edit-organization-nid', 'edit-task-nid', 'edit-ticket-nid', true, 'All')"),
  );

  $tree = _stormtask_get_tree($project_nid);
  $tasks = _stormtask_plain_tree($tree);
  $tasks = array(0 => t('All')) + $tasks;
  $form['filter']['group1']['task_nid'] = array(
    '#type' => 'select',
    '#title' => t('Task'),
    '#default_value' => $task_nid,
    '#options' => $tasks,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormticket_task_tickets(this, 'edit-organization-nid', 'edit-project-nid', 'edit-ticket-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormticket} AS sti ON sti.vid=n.vid
  WHERE n.status=1 AND n.type='stormticket' AND sti.organization_nid=%d AND sti.project_nid=%d AND sti.task_nid=%d ORDER BY title ";
  $s = stormticket_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid, $project_nid, $task_nid);
  $tickets = array();
  while ($ticket = db_fetch_object($r)) {
    $tickets[$ticket->nid] = $ticket->title;
  }
  $tickets = array(0 => t('All')) + $tickets;
  $form['filter']['group1']['ticket_nid'] = array(
    '#type' => 'select',
    '#title' => t('Ticket'),
    '#default_value' => $ticket_nid,
    '#process' => array('storm_dependent_select_process'),
    '#options' => $tickets,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -19,
  );

  $form['filter']['group2']['datefrom'] = array(
    '#type' => 'dateext',
    '#title' => t('Date from'),
    '#withnull' => TRUE,
    '#default_value' => $datefrom,
  );

  $form['filter']['group2']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#size' => 25,
    '#default_value' => $expenseuser,
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#size' => 40,
    '#default_value' => $title,
  );

  $form['filter']['group3']['provider_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Provider'),
    '#size' => 40,
    '#default_value' => $provider_title,
    '#autocomplete_path' => 'storm/expenses/provider_autocomplete',
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group4']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormexpense_list_filter_filter'),
  );

  $form['filter']['group4']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormexpense_list_filter_reset'),
  );

  $form['filter']['group4']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormexpense_list_filter_filter($form, &$form_state) {
  $_SESSION['stormexpense_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormexpense_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormexpense_list_filter']['task_nid'] = $form_state['values']['task_nid'];
  $_SESSION['stormexpense_list_filter']['ticket_nid'] = $form_state['values']['ticket_nid'];
  $_SESSION['stormexpense_list_filter']['datefrom'] = $form_state['values']['datefrom'];
  $_SESSION['stormexpense_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
  $_SESSION['stormexpense_list_filter']['user'] = $form_state['values']['user'];
  $_SESSION['stormexpense_list_filter']['title'] = $form_state['values']['title'];
  $_SESSION['stormexpense_list_filter']['provider_title'] = $form_state['values']['provider_title'];
}

function stormexpense_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormexpense_list_filter']);
}

function stormexpense_list_form_report() {
  $form = array();
  $form['#prefix'] = '<div id="stormexpense_list_form_report" class="storm_list_form_report">';
  $form['report'] = array(
    '#type' => 'select',
    '#title' => t('Reports'),
    '#options' => theme('stormexpense_list_form_report_reports'),
  );

  if (module_exists('locale')) {
    $language_list = language_list();
    $languages = array();
    foreach ($language_list as $key => $lang) {
      $languages[$key] = $lang->name;
    }
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $languages,
    );
  }
  else {
    $form['language'] = array(
      '#type' => 'hidden',
      '#default_value' => 'en',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Report'),
  );
  $form['#suffix'] = '</div>';
  return $form;
}

function stormexpense_list_form_report_submit($form, &$form_state) {
  $report = $form_state['values']['report'];
  $language = $form_state['values']['language'];
  drupal_goto('storm/expenses/report/'. $report .'/'. $language);
}

function stormexpense_list_report($report, $language) {
  $timetrackings = array();

  $s = "SELECT n.*,  sex.* FROM {node} AS n INNER JOIN {stormexpense} AS sex ON n.vid=sex.vid WHERE n.status=1 AND n.type='stormexpense' ";
  $s = stormexpense_access_sql($s, array());
  $s = db_rewrite_sql($s);

  $sql = isset($_SESSION['stormexpense_list_filter']['sql']) ? $_SESSION['stormexpense_list_filter']['sql'] : $s;
  $args = isset($_SESSION['stormexpense_list_filter']['args']) ? $_SESSION['stormexpense_list_filter']['args'] : array();
  $r = db_query($sql, $args);
  $expenses = array();
  while ($e = db_fetch_object($r)) {
    $expense = node_load($e->nid);
    $expenses[] = $expense;
  }

  print theme('stormexpense_list_report', $report, $language, $expenses);
}

function stormexpense_autocomplete($string = '') {
  // Autocomplete for providers textbox: shows any active organizations in Storm system which have been selected as providers.
  $matches = array();
  if ($string) {
    $s = "SELECT n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid WHERE n.status=1 AND n.type='stormorganization' AND sor.isprovider=1 AND sor.isactive=1";
    $where = array("LOWER(title) LIKE LOWER('%s%%')");
    $s = stormorganization_access_sql($s, $where);
    $s = db_rewrite_sql($s);
    $result = db_query_range($s, $string, 0, 10);
    while ($pro = db_fetch_object($result)) {
      $matches[$pro->title] = check_plain($pro->title);
    }
  }

  drupal_json($matches);
}
