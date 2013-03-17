<?php

/**
 * @file
 */

function stormproject_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormproject_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormproject_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
  }

  $i = new stdClass();
  $i->type = 'stormproject';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Organization'),
      'field' => 'spr.organization_title',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Status'),
      'field' => 'spr.projectstatus',
    ),
    array(
      'data' => t('Priority'),
      'field' => 'spr.projectpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.*, spr.*, nre.format FROM {node} AS n
    INNER JOIN {stormproject} AS spr ON n.vid=spr.vid
    INNER JOIN {node_revisions} AS nre ON n.vid = nre.vid
    WHERE n.status=1 AND n.type='stormproject'";

  $where = array();
  $args = array();
  $filterfields = array();

  if (isset($_SESSION['stormproject_list_filter']['organization_nid']) && ($_SESSION['stormproject_list_filter']['organization_nid'] != 0)) {
    $where[] = 'spr.organization_nid=%d';
    $args[] = $_SESSION['stormproject_list_filter']['organization_nid'];
    $filterfields[] = 'Organization';
  }

  $category_list = storm_attributes_bydomain('project category search');
  $projectcategory = isset($_SESSION['stormproject_list_filter']['projectcategory']) ? $_SESSION['stormproject_list_filter']['projectcategory'] : $category_list['default'];
  $_SESSION['stormproject_list_filter']['projectcategory'] = $projectcategory;

  if ($projectcategory != '-') {
    $where[] = "spr.projectcategory='%s'";
    $args[] = $projectcategory;
    $filterfields[] = 'Category';
  }

  $status_list = storm_attributes_bydomain('project status search');
  $projectstatus = isset($_SESSION['stormproject_list_filter']['projectstatus']) ? $_SESSION['stormproject_list_filter']['projectstatus'] : $status_list['default'];
  $_SESSION['stormproject_list_filter']['projectstatus'] = $projectstatus;

  if ($projectstatus != '-') {
    $status = split(',', $projectstatus);
    $v = array();
    foreach ($status as $item) $v[] = '%s';
    $where[] = "spr.projectstatus IN ('". implode("','", $v) ."')";
    foreach ($status as $key => $value) {
      $args[] = $value;
    }
    $filterfields[] = 'Status';
  }

  $priority_list = storm_attributes_bydomain('project priority search');
  $projectpriority = isset($_SESSION['stormproject_list_filter']['projectpriority']) ? $_SESSION['stormproject_list_filter']['projectpriority'] : $priority_list['default'];
  $_SESSION['stormproject_list_filter']['projectpriority'] = $projectpriority;

  if ($projectpriority != '-') {
    $where[] = "spr.projectpriority='%s'";
    $args[] = $projectpriority;
    $filterfields[] = 'Priority';
  }

  if (isset($_SESSION['stormproject_list_filter']['datebeginfrom'])) {
    $datebeginfrom = $_SESSION['stormproject_list_filter']['datebeginfrom'];
    $datebeginfrom['hour'] = 0;
    $datebeginfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($datebeginfrom);
    if ($datebeginfrom['year']>0 && $t>=0) {
      $where[] = 'spr.datebegin>=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormproject_list_filter']['datebeginto'])) {
    $datebeginto = $_SESSION['stormproject_list_filter']['datebeginto'];
    $datebeginto['hour'] = 23;
    $datebeginto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($datebeginto);
    if ($datebeginto['year']>0 && $t>=0) {
      $where[] = 'spr.datebegin<=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormproject_list_filter']['dateendfrom'])) {
    $dateendfrom = $_SESSION['stormproject_list_filter']['dateendfrom'];
    $dateendfrom['hour'] = 0;
    $dateendfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($dateendfrom);
    if ($dateendfrom['year']>0 && $t>=0) {
      $where[] = 'spr.dateend>=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormproject_list_filter']['dateendto'])) {
    $dateendto = $_SESSION['stormproject_list_filter']['dateendto'];
    $dateendto['hour'] = 23;
    $dateendto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($dateendto);
    if ($dateendto['year']>0 && $t>=0) {
      $where[] = 'spr.dateend<=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormproject_list_filter']['assigned_to'])) {
    if (!is_numeric($_SESSION['stormproject_list_filter']['assigned_to'])) {
      switch ($_SESSION['stormproject_list_filter']['assigned_to']) {
        case 'all':
          // NO FILTER
          break;
        case 'mine':
          // CURRENT USER
          if (!empty($user->stormperson_nid) && is_numeric($user->stormperson_nid)) {
            $assigned_to_nid = $user->stormperson_nid;
          }
          break;
        case 'none':
          $where[] = '(spr.assigned_nid IS NULL OR spr.assigned_nid = 0) ';
          break;
      }
    }
    else {
      $assigned_to_nid = $_SESSION['stormproject_list_filter']['assigned_to'];
    }
    if (!empty($assigned_to_nid) && is_numeric($assigned_to_nid)) {
      $where[] = 'spr.assigned_nid=%d';
      $args[] = $assigned_to_nid;
      $filterfields[] = t('Assigned to');
    }
  }

  if (isset($_SESSION['stormproject_list_filter']['billable']) && $_SESSION['stormproject_list_filter']['billable'] != '-') {
    $where[] = 'spr.billable=%d';
    $args[] = $_SESSION['stormproject_list_filter']['billable'];
    $filterfields[] = t('Billable');
  }

  if (isset($_SESSION['stormproject_list_filter']['billed']) && $_SESSION['stormproject_list_filter']['billed'] != '-') {
    $where[] = 'spr.billed=%d';
    $args[] = $_SESSION['stormproject_list_filter']['billed'];
    $filterfields[] = t('Billed');
  }

  $itemsperpage = isset($_SESSION['stormproject_list_filter']['itemsperpage']) ? $_SESSION['stormproject_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $o = drupal_get_form('stormproject_list_filter', $filterdesc);

  $tablesort = tablesort_sql($header);

  $s = stormproject_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[] = $project;
  }

  $o .= theme('stormproject_list', $header, $projects);
  $o .= theme('pager', NULL, 10, 0);
  print theme('page', $o);
}

