<?php
/**
 * @file
 * Functions to expose Storm Ticket module data to the Views module.
 */
function stormticket_views_data() {
  $data['stormticket']['table']['group']  = 'Storm Ticket';
  $data['stormticket']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['stormticket']['organization_nid'] = array(
    'title' => t('Organization'),
    'help' => t('Ticket -> Organization'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Ticket -> Organization'),
    ),
  );

  $data['stormticket']['organization_title'] = array(
    'title' => t('Organization'),
    'help' => t('Storm Ticket Organization (title only)'),
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

  $data['stormticket']['project_nid'] = array(
    'title' => t('Project'),
    'help' => t('Ticket -> Project'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Ticket -> Project'),
    ),
  );

  $data['stormticket']['project_title'] = array(
    'title' => t('Project'),
    'help' => t('Storm Ticket Project (title only)'),
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

  $data['stormticket']['task_nid'] = array(
    'title' => t('Task'),
    'help' => t('Ticket -> Task'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Ticket -> Task'),
    ),
  );

  $data['stormticket']['ticketcategory'] = array(
    'title' => t('Category'),
    'help' => t('Storm Ticket category'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Ticket Category',
      'icon' => 'category',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Ticket Category',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['ticketstatus'] = array(
    'title' => t('Status'),
    'help' => t('Storm Ticket Status'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Ticket Status',
      'icon' => 'status',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Ticket Status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['ticketpriority'] = array(
    'title' => t('Priority'),
    'help' => t('Storm Ticket priority'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Ticket Priority',
      'icon' => 'priority',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Ticket Priority',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['task_title'] = array(
    'title' => t('Task'),
    'help' => t('Storm Ticket Task (title only)'),
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

  $data['stormticket']['datebegin'] = array(
    'title' => t('Date begin'),
    'help' => t('Storm Ticket begin date'),
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

  $data['stormticket']['dateend'] = array(
    'title' => t('Date end'),
    'help' => t('Storm Ticket end date'),
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

  $data['stormticket']['durationunit'] = array(
    'title' => t('Duration unit'),
    'help' => t('Storm Ticket duration unit'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storm_handler_field_attributes_domain',
      'domain' => 'Duration unit',
    ),
    'filter' => array(
      'handler' => 'storm_handler_filter_attributes_domain',
      'domain' => 'Duration unit',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['stormticket']['duration'] = array(
    'title' => t('Duration'),
    'help' => t('Storm Ticket duration'),
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

  $data['stormticket']['assigned_nid'] = array(
    'title' => t('Assigned'),
    'help' => t('Ticket -> Assigned'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Ticket -> Assigned'),
    ),
  );

  $data['stormticket']['assigned_title'] = array(
    'title' => t('Assigned'),
    'help' => t('Storm Ticket Assigned (title only)'),
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

  $data['stormticket']['billable'] = array(
    'title' => t('Ticket billable'),
    'help' => 'Storm Ticket Billable',
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

  $data['stormticket']['billed'] = array(
    'title' => t('Ticket billed'),
    'help' => 'Storm Ticket Billed',
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

  $data['stormticket']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'stormticket',
    ),
  );

  return $data;
}

function stormticket_views_handlers() {
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

function stormticket_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX,
    'tz_handling' => 'site',
    'timezone_field' => '',
    'offset_field' => '',
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
  );

  switch ($field) {
    case 'stormticket.datebegin':
    case 'stormticket.dateend':
      return $values;
  }

}

function stormticket_date_api_tables() {
  return array('stormticket');
}
