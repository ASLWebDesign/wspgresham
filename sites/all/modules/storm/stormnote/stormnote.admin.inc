<?php

/**
 * @file
 */

function stormnote_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormnote_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormnote_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormnote_list_filter']['project_nid']);
    unset($_SESSION['stormnote_list_filter']['task_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormnote_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormnote_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormnote_list_filter']['organization_nid'] = $p->organization_nid;

    unset($_SESSION['stormnote_list_filter']['task_nid']);
  }

  if (array_key_exists('task_nid', $_GET)) {
    if ($_SESSION['stormnote_list_filter']['task_nid'] != $_GET['task_nid']) {
      $_SESSION['stormnote_list_filter']['task_nid'] = $_GET['task_nid'];
    }

    $t = node_load($_GET['task_nid']);
    $_SESSION['stormnote_list_filter']['organization_nid'] = $t->organization_nid;
    $_SESSION['stormnote_list_filter']['project_nid'] = $t->project_nid;
  }

  $i = new stdClass();
  $i->type = 'stormnote';

  $params = $_GET;
  if (isset($_SESSION['stormnote_list_filter']['organization_nid'])) {
    $params['organization_nid'] = $_SESSION['stormnote_list_filter']['organization_nid'];
  }

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'sno.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sno.project_title',
    ),
    array(
      'data' => t('Task'),
      'field' => 'sno.task_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.title, n.type, n.uid, sno.*, nre.format FROM {node} AS n
    INNER JOIN {stormnote} AS sno ON n.vid=sno.vid
    INNER JOIN {node_revisions} AS nre ON n.vid = nre.vid
    WHERE n.status=1 AND n.type='stormnote' ";

  $where = array();
  $args = array();
  $filterfields = array();

  if (isset($_SESSION['stormnote_list_filter']['organization_nid']) && $_SESSION['stormnote_list_filter']['organization_nid'] != 0) {
    $where[] = 'sno.organization_nid=%d';
    $args[] = $_SESSION['stormnote_list_filter']['organization_nid'];
    $filterfields[] = 'Organization';
  }
  if (isset($_SESSION['stormnote_list_filter']['project_nid']) && $_SESSION['stormnote_list_filter']['project_nid'] != 0) {
    $where[] = 'sno.project_nid=%d';
    $args[] = $_SESSION['stormnote_list_filter']['project_nid'];
    $filterfields[] = 'Project';
  }
  if (isset($_SESSION['stormnote_list_filter']['task_nid']) && $_SESSION['stormnote_list_filter']['task_nid'] != 0) {
    $where[] = 'sno.task_nid=%d';
    $args[] = $_SESSION['stormnote_list_filter']['task_nid'];
    $filterfields[] = 'Task';
  }
  if (isset($_SESSION['stormnote_list_filter']['title']) && $_SESSION['stormnote_list_filter']['title'] != '') {
    $where[] = "LOWER(n.title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormnote_list_filter']['title'];
    $filterfields[] = 'Title';
  }

  // This section only provides the value for the fieldset label, doesn't control actual filter
  $itemsperpage = isset($_SESSION['stormnote_list_filter']['itemsperpage']) ? $_SESSION['stormnote_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $o = drupal_get_form('stormnote_list_filter', $filterdesc);

  $s = stormnote_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $tablesort = tablesort_sql($header);

  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $notes = array();
  while ($item = db_fetch_object($r)) {
    $notes[] = $item;
  }

  $o .= theme('stormnote_list', $header, $notes);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormnote_list_filter(&$form_state, $filterdesc = 'Filter') {
  $organization_nid = isset($_SESSION['stormnote_list_filter']['organization_nid']) ? $_SESSION['stormnote_list_filter']['organization_nid'] : 0;
  $project_nid = isset($_SESSION['stormnote_list_filter']['project_nid']) ? $_SESSION['stormnote_list_filter']['project_nid'] : 0;
  $task_nid = isset($_SESSION['stormnote_list_filter']['task_nid']) ? $_SESSION['stormnote_list_filter']['task_nid'] : 0;
  $title = isset($_SESSION['stormnote_list_filter']['title']) ? $_SESSION['stormnote_list_filter']['title'] : '';

  $itemsperpage = isset($_SESSION['stormnote_list_filter']['itemsperpage']) ? $_SESSION['stormnote_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['stormnote_list_filter']['itemsperpage'] = $itemsperpage;

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => $filterdesc,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
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
    '#attributes' => array('onchange' => "stormtask_organization_project_tasks(this, 'edit-project-nid', 'edit-task-nid', '', true, 'All')"),
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
    '#attributes' => array('onchange' => "stormtask_project_tasks('edit-organization-nid', this, 'edit-task-nid', '', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormtask} AS sta ON sta.nid=n.nid WHERE n.status=1 AND n.type='stormtask' AND sta.project_nid=%d ORDER BY title ";
  $s = stormtask_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $project_nid);
  $tasks = array();
  while ($task = db_fetch_object($r)) {
    $tasks[$task->nid] = $task->title;
  }
  $tasks = array(0 => t('All')) + $tasks;
  $form['filter']['group1']['task_nid'] = array(
    '#type' => 'select',
    '#title' => t('Task'),
    '#default_value' => $task_nid,
    '#options' => $tasks,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $title,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormnote_list_filter_filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormnote_list_filter_reset'),
  );

  $form['filter']['group3']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;

}

function stormnote_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormnote_list_filter']);
}

function stormnote_list_filter_filter($form, &$form_state) {
  $_SESSION['stormnote_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormnote_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormnote_list_filter']['task_nid'] = $form_state['values']['task_nid'];
  $_SESSION['stormnote_list_filter']['title'] = $form_state['values']['title'];
  $_SESSION['stormnote_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}


