<?php
/**
 * @file
 * Functions to expose Storm organization data to views
 */
function stormorganization_views_data() {
  $data['stormorganization']['table']['group']  = 'Storm Organization';
  $data['stormorganization']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormorganization']['prefix'] = array(
    'title' => t('Prefix'),
    'help' => t('Storm Organization Prefix'),
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

  $data['stormorganization']['fullname'] = array(
    'title' => t('Fullname'),
    'help' => t('Storm Organization Fullname'),
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

  $data['stormorganization']['country'] = array(
    'title' => t('Country'),
    'help' => t('Storm Organization Country'),
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

  $data['stormorganization']['orglanguage'] = array(
    'title' => t('Language'),
    'help' => t('Storm Organization Language'),
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

  $data['stormorganization']['provstate'] = array(
    'title' => t('Province / State'),
    'help' => t('Storm Organization Province / State'),
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

  $data['stormorganization']['city'] = array(
    'title' => t('City'),
    'help' => t('Storm Organization City'),
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

  $data['stormorganization']['zip'] = array(
    'title' => t('Zip'),
    'help' => t('Storm Organization Zip'),
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

  $data['stormorganization']['address'] = array(
    'title' => t('Address'),
    'help' => t('Storm Organization Address'),
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

  $data['stormorganization']['taxid'] = array(
    'title' => t('Tax ID'),
    'help' => t('Storm Organization Tax ID'),
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

  $data['stormorganization']['email'] = array(
    'title' => t('Email'),
    'help' => t('Storm Organization Email'),
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

  $data['stormorganization']['www'] = array(
    'title' => t('WWW'),
    'help' => t('Storm Organization WWW'),
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

  $data['stormorganization']['phone'] = array(
    'title' => t('Phone'),
    'help' => t('Storm Organization Phone'),
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

  $data['stormorganization']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Storm Organization Currency'),
    'field' => array(
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Currency',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Currency',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['stormorganization']['iscustomer'] = array(
    'title' => t('Is customer'),
    'help' => t('Storm Organization Is Customer'),
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

  $data['stormorganization']['isprovider'] = array(
    'title' => t('Is provider'),
    'help' => t('Storm Organization Is Provider'),
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

  $data['stormorganization']['isactive'] = array(
    'title' => t('Is active'),
    'help' => t('Storm Organization Is Active'),
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

  $data['stormorganization']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormorganization',
    ),
  );

  return $data;
}

function stormorganization_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'storm'),
    ),
    'handlers' => array(
      'storm_handler_field_attributes_domain' => array(
        'parent' => 'views_handler_field',
      ),
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
