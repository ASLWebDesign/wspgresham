<?php
/**
 * @file
 * Functions to expose Storm Timetracking Module data to the Views module.
 */
function stormtimetracking_views_data() {
  $data['stormtimetracking']['table']['group']  = 'Storm Timetracking';
  $data['stormtimetracking']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormtimetracking']['organization_nid'] = array(
    'title' => t('Organization'),
    'help' => t('Timetracking -> Organization'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Timetracking -> Organization'),
    ),
  );

  $data['stormtimetracking']['organization_title'] = array(
    'title' => t('Organization'),
    'help' => t('Storm Timetracking Organization (title only)'),
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

  $data['stormtimetracking']['project_nid'] = array(
    'title' => t('Project'),
    'help' => t('Timetracking -> Project'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Timetracking -> Project'),
    ),
  );

  $data['stormtimetracking']['project_title'] = array(
    'title' => t('Project'),
    'help' => t('Storm Timetracking Project (title only)'),
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

  $data['stormtimetracking']['task_nid'] = array(
    'title' => t('Task'),
    'help' => t('Timetracking -> Task'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Timetracking -> Task'),
    ),
  );

  $data['stormtimetracking']['task_stepno'] = array(
    'title' => t('Task step number'),
    'help' => t('Storm Timetracking Task Step Number'),
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

  $data['stormtimetracking']['task_title'] = array(
    'title' => t('Task'),
    'help' => t('Storm Timetracking Task (title only)'),
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

  $data['stormtimetracking']['ticket_nid'] = array(
    'title' => t('Ticket'),
    'help' => t('Timetracking -> Ticket'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Timetracking -> Ticket'),
    ),
  );

  $data['stormtimetracking']['ticket_title'] = array(
    'title' => t('Ticket'),
    'help' => t('Storm Timetracking Ticket (title only)'),
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

  $data['stormtimetracking']['trackingdate'] = array(
    'title' => t('Tracking date'),
    'help' => t('Storm Timetracking Tracking Date'),
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

  $data['stormtimetracking']['timebegin'] = array(
    'title' => t('Time begin'),
    'help' => t('Storm Timetracking Time Begin'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtimetracking']['timeend'] = array(
    'title' => t('Time end'),
    'help' => t('Storm Timetracking Time End'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtimetracking']['duration'] = array(
    'title' => t('Duration'),
    'help' => t('Storm Timetracking Duration'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtimetracking']['billing_duration'] = array(
    'title' => t('Billing duration'),
    'help' => t('Storm Timetracking Billing Duration'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormtimetracking']['billable'] = array(
    'title' => t('Billable'),
    'help' => t('Storm Timetracking Billable'),
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

  $data['stormtimetracking']['billed'] = array(
    'title' => t('Billed'),
    'help' => t('Storm Timetracking Billed'),
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

  $data['stormtimetracking']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormtimetracking',
    ),
  );

  return $data;
}

function stormtimetracking_views_handlers() {
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

function stormtimetracking_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX,
    'tz_handling' => 'site',
    'timezone_field' => '',
    'offset_field' => '',
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day'),
  );

  switch ($field) {
    case 'stormtimetracking.trackingdate':
      return $values;
  }

}

function stormtimetracking_date_api_tables() {
  return array('stormtimetracking');
}
