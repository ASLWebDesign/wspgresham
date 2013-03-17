<?php
/**
 * @file
 * Functions to expose Storm Invoice module data to the views module
 */
function storminvoice_views_data() {
  // Invoice core fields
  $data['storminvoice']['table']['group']  = t('Storm Invoice');
  $data['storminvoice']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['storminvoice']['number'] = array(
    'title' => t('Number'),
    'help' => t('Storm Invoice Number'),
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

  $data['storminvoice']['reference'] = array(
    'title' => t('Reference'),
    'help' => t('Storm Invoice Reference'),
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

  $data['storminvoice']['organization_nid'] = array(
    'title' => t('Organization'),
    'help' => t('Invoice -> Organization'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Invoice -> Organization'),
    ),
  );

  $data['storminvoice']['organization_title'] = array(
    'title' => t('Organization'),
    'help' => t('Storm Invoice Organization (title only)'),
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

  $data['storminvoice']['project_nid'] = array(
    'title' => t('Project'),
    'help' => t('Invoice -> Project'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Invoice -> Project'),
    ),
  );

  $data['storminvoice']['project_title'] = array(
    'title' => t('Project'),
    'help' => t('Storm Invoice Project (title only)'),
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

  $data['storminvoice']['requestdate'] = array(
    'title' => t('Request date'),
    'help' => t('Storm Invoice Request Date'),
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

  $data['storminvoice']['duedate'] = array(
    'title' => t('Due date'),
    'help' => t('Storm Invoice Due Date'),
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

  $data['storminvoice']['paymentdate'] = array(
    'title' => t('Payment date'),
    'help' => t('Storm Invoice Payment Date'),
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

  $data['storminvoice']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('Storm Invoice Amount'),
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

  $data['storminvoice']['tax1'] = array(
    'title' => t('Tax 1'),
    'help' => t('Storm Invoice Tax 1'),
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

  $data['storminvoice']['tax2'] = array(
    'title' => t('Tax 2'),
    'help' => t('Storm Invoice Tax 2'),
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

  $data['storminvoice']['total'] = array(
    'title' => t('Total'),
    'help' => t('Storm Invoice Total'),
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

  $data['storminvoice']['totalcustomercurr'] = array(
    'title' => t('Total in customer currency'),
    'help' => t('Storm Invoice Total in Customer Currency'),
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

  $data['storminvoice']['taxexempt'] = array(
    'title' => t('Tax exempt'),
    'help' => t('Storm Invoice Tax Exempt'),
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

  $data['storminvoice']['invoice_status'] = array(
    'title' => t('Invoice status'),
    'help' => t('Storm Invoice Status'),
    'field' => array(
      'click sortable' => TRUE,
      'handler' => 'storminvoice_handler_field_invoice_status'
    ),
    'filter' => array(
      'handler' => 'storminvoice_handler_filter_invoice_status',
    ),
  );

  // Invoice items
  $data['storminvoice_items']['table']['group']  = t('Storm Invoice Item');
  $data['storminvoice_items']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'invoice_nid',
    ),
  );

  $data['storminvoice_items']['invoice_nid'] = array(
    'title' => t('Invoice'),
    'help' => t('Invoiceitem -> Invoice'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Invoiceitem -> Invoice'),
    ),
  );

  $data['storminvoice_items']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('Storm Invoiceitem Weight'),
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

  $data['storminvoice_items']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('Storm Invoiceitem Amount'),
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

  $data['storminvoice_items']['tax1percent'] = array(
    'title' => t('Tax 1 Percent'),
    'help' => t('Storm Invoiceitem Tax 1 Percent'),
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

  $data['storminvoice_items']['tax2percent'] = array(
    'title' => t('Tax 2 Percent'),
    'help' => t('Storm Invoiceitem Tax 2 Percent'),
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

  $data['storminvoice_items']['tax1'] = array(
    'title' => t('Tax 1'),
    'help' => t('Storm Invoiceitem Tax 1'),
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

  $data['storminvoice_items']['tax2'] = array(
    'title' => t('Tax 2'),
    'help' => t('Storm Invoiceitem Tax 2'),
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

  $data['storminvoice_items']['total'] = array(
    'title' => t('Total'),
    'help' => t('Storm Invoiceitem Total'),
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

  $data['storminvoice']['operation'] = array(
    'field' => array(
      'title' => t('Edit/Delete link'),
      'help' => t('Provide a simple link to edit and delete the node.'),
      'handler' => 'storm_handler_field_operation',
      'type' => 'storminvoice',
    ),
  );

  return $data;
}

function storminvoice_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'storminvoice'),
    ),
    'handlers' => array(
      'storm_handler_filter_attributes_domain' => array(
        'parent' => 'views_handler_filter_in_operator',
        'path' => drupal_get_path('module', 'storm'),
      ),
      'storm_handler_field_operation' => array(
        'parent' => 'views_handler_field_node_link',
        'path' => drupal_get_path('module', 'storm'),
      ),
      'storminvoice_handler_field_invoice_status' => array(
        'parent' => 'views_handler_field',
      ),
      'storminvoice_handler_filter_invoice_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

function storminvoice_date_api_fields($field) {
  $values = array(
    'sql_type' => DATE_UNIX,
    'tz_handling' => 'site',
    'timezone_field' => '',
    'offset_field' => '',
    'related_fields' => array(),
    'granularity' => array('year', 'month', 'day'),
  );

  switch ($field) {
    case 'storminvoice.paymentdate':
    case 'storminvoice.duedate':
    case 'storminvoice.requestdate':
      return $values;
  }

}

function storminvoice_date_api_tables() {
  return array('storminvoice');
}
