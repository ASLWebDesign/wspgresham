<?php

/**
 * @file
 * Provides theme functions for Storm modules
 */

function theme_storm_form_group($form) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');
  $output = NULL; // Variable set as per issue 684016.

  $row = array();
  foreach (element_children($form) as $key) {
    array_push($row, drupal_render($form[$key]));
  }
  $rows[] = array('data' => $row, 'class' => 'formgroup');
  $output .= theme('table', array(), $rows, array('class' => 'formgroup'));
  return $output;
}

function theme_datetime($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

function theme_dateext($element) {
  return theme('form_element', $element, '<div class="container-inline">'. $element['#children'] .'</div>');
}

function theme_storm_list_report($header, $rows, $title, $footer) {
  $css_all_file = drupal_get_path('module', 'storm') .'/storm_list_report_all.css';
  $query_string = '?'. drupal_substr(variable_get('css_js_query_string', '0'), 0, 1);

  $o = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $o .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $o .= '<head>';
  $o .= '<title>'. (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal')) .'</title>';
  $o .= '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $css_all_file . $query_string .'" />';
  $o .= '</head>';
  $o .= '<body>';
  $o .= '<table class="storm_report_header"><tr>';
  $o .= '<td class="storm_report_myorganization">';
  $o .= '<div class="site_name">'. variable_get('site_name', '') .'</div>';
  $o .= '<div class="site_slogan">'. variable_get('site_slogan', '') .'</div>';
  $o .= '</td>';  $o .= '<td class="storm_report_header">'. variable_get('storm_report_header', '') .'</td>';
  $o .= '</tr></table>';
  $o .= '<div class="storm_list_report_title">'. $title .'</div>';
  $o .= '<div class="storm_list_report_data">';
  $o .= theme('table', $header, $rows);
  $o .= '</div>';
  $o .= '<div class="storm_list_report_footer">'. $footer .'</div>';
  $o .= '</body>';
  $o .= '</html>';

  return $o;
}

function theme_storm_report($header, $content, $title, $footer, $headtitle='') {
  $css_all_file = drupal_get_path('module', 'storm') .'/storm_report_all.css';
  $query_string = '?'. drupal_substr(variable_get('css_js_query_string', '0'), 0, 1);

  $o = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $o .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $o .= '<head>';
  $o .= '<title>'. ($headtitle ? strip_tags($headtitle) : $title) .'</title>';
  $o .= '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $css_all_file . $query_string .'" />';
  $o .= '</head>';
  $o .= '<body>';
  $o .= '<table class="storm_report_header"><tr>';
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $o .= '<td class="storm_report_myorganization">';
  $o .= '<div class="site_name">'. variable_get('site_name', '') .'</div>';
  $o .= '<div class="site_slogan">'. variable_get('site_slogan', '') .'</div>';
  $o .= '</td>';
  $o .= '<td class="storm_report_header">'. variable_get('storm_report_header', '') .'</td>';
  $o .= '</tr></table>';
  $o .= '<div class="storm_report_title">'. $title .'</div>';
  $o .= '<div id="storm_report_content">';
  $o .= $content;
  $o .= '</div>';
  $o .= '<div class="storm_report_footer">'. $footer .'</div>';
  $o .= '</body>';
  $o .= '</html>';

  return $o;
}

function theme_storm_view_item($label, $value) {
  $o = '<div class="label"><span class="label">' . $label . ':&nbsp;</span></div><div class="value">' . $value . '&nbsp;</div>';
  return $o;
}

function theme_storm_dashboard($link_blocks) {
  $content = '<div id="stormdashboard">';
  if (!empty($link_blocks)) {
    $content .= '<dl class="stormdashboard clear-block">';
    foreach ($link_blocks as $block_id => $link_block_array) {
      $content .= '<div class="stormdashboard">';
      if (!empty($link_block_array)) {
        foreach ($link_block_array as $key => $link_array) {
          if (!empty($link_array['theme'])) {
            $content .= theme($link_array['theme'], $link_array);
          }
          else {
            $content .= theme('storm_dashboard_link', $link_array);
          }
        }
      }
      $content .= '</div>';
    }
    $content .= '</dl>';
  }
  else {
    $content .= t('No dashboard links available');
  }
  $content .= '</div>';

  return $content;
}

function theme_storm_dashboard_block($link_blocks) {
  $content = '<div id="stormdashboard-block">';
  if (!empty($link_blocks)) {
    foreach ($link_blocks as $block_id => $link_block_array) {
      if (!empty($link_block_array)) {
        foreach ($link_block_array as $key => $link_array) {
          $content .= '<div class="stormdashboard-icon">';
          if (!empty($link_array['theme'])) {
            $content .= theme($link_array['theme'], $link_array);
          }
          else {
            $content .= theme('storm_dashboard_link', $link_array);
          }
          $content .= '</div>';
        }
      }
    }
  }
  else {
    // no links, hide block
    return '';
  }
  $content .= '</div>';
  return $content;
}

function theme_storm_dashboard_link($link_array) {

  $content = '';

  // DEFAULT ICON
  if (empty($link_array['icon'])) {
    $dt_id = 'stormexpenses';
  }
  else {
    $dt_id = $link_array['icon'];
  }

  $params = array();
  if (!empty($link_array['nid'])) {
    $params_key = $link_array['node_type'] .'_nid';
    $params['query'] = array($params_key => $link_array['nid']);
  }

  $link = l($link_array['title'], $link_array['path'], $params);

  // ADD PLUS SIGN (node/add)
  if (!empty($link_array['add_type'])) {
    $item = new stdClass();
    $item->type = $link_array['add_type'];
    if (empty($link_array['params'])) {
      $link_array['params'] = array();
    }
    $link .= storm_icon_add('node/add/'. str_replace('_', '-', $link_array['add_type']), $item, $link_array['params']);
  }


  if (empty($link_array['nid']) || 0 == $link_array['nid']) {
    if (variable_get('storm_icons_display', TRUE)) {
      $content .= '<dt id="'. $dt_id .'" class="stormcomponent">';
    }
    else {
      $content .= '<dt class="stormcomponent">';
    }
    $content .= $link;
    $content .= '</dt>';
  }
  else {
    $content = array(
      '#prefix' => variable_get('storm_icons_display', TRUE) ? '<dt id="'. $dt_id .'" class="stormcomponent">' : '<dt class="stormcomponent">',
      '#suffix' => '</dt>',
      '#value' => $link,
      '#weight' => $link_array['weight'],
    );
  }


  return $content;
}

function theme_storm_dashboard_links_weight_table($form = array()) {
  $type = $form['#infix'];
  $rows = array();
  foreach ($form as $id => &$value) {
    if ($id[0] == '#') {
      continue;
    }
    $value[$type .'_storm_dashboard_link_weight_'. $id]['#attributes']['class'] = $type .'dashboard-link-table-weight';

    $row = array();
    $row[] = $value['#value'];
    $row[] = drupal_render($value[$type .'_storm_dashboard_link_active_'. $id]);
    $row[] = drupal_render($value[$type .'_storm_dashboard_link_weight_'. $id]);
    unset($value['#value']);
    if (!empty($row)) {
      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }
  $headers = array(t('Link'), t('Active'), t('Weight'));

  $output = theme('table', $headers, $rows, array('id' => $type .'dashboard-link-table'));

  drupal_add_tabledrag($type .'dashboard-link-table', 'order', 'sibling', $type .'dashboard-link-table-weight');

  $output .= drupal_render($form);

  return $output;
}

function theme_storm_link($source_module='', $destination_module='', $node_nid=0, $weight=0) {
  switch ($source_module) {

    case "stormorganization":
      $params_key = 'organization_nid';
      break;

    case "stormproject":
      $params_key = 'project_nid';
      break;

    case "stormtask":
      $params_key = 'task_nid';
      break;

    case "stormticket":
      $params_key = 'ticket_nid';
      break;
  }



  switch ($destination_module) {
    case "stormattribute":
      $user_access_check = 'Storm: access administration pages';
      $list_text = t('Attributes');
      $list_path = 'storm/attributes';
      $add_icon_type = 'stormattribute';
      $add_path = 'storm/attributes/add';
      $dt_id = 'stormattributes';
      break;

    case "stormconfiguration":
      $user_access_check = 'Storm: access administration pages';
      $list_text = t('Configuration');
      $list_path = 'admin/settings/storm';
      $add_icon_type = '';
      $add_path = '';
      $dt_id = 'stormconfiguration';
      break;

    case "stormexpense":
      $user_access_check = 'Storm expense: access';
      $list_text = t('Expenses');
      $list_path = 'storm/expenses';
      $add_icon_type = 'stormexpense';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormexpenses';
      break;

    case "storminvoice":
      $user_access_check = 'Storm invoice: access';
      $list_text = t('Invoices');
      $list_path = 'storm/invoices';
      $add_icon_type = 'storminvoice';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'storminvoices';
      break;

    case "stormknowledgebase":
      $user_access_check = 'Storm knowledgebase: access';
      $list_text = t('Knowledge base');
      $list_path = 'storm/knowledgebase';
      $add_icon_type = 'stormknowledgebase';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormknowledgebase';
      break;

    case "stormnote":
      $user_access_check = 'Storm note: access';
      $list_text = t('Notes');
      $list_path = 'storm/notes/';
      $add_icon_type = 'stormnote';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormnotes';
      break;

    case "stormorganization":
      $user_access_check = 'Storm organization: access';
      $list_text = t('Organizations');
      $list_path = 'storm/organizations';
      $add_icon_type = 'stormorganization';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormorganizations';
      break;

    case "stormperson":
      $user_access_check = 'Storm person: access';
      $list_text = t('People');
      $list_path = 'storm/people/';
      $add_icon_type = 'stormperson';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormpeople';
      break;

    case "stormproject":
      $user_access_check = 'Storm project: access';
      $list_text = t('Projects');
      $list_path = 'storm/projects';
      $add_icon_type = 'stormproject';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormprojects';
      break;

    case "stormtask":
      $user_access_check = 'Storm task: access';
      $list_text = t('Tasks');
      $list_path = 'storm/tasks/';
      $add_icon_type = 'stormtask';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormtasks';
      break;

    case "stormteam":
      $user_access_check = 'Storm team: access';
      $list_text = t('Teams');
      $list_path = 'storm/teams';
      $add_icon_type = 'stormteam';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormteams';
      break;

    case "stormticket":
      $user_access_check = 'Storm ticket: access';
      $list_text = t('Tickets');
      $list_path = 'storm/tickets/';
      $add_icon_type = 'stormticket';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormtickets';
      break;

    case "stormtimetracking":
      $user_access_check = 'Storm timetracking: access';
      $list_text = t('Timetrackings');
      $list_path = 'storm/timetrackings';
      $add_icon_type = 'stormtimetracking';
      $node_links_key = 'timetrackings';
      $add_path = 'node/add/'. $destination_module;
      $dt_id = 'stormtimetrackings';
      break;
  }

  if (($destination_module=='stormconfiguration' && user_access($user_access_check)) || (module_exists($destination_module) && user_access($user_access_check))) {
    if ($node_nid == 0) {
      $params = array();
    }
    else {
      $params = array('query' => array($params_key => $node_nid));
    }
    $v = l($list_text, $list_path, $params);

    if (!$add_icon_type=='') {
      $i = new stdClass();
      $i->type = $add_icon_type;
      if ($node_nid == 0) {
        $params = $_GET;
      }
      else {
        $params = $_GET;
        $params[$params_key] = $node_nid;
      }
      $nmb_itms = _storm_number_of_items($source_module, $destination_module, $node_nid);
      $nmb_itms = theme('storm_number_items', $nmb_itms);
      $v .= $nmb_itms . storm_icon_add($add_path, $i, $params);
    }

    if ($node_nid == 0) {
      $r = variable_get('storm_icons_display', TRUE) ? '<dt id="'. $dt_id .'" class="stormcomponent">' : '<dt class="stormcomponent">';
      $r .= $v;
      $r .= '</dt>';
    }
    else {
      $r = array(
        '#prefix' => variable_get('storm_icons_display', TRUE) ? '<dt id="'. $dt_id .'" class="stormcomponent">' : '<dt class="stormcomponent">',
        '#suffix' => '</dt>',
        '#value' => $v,
        '#weight' => $weight,
      );
    }
    return $r;
  }
}

function theme_storm_number_items($number) {
  $content = '';
  if ($number !== '') {
    $content .= '<span class="stormlinks-count">('. $number .')</span>';
  }
  return $content;
}

function theme_storm_attribute_list($form) {
  drupal_add_tabledrag('attributes', 'order', 'sibling', 'attribute-weight');

  $header = array();
  $row = array();
  $rows = array();

  $header = $form['header']['#value'];
  unset($form['header']);

  if (isset($form['attributes'])) {
    foreach (element_children($form['attributes']) as $key) {
      $form['attributes'][$key]['attribute_weight_'. $key]['#attributes']['class'] = 'attribute-weight';
      $domain = check_markup(drupal_render($form['attributes'][$key]['attribute_domain_'. $key]));
      $isdefault = $form['attributes'][$key]['attribute_default_'. $key]['#default_value'];
      if ($isdefault) {
        $ischecked = 'checked="checked"';
      }
      else {
        $ischecked = '';
      }
      $domain_name = 'attribute_default_'. str_replace(' ', '|', $domain);
      $default_ctl = '<input type="radio" name="'. $domain_name .'" value="'. $key .'" '. $ischecked .'"/>';
      $data = array(
        $domain,
        check_markup(drupal_render($form['attributes'][$key]['attribute_akey_'. $key])),
        check_markup(drupal_render($form['attributes'][$key]['attribute_avalue_'. $key])),
        drupal_render($form['attributes'][$key]['attribute_isactive_'. $key]),
        $default_ctl,
        drupal_render($form['attributes'][$key]['attribute_weight_'. $key]),
        array(
          'data' => drupal_render($form['attributes'][$key]['attribute_operations_'. $key]),
          'class' => 'storm_list_operations',
        ),
      );
      $row['data'] = $data;
      $row['class'] = empty($row['class']) ? 'draggable' : $row['class'] .' draggable';
      $rows[] = $row;
    }
  }
  $o = theme('table', $header, $rows, array('id' => 'attributes'));
  return $o;
}
