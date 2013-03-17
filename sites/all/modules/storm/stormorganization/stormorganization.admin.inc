<?php

/**
 * @file
 */

function stormorganization_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('name', $_GET)) {
    if ($_SESSION['stormorganization_list_filter']['name'] != $_GET['name']) {
      $_SESSION['stormorganization_list_filter']['name'] = $_GET['name'];
    }
  }

  $i = new stdClass();
  $i->type = 'stormorganization';

  $header = array(
    array(
      'data' => t('Name'),
      'field' => 'n.title',
      'sort' => 'ASC',
    ),
    array(
      'data' => t('Country'),
      'field' => 'sor.country',
      'sort' => '',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s = "SELECT n.*, sor.*, nre.format FROM {node} AS n
    INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid
    INNER JOIN {node_revisions} AS nre ON n.vid = nre.vid
    WHERE n.status=1 AND n.type='stormorganization'";

  $where = array();
  $args = array();
  $filterfields = array();

  $country = isset($_SESSION['stormorganization_list_filter']['country']) ? $_SESSION['stormorganization_list_filter']['country'] : '-';
  if ($country != '-') {
    $where[] = "sor.country='%s'";
    $args[] = $_SESSION['stormorganization_list_filter']['country'];
    $filterfields[] = t('Country');
  }

  if (isset($_SESSION['stormorganization_list_filter']['name']) && $_SESSION['stormorganization_list_filter']['name'] != '') {
    $where[] = "LOWER(n.title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormorganization_list_filter']['name'];
    $filterfields[] = t('Name');
  }

  $iscustomer = isset($_SESSION['stormorganization_list_filter']['iscustomer']) ? $_SESSION['stormorganization_list_filter']['iscustomer'] : '-';
  if ($iscustomer != '-') {
    if ($iscustomer == 'yes') {
      $where[] = "sor.iscustomer=1";
    }
    elseif ($iscustomer == 'no') {
      $where[] = "sor.iscustomer=0";
    }
    $filterfields[] = t('Customer');
  }

  $isprovider = isset($_SESSION['stormorganization_list_filter']['isprovider']) ? $_SESSION['stormorganization_list_filter']['isprovider'] : '-';
  if ($isprovider != '-') {
    if ($isprovider == 'yes') {
      $where[] = "sor.isprovider=1";
    }
    elseif ($isprovider == 'no') {
      $where[] = "sor.isprovider=0";
    }
    $filterfields[] = t('Provider');
  }

  $isactive = isset($_SESSION['stormorganization_list_filter']['isactive']) ? $_SESSION['stormorganization_list_filter']['isactive'] : '-';
  if ($isactive != '-') {
    if ($isactive == 'yes') {
      $where[] = "sor.isactive=1";
    }
    elseif ($isactive == 'no') {
      $where[] = "sor.isactive=0";
    }
    $filterfields[] = t('Active');
  }

  // Sets value for fieldset label, does not affect filter itself.
  $itemsperpage = isset($_SESSION['stormorganization_list_filter']['itemsperpage']) ? $_SESSION['stormorganization_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);

  if (count($filterfields) == 0) {
    $filterdesc = t('Not filtered');
  }
  else {
    $filterdesc = t('Filtered by !fields', array('!fields' => implode(", ", array_unique($filterfields))));
  }
  $filterdesc .= ' | '. t('!items items per page', array('!items' => $itemsperpage));

  $o = drupal_get_form('stormorganization_list_filter', $filterdesc);

  $s = stormorganization_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[] = $organization;
  }

  $o .= theme('stormorganization_list', $header, $organizations);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormorganization_list_filter(&$form_state, $filterdesc = 'Filter') {
  $country_list = storm_attributes_bydomain('Country');
  $country = isset($_SESSION['stormorganization_list_filter']['country']) ? $_SESSION['stormorganization_list_filter']['country'] : '-';
  $_SESSION['stormorganization_list_filter']['country'] = $country;

  $name = isset($_SESSION['stormorganization_list_filter']['name']) ? $_SESSION['stormorganization_list_filter']['name'] : '';
  $iscustomer = isset($_SESSION['stormorganization_list_filter']['iscustomer']) ? $_SESSION['stormorganization_list_filter']['iscustomer'] : NULL;
  $isprovider = isset($_SESSION['stormorganization_list_filter']['isprovider']) ? $_SESSION['stormorganization_list_filter']['isprovider'] : NULL;

  $isactive = isset($_SESSION['stormorganization_list_filter']['isactive']) ? $_SESSION['stormorganization_list_filter']['isactive'] : 'yes';
  $_SESSION['stormorganization_list_filter']['isactive'] = $isactive;

  $itemsperpage = isset($_SESSION['stormorganization_list_filter']['itemsperpage']) ? $_SESSION['stormorganization_list_filter']['itemsperpage'] : variable_get('storm_default_items_per_page', 10);
  $_SESSION['stormorganization_list_filter']['itemsperpage'] = $itemsperpage;

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => $filterdesc,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -20,
  );

  $form['filter']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => $country,
    '#options' => array('-' => t('-')) + $country_list['values'],
  );

  $form['filter']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $name,
    '#autocomplete_path' => 'storm/organizations/autocomplete',
  );

  $qs = '<div class="storm_quick_shortcuts"><ul class="storm_quick_shortcuts">';
  foreach (range(0, 9) as $n) {
    $qs .= '<li>'. l($n, 'storm/organizations', array('query' => array('name' => $n .'%'))) .'</li>';
  }
  $qs .= '</ul></div>';
  $form['filter']['name_quick_shortcuts_numbers'] = array(
    '#type' => 'markup',
    '#value' => $qs,
  );
  $qs = '<div class="storm_quick_shortcuts"><ul class="storm_quick_shortcuts">';
  foreach (range('A', 'Z') as $l) {
    $qs .= '<li>'. l($l, 'storm/organizations', array('query' => array('name' => $l .'%'))) .'</li>';
  }
  $qs .= '</ul></div>';
  $form['filter']['name_quick_shortcuts_letters'] = array(
    '#type' => 'markup',
    '#value' => $qs,
  );

  $form['filter']['group0'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $options = array(
    '-' => t('-'),
    'no' => t('no'),
    'yes' => t('yes')
  );
  $form['filter']['group0']['iscustomer'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Is customer'),
    '#default_value' => $iscustomer,
  );

  $form['filter']['group0']['isprovider'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Is provider'),
    '#default_value' => $isprovider,
  );

  $form['filter']['group0']['isactive'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Is active'),
    '#default_value' => $isactive,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('stormorganization_list_filter_filter'),
  );

  $form['filter']['group1']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('stormorganization_list_filter_reset'),
  );

  $form['filter']['group1']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormorganization_list_filter_filter($form, &$form_state) {
  $_SESSION['stormorganization_list_filter']['country'] = $form_state['values']['country'];
  $_SESSION['stormorganization_list_filter']['name'] = $form_state['values']['name'];
  $_SESSION['stormorganization_list_filter']['iscustomer'] = $form_state['values']['iscustomer'];
  $_SESSION['stormorganization_list_filter']['isprovider'] = $form_state['values']['isprovider'];
  $_SESSION['stormorganization_list_filter']['isactive'] = $form_state['values']['isactive'];
  $_SESSION['stormorganization_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormorganization_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormorganization_list_filter']);
}

function stormorganization_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $s = "SELECT title FROM {node} AS n WHERE n.type='stormorganization' AND LOWER(title) LIKE LOWER('%s%%')";
    $s = stormorganization_access_sql($s);
    $s = db_rewrite_sql($s);

    $result = db_query_range($s, $string, 0, 10);
    while ($org = db_fetch_object($result)) {
      $matches[$org->title] = check_plain($org->title);
    }
  }

  drupal_json($matches);
}