function stormproject_list_filter(&$form_state, $filterdesc = 'Filter') {
  $organization_nid = isset($_SESSION['stormproject_list_filter']['organization_nid']) ? $_SESSION['stormproject_list_filter']['organization_nid'] : 0;

  $category_list = storm_attributes_bydomain('project category search');
  $projectcategory = isset($_SESSION['stormproject_list_filter']['projectcategory']) ? $_SESSION['stormproject_list_filter']['projectcategory'] : $category_list['default'];

  $status_list = storm_attributes_bydomain('project status search');
  $projectstatus = isset($_SESSION['stormproject_list_filter']['projectstatus']) ? $_SESSION['stormproject_list_filter']['projectstatus'] : $status_list['default'];

  $priority_list = storm_attributes_bydomain('project priority search');
  $projectpriority = isset($_SESSION['stormproject_list_filter']['projectpriority']) ? $_SESSION['stormproject_list_filter']['projectpriority'] : $priority_list['default'];

  $datebeginfrom = isset($_SESSION['stormproject_list_filter']['datebeginfrom']) ? $_SESSION['stormproject_list_filter']['datebeginfrom'] : NULL;
  $datebeginto = isset($_SESSION['stormproject_list_filter']['datebeginto']) ? $_SESSION['stormproject_list_filter']['datebeginto'] : NULL;
  $dateendfrom = isset($_SESSION['stormproject_list_filter']['dateendfrom']) ? $_SESSION['stormproject_list_filter']['dateendfrom'] : NULL;
  $dateendto = isset($_SESSION['stormproject_list_filter']['dateendto']) ? $_SESSION['stormproject_list_filter']['dateendto'] : NULL;

  $assigned_to = isset($_SESSION['stormproject_list_filter']['assigned_to']) ? $_SESSION['stormproject_list_filter']['assigned_to'] : NULL;

  $itemsperpage = isset($_SESSION['stormproject_list_filter']['itemsperpage']) ? $_SESSION['stormproject_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['stormproject_list_filter']['itemsperpage'] = $itemsperpage;

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => $filterdesc,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title ";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);

  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $form['filter']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => t('Organization'),
    '#default_value' => $organization_nid,
    '#options' => array(0 => t('All')) + $organizations,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['projectcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $projectcategory,
    '#options' => $category_list['values'],
  );

  $form['filter']['group1']['projectstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $projectstatus,
    '#options' => $status_list['values'],
  );

  $form['filter']['group1']['projectpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $projectpriority,
    '#options' => $priority_list['values'],
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['datebeginfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date begin from'),
    '#default_value' => $datebeginfrom,
  );

  $form['filter']['group2']['datebeginto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date begin to'),
    '#default_value' => $datebeginto,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['dateendfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date end from'),
    '#default_value' => $dateendfrom,
  );

  $form['filter']['group3']['dateendto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date end to'),
    '#default_value' => $dateendto,
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  // ASSIGNED TO
  $options = storm_get_assignment_options(0, 0);
  $form['filter']['group4']['assigned_to'] = array(
    '#type' => 'select',
    '#title' => t('Assigned to'),
    '#default_value' => $assigned_to,
    '#options' => $options,
  );

  $form['filter']['group4']['billable'] = array(
    '#type' => 'select',
    '#title' => 'Billable',
    '#options' => array('-' => t('all'), '1' => t('billable'), '0' => t('not billable')),
    '#default_value' => isset($_SESSION['stormproject_list_filter']['billable']) ? $_SESSION['stormproject_list_filter']['billable'] : '-',
  );

  $form['filter']['group4']['billed'] = array(
    '#type' => 'select',
    '#title' => 'Billed',
    '#options' => array('-' => t('all'), '1' => t('billed'), '0' => t('not billed')),
    '#default_value' => isset($_SESSION['stormproject_list_filter']['billed']) ? $_SESSION['stormproject_list_filter']['billed'] : '-',
  );

  $form['filter']['group5'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group5']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['filter']['group5']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormproject_list_filter_filter'),
  );

  $form['filter']['group5']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormproject_list_filter_reset'),
  );

  return $form;
}

