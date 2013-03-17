<?php
/**
 * @file
 * Functions to expose Storm Task module data to the Views module.
 */
function stormtask_views_data() {
  $data['stormtask']['table']['group']  = 'Storm Task';
  $data['stormtask']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormtask']['stepno'] = array(
    'title' => t('Step number'),
    'help' => t('Storm Task Step Number'),
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

  $data['stormtask']['organization_nid'] = array(
    'title' => t('Organization'),
    'help' => t('Task -> Organization'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Task -> Organization'),
    ),
  );

  $data['stormtask']['organization_title'] = array(
    'title' => t('Organization'),
    'help' => t('Storm Task Organization (title only)'),
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

  $data['stormtask']['project_nid'] = array(
    'title' => t('Project'),
    'help' => t('Task -> Project'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Task -> Project'),
    ),
  );

  $data['stormtask']['project_title'] = array(
    'title' => t('Project'),
    'help' => t('Storm Task Project (title only)'),
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

  $data['stormtask']['parent_nid'] = array(
    'title' => t('Parent task'),
    'help' => t('Task -> Parent Task'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Task -> Parent Task'),
    ),
  );

  $data['stormtask']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('Storm Task Weight'),
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

  $data['stormtask']['taskstatus'] = array(
    'title' => t('Status'),
    'help' => t('Storm Task Status'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Task Status',
      'icon' => 'status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Task Status',
    ),
  );

  $data['stormtask']['taskcategory'] = array(
    'title' => t('Category'),
    'help' => t('Storm Task Category'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Task Category',
      'icon' => 'category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Task Category',
    ),
  );

  $data['stormtask']['taskpriority'] = array(
    'title' => t('Priority'),
    'help' => t('Storm Task Priority'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Task Priority',
      'icon' => 'priority',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Task Priority',
    ),
  );

  $data['stormtask']['pricemode'] = array(
    'title' => t('Price mode'),
    'help' => t('Storm Task Price Mode'),
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

  $data['stormtask']['price'] = array(
    'title' => t('Price'),
    'help' => t('Storm Task Price'),
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

  $data['stormtask']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Storm Task Currency'),
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

  $data['stormtask']['datebegin'] = array(
    'title' => t('Date begin'),
    'help' => t('Storm Task Date Begin'),
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

  $data['stormtask']['dateend'] = array(
    'title' => t('Date end'),
    'help' => t('Storm Task Date End'),
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

  $data['stormtask']['durationunit'] = array(
    'title' => t('Duration unit'),
    'help' => t('Storm Task Duration Unit'),
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

  $data['stormtask']['duration'] = array(
    'title' => t('Duration'),
    'help' => t('Storm Task Duration'),
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

  $data['stormtask']['assigned_nid'] = array(
    'title' => t('Assigned'),
    'help' => t('Task -> Assigned'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Task -> Assigned'),
    ),
  );

  $data['stormtask']['assigned_title'] = array(
    'title' => t('Assigned'),
    'help' => t('Storm Task Assigned (title only)'),
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

  $data['stormtask']['billable'] = array(
    'title' => t('Task billable'),
    'help' => 'Storm Task Billable',
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

  $data['stormtask']['billed'] = array(
    'title' => t('Task billed'),
    'help' => 'Storm Task Billed',
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

  $data['stormtask']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormtask',
    ),
  );

  return $data;
}

function stormtask_views_handlers() {
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
function stormtask_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX,
    'tz_handling' => 'site',
    'timezone_field' => '',
    'offset_field' => '',
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
  );

  switch ($field) {
    case 'stormtask.datebegin':
    case 'stormtask.dateend':
      return $values;
  }

}

/**
 * Implement hook_date_api_tables().
 */
function stormtask_date_api_tables() {
  return array('stormtask');
}
