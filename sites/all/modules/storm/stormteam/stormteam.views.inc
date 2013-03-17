<?php
/**
 * @file
 * Functions to expose Storm Team module data to the Views module.
 */
function stormteam_views_data() {
  $data['stormteam']['table']['group']  = 'Storm Team';
  $data['stormteam']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormteam']['mnid'] = array(
    'title' => t('Team member nid'),
    'help' => 'Storm Team Members',
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'parent' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_nid',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Member -> Node'),
    ),
  );

  $data['stormteam']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormteam',
    ),
  );

  return $data;
}

function stormteam_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'storm'),
    ),
    'handlers' => array(
      'storm_handler_field_operation' => array(
        'parent' => 'views_handler_field_node_link',
      ),
    ),
  );
}