function stormproject_list_filter_filter($form, &$form_state) {
  $_SESSION['stormproject_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormproject_list_filter']['projectcategory'] = $form_state['values']['projectcategory'];
  $_SESSION['stormproject_list_filter']['projectstatus'] = $form_state['values']['projectstatus'];
  $_SESSION['stormproject_list_filter']['projectpriority'] = $form_state['values']['projectpriority'];
  $_SESSION['stormproject_list_filter']['datebeginfrom'] = $form_state['values']['datebeginfrom'];
  $_SESSION['stormproject_list_filter']['datebeginto'] = $form_state['values']['datebeginto'];
  $_SESSION['stormproject_list_filter']['dateendfrom'] = $form_state['values']['dateendfrom'];
  $_SESSION['stormproject_list_filter']['dateendto'] = $form_state['values']['dateendto'];
  $_SESSION['stormproject_list_filter']['billable'] = $form_state['values']['billable'];
  $_SESSION['stormproject_list_filter']['billed'] = $form_state['values']['billed'];
  $_SESSION['stormproject_list_filter']['assigned_to'] = $form_state['values']['assigned_to'];
  $_SESSION['stormproject_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormproject_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormproject_list_filter']);
}

function _stormproject_organization_projects_js($organization_nid=0) {
  $projects = array();

  if ($organization_nid) {
    $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormproject} AS spr ON n.vid=spr.vid WHERE n.status=1 AND n.type='stormproject' AND spr.organization_nid=%d ORDER BY n.title";
    $s = stormproject_access_sql($s);
    $s = db_rewrite_sql($s);
    $r = db_query($s, $organization_nid);

    while ($item = db_fetch_object($r)) {
      $nid = $item->nid;
      $projects[$nid] = check_plain($item->title);
    }
  }
  print drupal_to_js($projects);
  exit();
}

