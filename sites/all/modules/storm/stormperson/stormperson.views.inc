<?php
/**
 * @file
 * Functions to expose Storm Person module data to the Views module.
 */
function stormperson_views_data() {
  $data['stormperson']['table']['group']  = 'Storm Person';
  $data['stormperson']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormperson']['organization_nid'] = array(
    'title' => t('Organization'),
    'help' => t('Person -> Organization'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Person -> Organization'),
    ),
  );

  $data['stormperson']['organization_title'] = array(
    'title' => t('Organization'),
    'help' => t('Storm Person Organization (title only)'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['stormperson']['prefix'] = array(
    'title' => t('Prefix'),
    'help' => t('Storm Person Prefix'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['stormperson']['email'] = array(
    'title' => t('Email'),
    'help' => t('Storm Person Email'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['stormperson']['www'] = array(
    'title' => t('WWW'),
    'help' => t('Storm Person WWW'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['stormperson']['phone'] = array(
    'title' => t('Phone'),
    'help' => t('Storm Person Phone'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['stormperson']['im'] = array(
    'title' => t('IM'),
    'help' => t('Storm Person IM'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['stormperson']['user_uid'] = array(
    'title' => t('Drupal user'),
    'help' => t('Storm Person -> Drupal User'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Person -> User'),
    ),
  );

  $data['stormperson']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormperson',
    ),
  );

  return $data;
}

function stormperson_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'storm'),
    ),
    'handlers' => array(
      'storm_handler_filter_attributes_domain' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'storm_handler_field_operation' => array(
        'parent' => 'views_handler_field_node_link',
        'path' => drupal_get_path('module', 'storm'),
      ),
    ),
  );
}
