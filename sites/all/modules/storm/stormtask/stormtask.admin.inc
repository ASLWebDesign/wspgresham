<?php

/**
 * @file
 */

function stormtask_tasks($project) {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  $breadcrumb[] = l(t('Projects'), 'storm/projects');
  $breadcrumb[] = l(t('Project : !project', array('!project' => $project->title)), 'node/'. $project->nid);
  drupal_set_breadcrumb($breadcrumb);

  $i = new stdClass();
  $i->type = 'stormtask';

  $o = drupal_get_form('stormtask_tasks_filter');

  $params = $_GET;
  $params['organization_nid'] = $project->organization_nid;
  $params['project_nid'] = $project->nid;

  $header = array(
    array(
      'data' => t('Title'),
    ),
    array(
      'data' => t('Category'),
    ),
    array(
      'data' => t('Status'),
    ),
    array(
      'data' => t('Priority'),
    ),
    array(
      'data' => t('Duration'),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => t('Parent'),
    ),
    array(
      'data' => t('Weight'),
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  if (!stormproject_access('update', $project->nid)) {
    unset($header[5]);
    unset($header[6]);
  }

  $where = array();
  $args = array();

  $taskcategory = $_SESSION['stormtask_tasks_filter']['taskcategory'];
  if (!$taskcategory) {
    $category_list = storm_attributes_bydomain('Task category search');
    $taskcategory = $category_list['default'];
    $_SESSION['stormtask_tasks_filter']['taskcategory'] = $taskcategory;
  }
  if ($taskcategory && $taskcategory != '-') {
    $category = split(',', $taskcategory);
    $v = array();
    foreach ($category as $item) $v[] = '%s';
    $where[] = "sta.taskcategory IN ('". implode("','", $v) ."')";
    foreach ($category as $key => $value) {
      $args[] = $value;
    }
  }

  $taskstatus = $_SESSION['stormtask_tasks_filter']['taskstatus'];
  if (!$taskstatus) {
    $status_list = storm_attributes_bydomain('Task status search');
    $taskstatus = $status_list['default'];
    $_SESSION['stormtask_tasks_filter']['taskstatus'] = $taskstatus;
  }
  if ($taskstatus && $taskstatus != '-') {
    $status = split(',', $taskstatus);
    $v = array();
    foreach ($status as $item) $v[] = '%s';
    $where[] = "sta.taskstatus IN ('". implode("','", $v) ."')";
    foreach ($status as $key => $value) {
      $args[] = $value;
    }
  }

  $taskpriority = $_SESSION['stormtask_tasks_filter']['taskpriority'];
  if (!$taskpriority) {
    $priority_list = storm_attributes_bydomain('Task priority search');
    $taskpriority = $priority_list['default'];
    $_SESSION['stormtask_tasks_filter']['taskpriority'] = $taskpriority;
  }
  if ($taskpriority && $taskpriority != '-') {
    $priority = split(',', $taskpriority);
    $v = array();
    foreach ($priority as $item) $v[] = '%s';
    $where[] = "sta.taskpriority IN ('". implode("','", $v) ."')";
    foreach ($priority as $key => $value) {
      $args[] = $value;
    }
  }

  $taskstree = _stormtask_get_tree($project->nid, 0, -1, NULL, $where, $args);

  $o_gantt ="";
  $jsgantt_dir = drupal_get_path('module', 'stormtask') .'/jsgantt';

  $dispOpt=variable_get('stormtask_jsgantt_options', array());
  if ($dispOpt['ShowStartDate']) {
    $dispOpt['ShowStartDate']=1;
  }
  if ($dispOpt['ShowEndDate']) {
    $dispOpt['ShowEndDate']=1;
  }
  if ($dispOpt['ShowDur']) {
    $dispOpt['ShowDur']=1;
  }

  if (variable_get('stormtask_enable_ganttchart', 0)) {
    drupal_add_js($jsgantt_dir ."/jsgantt.js");
    drupal_add_css($jsgantt_dir ."/jsgantt.css");
    drupal_add_css(drupal_get_path('module', 'stormtask') . '/stormtask-gantt.css');

    // version w/o xml file
    $pMile = 0;
    $pRes = "";
    $pComp = 50;
    $pGroup = 0;
    $pParent = 0;
    $pOpen = 1;
    $js = "";
    $pDepend = 0;
    $pParent = 0;

    $now = time();
    $one_day = (24 * 60 * 60);
    $pColor_default = variable_get('stormtask_jsgantt_color', '00CCFF');
    $pColor_overdue = variable_get('stormtask_jsgantt_color_overdue', 'FF0D0D');

    foreach ($taskstree as $task ) {
      $pID = $task->nid;
      $pName = check_plain($task->title);
      $pCaption = $pName;
      $pLink = base_path() ."node/". $task->nid;
      $pStart = date("n/j/Y", $task->datebegin);
      $pEnd = date("n/j/Y", $task->dateend);
      $pColor = variable_get('stormtask_jsgantt_color', '00CCFF'); // $task->taskstatus;
      // $pDepend = $task->parents[0];
      // $pParent = $task->parents[0];

      // theme by task status
      $pColor = variable_get('stormtask_jsgantt_color_' . str_replace(' ', '_', $task->taskstatus), $pColor_default);
      $pComp = 0;
      switch ($task->taskstatus) {
        case 'completed':
          $pComp = 100;
          break;

        case 'on hold':
          $pComp = 50;
          break;

        case 'in progress':
          // percent complete as f($now), end date inclusive
          if ($now > $task->datebegin && $task->dateend > $task->datebegin) {
            $pComp = ($now - $task->datebegin) / ($task->dateend - $task->datebegin + $one_day);
            $pComp = round(100 * $pComp, 2);
          }
          // highlight overdue tasks
          if ($now > $task->dateend) {
            $pColor = $pColor_overdue;
          }
          break;
      }

      // show tasks with depth
      if ($task->depth) {
        $pName = str_repeat('--', $task->depth) .' '. $pName;
      }

      $js .= "g.AddTaskItem(new JSGantt.TaskItem(". $pID .",'". $pName ."','". $pStart ."','". $pEnd ."','". $pColor ."','". $pLink ."',". $pMile .",'". $pRes ."',". $pComp .",". $pGroup .",". $pParent .",". $pOpen .",". $pDepend .",'". $pCaption ."'));\n";
    }

    $o_gantt .= "<div style='position:relative' class='gantt' id='GanttChartDIV'></div>";

    drupal_add_js("
      // Here is all the html code neccessary to display the chart object
      // Future idea would be to allow XML file name to be passed in and chart tasks built from file.
      var g = new JSGantt.GanttChart('g',document.getElementById('GanttChartDIV'), 'day');
        g.setShowRes(0); // Show/Hide Responsible (0/1)
        g.setShowComp(0); // Show/Hide % Complete(0/1)
        g.setShowDur('". $dispOpt['ShowDur'] ."'); // Show/Hide Duration (0/1)
        g.setShowStartDate('". $dispOpt['ShowStartDate'] ."'); // Show/Hide Start Date(0/1)
        g.setShowEndDate('". $dispOpt['ShowEndDate'] ."'); // Show/Hide End Date(0/1)
        //g.setFormatArr('day','week','month')
        g.setCaptionType('Resource');  // Set to Show Caption (None,Caption,Resource,Duration,Complete)
      if( g ) {
// Parameters (pID, pName, pStart, pEnd, pColor, pLink, pMile, pRes,  pComp, pGroup, pParent, pOpen)
". $js ."

// use the XML file parser
//    JSGantt.parseXML('http://localhost/services/sites/default/files/project.xml',g)

    g.Draw();
    g.DrawDependencies();
      }
      else
      {
    alert('not defined');
      }", 'inline', 'footer'); // end drupal_add_js
  }

  $o .= drupal_get_form('stormtask_tasks_form', $header, $taskstree, $project);
  $o .= $o_gantt;
  return $o;
}

function stormtask_tasks_form($form_id, $header, $tasks, $project) {
  $form = array();

  $form['tasks'] = array(
    '#theme' => 'stormtask_tasks',
  );

  $form['tasks']['header'] = array(
    '#value' => $header,
  );

  $tree = _stormtask_get_tree($project->nid);
  $parent_tasks = _stormtask_plain_tree($tree);

  $params = $_GET;
  $params['project_nid'] = $project->nid;

  foreach ($tasks as $task) {
    $task->type = 'stormtask';

    $form['tasks']['tasks'][$task->nid]['task_depth_'. $task->nid] = array(
      '#value' => $task->depth,
    );

    $form['tasks']['tasks'][$task->nid]['task_description_'. $task->nid] = array(
      '#value' => l($task->stepno .' '. $task->title, 'node/'. $task->nid, array('html' => TRUE)),
    );

    $form['tasks']['tasks'][$task->nid]['task_category_'. $task->nid] = array(
      '#value' => storm_icon('category_'. $task->taskcategory, storm_attribute_value('Task category', $task->taskcategory)),
    );

    $form['tasks']['tasks'][$task->nid]['task_status_'. $task->nid] = array(
      '#value' => storm_icon('status_'. $task->taskstatus, storm_attribute_value('Task status', $task->taskstatus)),
    );

    $form['tasks']['tasks'][$task->nid]['task_priority_'. $task->nid] = array(
      '#value' => storm_icon('priority_'. $task->taskpriority, storm_attribute_value('Task priority', $task->taskpriority)),
    );

    $form['tasks']['tasks'][$task->nid]['task_duration_'. $task->nid] = array(
      '#value' => $task->duration,
    );

    $form['tasks']['tasks'][$task->nid]['task_durationunit_'. $task->nid] = array(
      '#type' => 'hidden',
      '#value' => $task->durationunit,
    );

    $form['tasks']['tasks'][$task->nid]['task_parent-nid_'. $task->nid] = array(
      '#type' => 'select',
      '#options' => array(0 => '-') + $parent_tasks,
      '#default_value' => $task->parent_nid,
    );

    $form['tasks']['tasks'][$task->nid]['task_nid_'. $task->nid] = array(
      '#type' => 'hidden',
      '#default_value' => $task->nid,
    );

    $form['tasks']['tasks'][$task->nid]['task_weight_'. $task->nid] = array(
      '#type' => 'weight',
      '#default_value' => $task->weight,
    );

    $params['task_nid'] = $task->nid;

    $v = storm_icon_edit_node($task, $params);
    $v .= '&nbsp;'. storm_icon_delete_node($task, $params);
    $v .= '&nbsp;'. storm_icon_add_node($task, $params);

    $form['tasks']['tasks'][$task->nid]['task_operations_'. $task->nid] = array(
      '#value' => $v,
    );
  }

  $form['tasks']['project'] = array(
    '#type' => 'hidden',
    '#value' => $project->nid,
  );

  if (stormproject_access('update', $project->nid)) {
    // This form is displayed for users which may not have edit permissions for the nodes displayed.
    // Furthermore, permissions can mean that the user could have edit permissions for some nodes displayed and not others.
    // Therefore, the submit button must only be displayed if the user has the permission 'edit all', which guarantees the user is allowed to edit all nodes displayed.
    $form['submit'] = array(
      '#type' => 'submit',
      '#submit' => array('stormtask_tasks_submit'),
      '#value' => t('Save'),
    );
  }

  return $form;
}

function stormtask_tasks_submit($form, &$form_state) {
  $tasks = array();
  foreach ($form_state['values'] as $key => $value) {
    $ar = explode('_', $key);
    if ($ar[0]=='task') {
      if ($ar[1]=='weight') $tasks[$ar[2]]['weight'] = $value;
      if ($ar[1]=='parent-nid') $tasks[$ar[2]]['parent-nid'] = $value;
    }
  }

  $s = "UPDATE {stormtask} SET weight=%d, parent_nid=%d WHERE nid=%d";
  foreach ($tasks as $nid => $values) {
    db_query($s, $values['weight'], $values['parent-nid'], $nid);
  }

  drupal_set_message(t('Tasks saved'));
}

function stormtask_tasks_filter() {
  $category_list = storm_attributes_bydomain('Task category search');
  $taskcategory = $_SESSION['stormtask_tasks_filter']['taskcategory'];

  $status_list = storm_attributes_bydomain('Task status search');
  $taskstatus = $_SESSION['stormtask_tasks_filter']['taskstatus'];

  $priority_list = storm_attributes_bydomain('Task priority search');
  $taskpriority = $_SESSION['stormtask_tasks_filter']['taskpriority'];

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['taskcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $taskcategory,
    '#options' => $category_list['values'],
  );

  $form['filter']['group1']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $taskstatus,
    '#options' => $status_list['values'],
  );

  $form['filter']['group1']['taskpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $taskpriority,
    '#options' => $priority_list['values'],
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormtask_tasks_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormtask_tasks_filter_reset'),
  );

  return $form;
}

function stormtask_tasks_filter_filter($form, &$form_state) {
  $_SESSION['stormtask_tasks_filter']['taskcategory'] = $form_state['values']['taskcategory'];
  $_SESSION['stormtask_tasks_filter']['taskstatus'] = $form_state['values']['taskstatus'];
  $_SESSION['stormtask_tasks_filter']['taskpriority'] = $form_state['values']['taskpriority'];
}

function stormtask_tasks_filter_reset($form, &$form_state) {
  unset($_SESSION['stormtask_tasks_filter']);
}

function _stormtask_project_tasks_js($project_nid) {
  $tree = _stormtask_get_tree($project_nid);
  $tasks = _stormtask_plain_tree($tree);
  print drupal_to_js($tasks);
  exit();
}

function _stormtask_project_assignments_js($organization_nid, $project_nid) {
  $assignments = storm_get_assignment_options($organization_nid, $project_nid);
  print drupal_to_js($assignments);
  exit();
}

function stormtask_list() {

  global $user;

  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormtask_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormtask_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormtask_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormtask_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormtask_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormtask_list_filter']['organization_nid'] = $p->organization_nid;

  }

  $i = new stdClass();
  $i->type = 'stormtask';

  $header = array(
    array(
      'data' => t('Organization'),
      'field' => 'sta.organization_title',
    ),
    array(
      'data' => t('Project'),
      'field' => 'sta.project_title',
    ),
    array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    array(
      'data' => t('Category'),
      'field' => 'sta.taskcategory',
    ),
    array(
      'data' => t('Status'),
      'field' => 'sta.taskstatus',
    ),
    array(
      'data' => t('Priority'),
      'field' => 'sta.taskpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();
  $filterfields = array();

  $s  = "SELECT n.*, sta.*, nre.format FROM {node} AS n
    INNER JOIN {stormtask} AS sta ON n.vid=sta.vid
    INNER JOIN {node_revisions} AS nre ON n.vid = nre.vid
    WHERE n.status=1 AND n.type='stormtask' ";
  if (isset($_SESSION['stormtask_list_filter']['organization_nid']) && ($_SESSION['stormtask_list_filter']['organization_nid'] != 0)) {
    $where[] = 'sta.organization_nid=%d';
    $args[] = $_SESSION['stormtask_list_filter']['organization_nid'];
    $filterfields[] = 'Organization';
  }
  if (isset($_SESSION['stormtask_list_filter']['project_nid']) && ($_SESSION['stormtask_list_filter']['project_nid'] != 0)) {
    $where[] = 'sta.project_nid=%d';
    $args[] = $_SESSION['stormtask_list_filter']['project_nid'];
    $filterfields[] = 'Project';
  }
  if (isset($_SESSION['stormtask_list_filter']['taskcategory']) && $_SESSION['stormtask_list_filter']['taskcategory'] != '-') {
    $category = split(',', $_SESSION['stormtask_list_filter']['taskcategory']);
    $v = array();
    foreach ($category as $item) $v[] = '%s';
    $where[] = "sta.taskcategory IN ('". implode("','", $v) ."')";
    foreach ($category as $key => $value) {
      $args[] = $value;
    }
    $filterfields[] = 'Category';
  }
  if (isset($_SESSION['stormtask_list_filter']['taskstatus']) && $_SESSION['stormtask_list_filter']['taskstatus'] != '-') {
    $status = split(',', $_SESSION['stormtask_list_filter']['taskstatus']);
    $v = array();
    foreach ($status as $item) $v[] = '%s';
    $where[] = "sta.taskstatus IN ('". implode("','", $v) ."')";
    foreach ($status as $key => $value) {
      $args[] = $value;
    }
    $filterfields[] = 'Status';
  }
  if (isset($_SESSION['stormtask_list_filter']['taskpriority']) && $_SESSION['stormtask_list_filter']['taskpriority'] != '-') {
    $priority = split(',', $_SESSION['stormtask_list_filter']['taskpriority']);
    $v = array();
    foreach ($priority as $item) $v[] = '%s';
    $where[] = "sta.taskpriority IN ('". implode("','", $v) ."')";
    foreach ($priority as $key => $value) {
      $args[] = $value;
    }
    $filterfields[] = 'Priority';
  }

  if (isset($_SESSION['stormtask_list_filter']['datebeginfrom'])) {
    $datebeginfrom = $_SESSION['stormtask_list_filter']['datebeginfrom'];
    $datebeginfrom['hour'] = 0;
    $datebeginfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($datebeginfrom);
    if ($datebeginfrom['year']>0 && $t>=0) {
      $where[] = 'sta.datebegin>=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormtask_list_filter']['datebeginto'])) {
    $datebeginto = $_SESSION['stormtask_list_filter']['datebeginto'];
    $datebeginto['hour'] = 23;
    $datebeginto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($datebeginto);
    if ($datebeginto['year']>0 && $t>=0) {
      $where[] = 'sta.datebegin<=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormtask_list_filter']['dateendfrom'])) {
    $dateendfrom = $_SESSION['stormtask_list_filter']['dateendfrom'];
    $dateendfrom['hour'] = 0;
    $dateendfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($dateendfrom);
    if ($dateendfrom['year']>0 && $t>=0) {
      $where[] = 'sta.dateend>=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormtask_list_filter']['dateendto'])) {
    $dateendto = $_SESSION['stormtask_list_filter']['dateendto'];
    $dateendto['hour'] = 23;
    $dateendto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($dateendto);
    if ($dateendto['year']>0 && $t>=0) {
      $where[] = 'sta.dateend<=%d';
      $args[] = $t;
      $filterfields[] = 'Date';
    }
  }

  if (isset($_SESSION['stormtask_list_filter']['assigned_to'])) {
    if (!is_numeric($_SESSION['stormtask_list_filter']['assigned_to'])) {
      switch ($_SESSION['stormtask_list_filter']['assigned_to']) {

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
          $where[] = '(sta.assigned_nid IS NULL OR sta.assigned_nid = 0) ';
          break;

      }
    }
    else {
      $assigned_to_nid = $_SESSION['stormtask_list_filter']['assigned_to'];
    }
    if (!empty($assigned_to_nid) && is_numeric($assigned_to_nid)) {
      $where[] = 'sta.assigned_nid=%d';
      $args[] = $assigned_to_nid;
      $filterfields[] = t('Assigned to');
    }
  }

  if (isset($_SESSION['stormtask_list_filter']['billable']) && $_SESSION['stormtask_list_filter']['billable'] != '-') {
    $where[] = 'sta.billable=%d';
    $args[] = $_SESSION['stormtask_list_filter']['billable'];
    $filterfields[] = t('Billable');
  }

  if (isset($_SESSION['stormtask_list_filter']['billed']) && $_SESSION['stormtask_list_filter']['billed'] != '-') {
    $where[] = 'sta.billed=%d';
    $args[] = $_SESSION['stormtask_list_filter']['billed'];
    $filterfields[] = t('Billed');
  }

  $itemsperpage = isset($_SESSION['stormtask_list_filter']['itemsperpage']) ? $_SESSION['stormtask_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $o = drupal_get_form('stormtask_list_filter', $filterdesc);

  $s = stormtask_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $tasks = array();
  while ($item = db_fetch_object($r)) {
    $tasks[] = $item;
  }

  $o .= theme('stormtask_list', $header, $tasks);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormtask_list_filter(&$form_state, $filterdesc = 'Filter') {
  $category_list = storm_attributes_bydomain('Task category search');
  $taskcategory = isset($_SESSION['stormtask_list_filter']['taskcategory']) ? $_SESSION['stormtask_list_filter']['taskcategory'] : $taskcategory = $category_list['default'];
  $_SESSION['stormtask_list_filter']['taskcategory'] = $taskcategory;

  $status_list = storm_attributes_bydomain('Task status search');
  $taskstatus = isset($_SESSION['stormtask_list_filter']['taskstatus']) ? $_SESSION['stormtask_list_filter']['taskstatus'] : $status_list['default'];
  $_SESSION['stormtask_list_filter']['taskstatus'] = $taskstatus;

  $priority_list = storm_attributes_bydomain('Task priority search');
  $taskpriority = isset($_SESSION['stormtask_list_filter']['taskpriority']) ? $_SESSION['stormtask_list_filter']['taskpriority'] : $priority_list['default'];
  $_SESSION['stormtask_list_filter']['taskpriority'] = $taskpriority;

  $organization_nid = isset($_SESSION['stormtask_list_filter']['organization_nid']) ? $_SESSION['stormtask_list_filter']['organization_nid'] : 0;
  $project_nid = isset($_SESSION['stormtask_list_filter']['project_nid']) ? $_SESSION['stormtask_list_filter']['project_nid'] : 0;

  $datebeginfrom = isset($_SESSION['stormtask_list_filter']['datebeginfrom']) ? $_SESSION['stormtask_list_filter']['datebeginfrom'] : NULL;
  $datebeginto = isset($_SESSION['stormtask_list_filter']['datebeginto']) ? $_SESSION['stormtask_list_filter']['datebeginto'] : NULL;
  $dateendfrom = isset($_SESSION['stormtask_list_filter']['dateendfrom']) ? $_SESSION['stormtask_list_filter']['dateendfrom'] : NULL;
  $dateendto = isset($_SESSION['stormtask_list_filter']['dateendto']) ? $_SESSION['stormtask_list_filter']['dateendfrom'] : NULL;

  $assigned_to = isset($_SESSION['stormtask_list_filter']['assigned_to']) ? $_SESSION['stormtask_list_filter']['assigned_to'] : NULL;

  $itemsperpage = isset($_SESSION['stormtask_list_filter']['itemsperpage']) ? $_SESSION['stormtask_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['stormtask_list_filter']['itemsperpage'] = $itemsperpage;

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
    '#weight' => -20,
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
    '#attributes' => array('onchange' => "stormproject_organization_projects(this, 'edit-project-nid', true, 'All')"),
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
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['taskcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $taskcategory,
    '#options' => $category_list['values'],
  );

  $form['filter']['group2']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $taskstatus,
    '#options' => $status_list['values'],
  );

  $form['filter']['group2']['taskpriority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#default_value' => $taskpriority,
    '#options' => $priority_list['values'],
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['datebeginfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date begin from'),
    '#default_value' => $datebeginfrom,
  );

  $form['filter']['group3']['datebeginto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date begin to'),
    '#default_value' => $datebeginto,
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group4']['dateendfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date end from'),
    '#default_value' => $dateendfrom,
  );

  $form['filter']['group4']['dateendto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => t('Date end to'),
    '#default_value' => $dateendto,
  );

  $form['filter']['group4_1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  // ASSIGNED TO
  $options = storm_get_assignment_options(0, 0);
  $form['filter']['group4_1']['assigned_to'] = array(
    '#type' => 'select',
    '#title' => t('Assigned to'),
    '#default_value' => $assigned_to,
    '#options' => $options,
  );

  $form['filter']['group4_1']['billable'] = array(
    '#type' => 'select',
    '#title' => 'Billable',
    '#options' => array('-' => t('all'), '1' => t('billable'), '0' => t('not billable')),
    '#default_value' => isset($_SESSION['stormtask_list_filter']['billable']) ? $_SESSION['stormtask_list_filter']['billable'] : '-',
  );

  $form['filter']['group4_1']['billed'] = array(
    '#type' => 'select',
    '#title' => 'Billed',
    '#options' => array('-' => t('all'), '1' => t('billed'), '0' => t('not billed')),
    '#default_value' => isset($_SESSION['stormtask_list_filter']['billed']) ? $_SESSION['stormtask_list_filter']['billed'] : '-',
  );

  $form['filter']['group5'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group5']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormtask_list_filter_filter'),
  );

  $form['filter']['group5']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormtask_list_filter_reset'),
  );

  $form['filter']['group5']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormtask_list_filter_filter($form, &$form_state) {
  $_SESSION['stormtask_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormtask_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormtask_list_filter']['taskcategory'] = $form_state['values']['taskcategory'];
  $_SESSION['stormtask_list_filter']['taskstatus'] = $form_state['values']['taskstatus'];
  $_SESSION['stormtask_list_filter']['taskpriority'] = $form_state['values']['taskpriority'];
  $_SESSION['stormtask_list_filter']['datebeginfrom'] = $form_state['values']['datebeginfrom'];
  $_SESSION['stormtask_list_filter']['datebeginto'] = $form_state['values']['datebeginto'];
  $_SESSION['stormtask_list_filter']['dateendfrom'] = $form_state['values']['dateendfrom'];
  $_SESSION['stormtask_list_filter']['dateendto'] = $form_state['values']['dateendto'];
  $_SESSION['stormtask_list_filter']['assigned_to'] = $form_state['values']['assigned_to'];
  $_SESSION['stormtask_list_filter']['billable'] = $form_state['values']['billable'];
  $_SESSION['stormtask_list_filter']['billed'] = $form_state['values']['billed'];
  $_SESSION['stormtask_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormtask_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormtask_list_filter']);
}

