<?php
/**
 * @file
 * Functions to expose Storm Project data to the views module
 */

function stormproject_views_data() {
  $data['stormproject']['table']['group']  = 'Storm Project';
  $data['stormproject']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormproject']['organization_nid'] = array(
    'title' => t('Organization'),
    'help' => t('Project -> Organization'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Project -> Organization'),
    ),
  );

  $data['stormproject']['organization_title'] = array(
    'title' => t('Organization'),
    'help' => t('Storm Project Organization (title only)'),
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

  $data['stormproject']['projectstatus'] = array(
    'title' => t('Status'),
    'help' => t('Storm Project Status'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Project Status',
      'icon' => 'status',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Project Status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormproject']['projectcategory'] = array(
    'title' => t('Category'),
    'help' => t('Storm Project Category'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Project Category',
      'icon' => 'projectcategory',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Project Category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormproject']['projectpriority'] = array(
    'title' => t('Priority'),
    'help' => t('Storm Project Priority'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Project Priority',
      'icon' => 'priority'
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Project Priority',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormproject']['pricemode'] = array(
    'title' => t('Price mode'),
    'help' => t('Storm Project Price Mode'),
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

  $data['stormproject']['price'] = array(
    'title' => t('Price'),
    'help' => t('Storm Project Price'),
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

  $data['stormproject']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Storm Project Currency'),
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

  $data['stormproject']['datebegin'] = array(
    'title' => t('Date begin'),
    'help' => t('Storm Project Date Begin'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['stormproject']['dateend'] = array(
    'title' => t('Date end'),
    'help' => t('Storm Project Date End'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['stormproject']['duration unit'] = array(
    'title' => t('Duration unit'),
    'help' => t('Storm Project Duration Unit'),
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

  $data['stormproject']['duration'] = array(
    'title' => t('Duration'),
    'help' => t('Storm Project Duration'),
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

  $data['stormproject']['manager_nid'] = array(
    'title' => t('Manager'),
    'help' => t('Project -> Manager'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Project -> Manager'),
    ),
  );

  $data['stormproject']['manager_title'] = array(
    'title' => t('Project manager'),
    'help' => t('Storm Project Manager (title only)'),
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

  $data['stormproject']['assigned_nid'] = array(
    'title' => t('Assigned'),
    'help' => t('Project -> Assigned'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Project -> Assigned'),
    ),
  );

  $data['stormproject']['assigned_title'] = array(
    'title' => t('Assigned'),
    'help' => t('Storm Project Assigned (title only)'),
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

  $data['stormproject']['billable'] = array(
    'title' => t('Project billable'),
    'help' => 'Storm Project Billable',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['stormproject']['billed'] = array(
    'title' => t('Project billed'),
    'help' => 'Storm Project Billed',
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['stormproject']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormproject',
    ),
  );

  return $data;
}

function stormproject_views_handlers() {
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


/**
 * Implementation of hook_date_api_fields().
 * on behalf of core fields.
 *
 * All modules that create custom fields that use the
 * 'views_handler_field_date' handler can provide
 * additional information here about the type of
 * date they create so the date can be used by
 * the Date API views date argument and date filter.
 */

function stormproject_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX,
    'tz_handling' => 'site',
    'timezone_field' => '',
    'offset_field' => '',
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
  );

  switch ($field) {
    case 'stormproject.datebegin':
    case 'stormproject.dateend':
      return $values;
  }
}

/**
 * Implement hook_date_api_tables().
 */
function stormproject_date_api_tables() {
  return array('stormproject');
}
