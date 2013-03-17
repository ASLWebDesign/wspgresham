<?php
/**
 * @file
 * Themable functions for Storm invoice component.
 */

/**
 * Returns the HTML code for the invoices list
 *
 * @param $header
 *   Array with the fields headers.
 * @param $invoices
 *   Array with the list of invoices.
 * @param $itempsperpage
 *   Number of items to show on every page.
 * @param $totals_topay
 *   Total amount yet to pay.
 * @param $totals_paid
 *   Total amount already paid.
 * @param $totals
 *   Grand total.
 * @return
 *   HTML of the invoices list.
 */
function theme_storminvoice_list($header, $invoices, $itemsperpage, $totals_topay, $totals_paid, $totals) {
  $rows = array();
  foreach ($invoices as $invoice) {

    $invoice->status = 'open';
    if ($invoice->paymentdate) {
      $invoice->status = 'paid';
    }
    elseif ($invoice->duedate < time()) {
      $invoice->status = 'overdue';
    }

    $rows[] = array(
      array(
        'data' => storm_icon('invoice_status_'. check_plain($invoice->status), storm_attribute_value('Invoice status', $invoice->status)),
        'style' => 'text-align: center',
      ),
      array(
        'data' => l($invoice->number, 'node/'. $invoice->nid),
        'style' => 'text-align: right',
      ),
      l($invoice->organization_title, 'node/'. $invoice->organization_nid),
      l($invoice->project_title, 'node/'. $invoice->project_nid),
      l($invoice->title, 'node/'. $invoice->nid),
      format_date($invoice->requestdate, 'custom', 'Y-m-d'),
      array('data' => sprintf('%.2f', $invoice->total), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($invoice, $_GET) .'&nbsp;'. storm_icon_delete_node($invoice, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }

  $o = theme('table', $header, $rows);
  $o .= theme('pager', NULL, $itemsperpage, 0);

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => t('Amount'),
      'style' => 'text-align: right;',
    ),
  );

  $header[] = array(
    'data' => t(variable_get('storm_tax1_name', 'Tax 1')),
    'style' => 'text-align: right;',
  );
  $header[] = array(
    'data' => t(variable_get('storm_tax2_name', 'Tax 2')),
    'style' => 'text-align: right;',
  );
  $header[] = array(
    'data' => t('Total'),
    'style' => 'text-align: right;',
  );

  $rows = array();
  $row = array(
    array(
      'data' => t('Total to pay'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->amount),
      'style' => 'text-align: right;',
    ),
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals_topay->tax1),
    'style' => 'text-align: right;',
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals_topay->tax2),
    'style' => 'text-align: right;',
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals_topay->total),
    'style' => 'text-align: right;',
  );
  $rows[] = $row;

  $row = array(
    array(
      'data' => t('Total paid'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->amount),
      'style' => 'text-align: right;',
    ),
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals_paid->tax1),
    'style' => 'text-align: right;',
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals_paid->tax2),
    'style' => 'text-align: right;',
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals_paid->total),
    'style' => 'text-align: right;',
  );
  $rows[] = $row;

  $row = array(
    array(
      'data' => t('Total'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->amount),
      'style' => 'text-align: right;',
    ),
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals->tax1),
    'style' => 'text-align: right;',
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals->tax2),
    'style' => 'text-align: right;',
  );
  $row[] = array(
    'data' => sprintf('%.2f', $totals->total),
    'style' => 'text-align: right;',
  );
  $rows[] = $row;

  $o .= theme('table', $header, $rows);

  return $o;
}

/**
 * Returns the HTML code for the invoice view page
 *
 * @param $node
 *   The node object that contains the invoice.
 * @param $teaser
 *   True if the teaser is showed.
 * @param $page
 *   True if the full page is showed.
 * @return
 *   The node object that contains the invoice.
 */
function theme_storminvoice_view($node, $teaser = FALSE, $page = FALSE) {
  // Add generic Storm node CSS
  drupal_add_css(drupal_get_path('module', 'storm') . '/storm-node.css', 'module');
  // Adds CSS for Storm Invoice node view
  drupal_add_css(drupal_get_path('module', 'storminvoice') . '/storminvoice-nodeview.css');

  $node = node_prepare($node, $teaser);
  $l_pos = 1; // Used to increase the link position number (see issue 814820)

  $node->content['links'] = array(
    '#prefix' => '<div class="stormlinks"><dl>',
    '#suffix' => '</dl></div>',
    '#weight' => -25,
  );

  // Bodge fix to keep html and pdf invoices working. Without language, menu items default to /storm
  if (!$node->language) {
    $node->language = 'en';
  }

  $node->content['links']['print']['html'] = array(
    '#prefix' => '<dt class="stormcomponent print_html">',
    '#suffix' => '</dt>',
    '#value' => l(t('Print HTML'), 'storm/invoice/report/'. $node->nid .'/html/'. $node->language),
    '#weight' => $l_pos++,
  );

  // Display link to tcpdf library only if library is installed.
  $tcpdf_dir = variable_get('storminvoice_tcpdf_location', 'sites/all/libraries/tcpdf');
  if (file_exists($tcpdf_dir .'/tcpdf.php')) {
    $node->content['links']['print']['pdf'] = array(
      '#prefix' => '<dt class="stormcomponent print_pdf">',
      '#suffix' => '</dt>',
      '#value' => l(t('Print PDF'), 'storm/invoice/report/'. $node->nid .'/pdf/'. $node->language),
      '#weight' => $l_pos++,
    );
      $node->content['links']['print']['email'] = array(
      '#value' => t('Send PDF via e-mail'),
      '#prefix' => '<dt class="stormcomponent print_pdf">',
      '#suffix' => '</dt>',
      '#value' => l(t('Send PDF via e-mail'), 'storm/invoice/report/'. $node->nid .'/email/'. $node->language, array('query' => drupal_get_destination())),
      '#weight' => $l_pos++,
    );
  }

  $node->content['group1'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group1') : -20,
  );

  $node->content['group1']['number'] = array(
    '#prefix' => '<div class="number">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Number'), $node->number),
    '#weight' => 1,
  );

  $node->content['group2'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group2') : -19,
  );

  $node->content['group2']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Organization'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => 1,
  );

  $node->content['group2']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Project'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => 2,
  );

  if ($node->reference) {
    $node->content['group2']['reference'] = array(
      '#prefix' => '<div class="reference">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', t('Reference'), check_plain($node->reference)),
      '#weight' => 3,
    );
  }

  $node->content['group3'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group3') : -18,
  );

  $node->content['group3']['requestdate'] = array(
    '#prefix' => '<div class="requestdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Request date'), format_date($node->requestdate, 'custom', 'Y-m-d')),
    '#weight' => 1,
  );

  $node->content['group3']['duedate'] = array(
    '#prefix' => '<div class="duedate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Due date'), format_date($node->duedate, 'custom', 'Y-m-d')),
    '#weight' => 2,
  );

  if ($node->paymentdate) {
    $node->content['group3']['paymentdate'] = array(
      '#prefix' => '<div class="paymentdate">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', t('Payment date'), $node->paymentdate? format_date($node->paymentdate, 'custom', 'Y-m-d') : ''),
      '#weight' => 3,
    );
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  elseif ($node->duedate < time()) {
    $status = 'overdue';
  }
  $node->content['group3']['status'] = array(
    '#prefix' => '<div class="invoicestatus">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Status'), $status),
    '#weight' => 4,
  );

  // Processing for invoice items.
  $header = array(
    array(
      'data' => t('Item'),
    ),
    array(
      'data' => t('Amount'),
      // 'style' => 'text-align: right;',
    ),
  );

  if ($node->tax1) {
    $header[] = array(
      'data' => t(variable_get('storm_tax1_name', 'Tax 1')),
      // 'style' => 'text-align: right;',
    );
  }

  if ($node->tax2) {
    $header[] = array(
      'data' => t(variable_get('storm_tax2_name', 'Tax 2')),
      // 'style' => 'text-align: right;',
    );
  }

  $header[] = array(
      'data' => t('Total'),
      // 'style' => 'text-align: right;',
    );

  $where = array();
  $s  = "SELECT sit.description, sit.amount, ";
  if ($node->tax1) {
    $s .= "sit.tax1, ";
  }
  if ($node->tax2) {
    $s .= "sit.tax2, ";
  }
  $s .= "sit.total, sit.src_nid FROM {storminvoice_items} sit WHERE sit.invoice_vid=%d ORDER BY sit.weight ASC";
  $r = db_query($s, $node->vid);

  $invoice_items = array();
  $i = 0;
  while ($invoice_item = db_fetch_array($r)) {
    $invoice_items[$i] = $invoice_item;
    $i++;
  }

  foreach ($invoice_items as $key => $inv_item) {
    // PHP4 compatibility - for D7 change this to use the original array by reference
    $invoice_items[$key]['amount'] = sprintf("%.2f", $inv_item['amount']);
    if ($node->tax1) {
    $invoice_items[$key]['tax1'] = sprintf("%.2f", $inv_item['tax1']);
    }
    if ($node->tax2) {
    $invoice_items[$key]['tax2'] = sprintf("%.2f", $inv_item['tax2']);
    }
    $invoice_items[$key]['total'] = sprintf("%.2f", $inv_item['total']);
    if ($invoice_items[$key]['src_nid'] != 0 AND $invoice_items[$key]['src_nid'] != NULL) {
      $invoice_items[$key]['description'] = l(t($invoice_items[$key]['description']), 'node/'. $inv_item['src_nid']);
    }
    unset($invoice_items[$key]['src_nid']);
  }

  $node->content['group4'] = array(
    '#prefix' => '<div class="stormfields">',
    '#value' => theme('table', $header, $invoice_items),
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group4') : -17,
  );

  $node->content['group5'] = array(
    '#prefix' => '<div class="stormfields">',
    '#suffix' => '</div>',
    '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'group5') : -16,
  );

  $node->content['group5']['amount'] = array(
    '#prefix' => '<div class="amount">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Amount'), sprintf('%.2f', $node->amount)),
    '#weight' => 1,
  );

  if ($node->tax1) {
  $node->content['group5']['tax1'] = array(
    '#prefix' => '<div class="tax">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t(variable_get('storm_tax1_name', 'Tax 1')), sprintf('%.2f', $node->tax1)),
    '#weight' => 2,
  );
  }

  if ($node->tax2) {
  $node->content['group5']['tax2'] = array(
    '#prefix' => '<div class="tax">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t(variable_get('storm_tax2_name', 'Tax 2')), sprintf('%.2f', $node->tax2)),
    '#weight' => 3,
  );
  }

  $node->content['group5']['total'] = array(
    '#prefix' => '<div class="total">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', t('Total'), sprintf('%.2f', $node->total)),
    '#weight' => 4,
  );

  $organization = node_load($node->organization_nid);
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  if (isset($myorg->orglanguage)) {
    if ($myorg->orglanguage != $organization->orglanguage) {
      $language = $organization->orglanguage .','. $myorg->orglanguage;
    }
    else {
      $language = $myorg->orglanguage;
    }
  }

  if (isset($myorg->currency)) {
    $currencies = storm_attributes_bydomain('Currency');
    $node->content['group5']['currency'] = array(
      '#prefix' => '<div class="currency">',
      '#suffix' => '</div>',
      '#value' => t('Unless stated, amounts shown on this invoice are quoted in %currency.', array('%currency' => $currencies['values'][$myorg->currency])),
      '#weight' => 5,
    );
  }

  if ($node->content['body']['#value']) {
    $node->content['body_field'] = array(
      '#prefix' => '<div class="stormbody">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', t('Description'), $node->content['body']['#value']),
      '#weight' => module_exists('content') ? content_extra_field_weight($node->type, 'body_field') : -18,
    );
    unset($node->content['body']);
  }

  return $node;
}

function theme_storminvoice_report($node, $report, $language) {
  if ($report=='html') {
    print theme('storminvoice_report_html', $node, $language);
  }
  elseif ($report=='pdf') {
    theme('storminvoice_report_pdf', $node, $language);
  }
}

function theme_storminvoice_report_pdf($node, $language, $output = 'screen') {
  $tcpdf_dir = variable_get('storminvoice_tcpdf_location', 'sites/all/libraries/tcpdf');

  // Performs simple check for existance of tcpdf library . If it doesn't exist,  revert to node display with message about tcpdf library.
  if (!file_exists($tcpdf_dir .'/tcpdf.php')) {
    drupal_set_message(t('The tcpdf library has not been installed. See the Storm module README.txt for more details.'));
    drupal_goto('node/'. $node->nid);
  }

  require_once($tcpdf_dir .'/config/lang/eng.php');
  require_once($tcpdf_dir .'/tcpdf.php');
  $languages = explode(',', $language);
  $language = $languages[0];
  $language1 = '';
  if (array_key_exists(1, $languages)) {
    $language1 = $languages[1];
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  elseif ($node->duedate < time()) {
    $status = 'overdue';
  }

  $countries = storm_attributes_bydomain('Country');
  $countries = $countries['values'];
  $currencies = storm_attributes_bydomain('Currency');
  $currencies = $currencies['values'];
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $mycurrency = $currencies[$myorg->currency];

  $organization = node_load($node->organization_nid);
  $project = node_load($node->project_nid);

  $o = '';
  $title = t('Invoice', array(), $language);
  $complete_title = $title .' '. $myorg->title .' : '. $organization->title .' - '. $node->number;

  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, TRUE);
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("Storm");
  $pdf->SetTitle($complete_title);
  $pdf->SetSubject($title);
  $pdf->SetKeywords($title, $myorg->title, $organization->title, $node->number);
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);
  $margins = $pdf->getMargins();
  $pageWidth = $pdf->getPageWidth() - $margins['left'] - $margins['right'];
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);
  $pdf->AddPage();
  $pdf->setDrawColor(204, 204, 204);
  $pdf->setFillColor(220, 220, 220);

  $pdf->SetFont("times", "B", 14);
  $headerleft = variable_get('site_name', '') .'<br />'. variable_get('site_slogan', '');
  $pdf->writeHTMLCell($pageWidth * .5, 0, $pdf->getX(), $pdf->getY(), $headerleft, 0 , 0 , 0, FALSE, 'L');
  $pdf->SetFont("times", "N", 10);
  $pdf->writeHTMLCell($pageWidth * .5, 0, $pdf->getX(), $pdf->getY(), variable_get('storm_report_header', ''), 0 , 1, 0, FALSE, 'R');
  $pdf->SetFont("times", "B", 14);
  $o = $title;
  if ($language1) $o .= "\n". t('Invoice', array(), $language1);
  $pdf->MultiCell(0, 0, $o, 0 , 'C', 0, 1, $pdf->getX(), $pdf->getY() + 10);

  $y = $pdf->getY() + 10;

  $pdf->SetFont("times", "B", 10);
  $o = t('Bill to', array(), $language);
  if ($language1) $o .= "\n". t('Bill to', array(), $language1);
  $pdf->MultiCell($pageWidth * .4, 0, $o, 'B', 'L', 0, 1, PDF_MARGIN_LEFT, $y);
  $o =  $organization->title ."\n";
  $o .= $organization->address ."\n";
  $o .= $organization->city ."\n";
  if ($organization->zip) $o .= $organization->zip .' ';
  if ($organization->provstate) $o .= $organization->provstate .' ';
  $o .= $countries[$organization->country] ."\n";;
  if ($organization->taxid) {
    $o .= t('Tax ID', array(), $language);
    if ($language1) $o .= ' / '. t('Tax ID', array(), $language1);
    $o .= ' : '. $organization->taxid;
  }
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($pageWidth * .4, 0, $o, 0 , 'L' , 0, 1, PDF_MARGIN_LEFT);
  $destY = $pdf->getY();

  $w = ($pageWidth *.5) / 4;

  $pdf->SetFont("times", "B", 10);

  $o = t('Invoice#', array(), $language);
  if ($language1) $o .= "\n". t('Invoice#', array(), $language1);
  $pdf->MultiCell($w-2, 0, $o, 1, 'L', 1, 0, PDF_MARGIN_LEFT + $pageWidth *.5, $y);

  $o = t('Currency', array(), $language);
  if ($language1) $o .= "\n". t('Currency', array(), $language1);
  $pdf->MultiCell($w-3, 0, $o, 1, 'L', 1, 0);
  $o = t('Date', array(), $language);
  if ($language1) $o .= "\n". t('Date', array(), $language1);
  $pdf->MultiCell($w+5, 0, $o, 1, 'L', 1, 0);
  $o = t('Reference', array(), $language);
  if ($language1) $o .= "\n". t('Reference', array(), $language1);
  $pdf->MultiCell($w, 0, $o, 1, 'L', 1, 1);

  $pdf->SetFont("times", "N", 10);
  $h = $pdf->getY();
  $pdf->MultiCell($w, 0, $node->reference?$node->reference:'-' , 1, 'L', 0, 1, PDF_MARGIN_LEFT + $pageWidth * .5 + $w * 3);
  $h = $pdf->getY() - $h;
  $pdf->MultiCell($w-2, $h, $node->number, 1, 'L', 0, 0, PDF_MARGIN_LEFT + $pageWidth *.5, $pdf->getY() - $h);
  $pdf->MultiCell($w-3, $h, $mycurrency, 1, 'L', 0, 0);
  $pdf->MultiCell($w+5, $h, format_date($node->requestdate, 'custom', 'Y-m-d'), 1, 'L', 0, 1);

  $pdf->SetFont("times", "B", 10);

  $o = t('Due total', array(), $language);
  if ($language1) $o .= "\n". t('Due total', array(), $language1);
  $pdf->MultiCell($w*2 - 5, 0, $o, 1, 'L', 1, 0, PDF_MARGIN_LEFT + $pageWidth *.5);

  $o = t('Due date', array(), $language);
  if ($language1) $o .= "\n". t('Due date', array(), $language1);
  $pdf->MultiCell($w + 5, 0, $o, 1, 'L', 1, 0);

  $o = t('Terms', array(), $language);
  if ($language1) $o .= "\n". t('Terms', array(), $language1);
  $pdf->MultiCell($w, 0, $o, 1, 'L', 1, 1);

  $pdf->SetFont("times", "B", 10);
  $o = $mycurrency .' '. sprintf('%.2f', $node->total);
  if ($organization->currency != $myorg->currency) {
    $o .= "\n". $organization->currency .' '. sprintf('%.2f', $node->totalcustomercurr);
  }
  $pdf->MultiCell($w * 2 - 5, 12, $o, 1, 'C', 0, 0, PDF_MARGIN_LEFT + $pageWidth * .5);
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($w + 5, 12, format_date($node->duedate, 'custom', 'Y-m-d'), 1, 'L', 0, 0);
  $pdf->MultiCell($w, 12, variable_get('storminvoice_payment_terms', ''), 1, 'L', 0, 1);

  $y = $pdf->getY();
  if ($destY > $y) $y = $destY;
  $pdf->setY($y+10);

  $pdf->setX(PDF_MARGIN_LEFT);
  $pdf->SetFont("times", "B", 10);

  $o = t('Description', array(), $language);
  if ($language1) $o .= "\n". t('Description', array(), $language1);
  $pdf->MultiCell($pageWidth * .4, 0, $o, 1, 'L', 1, 0);

  $o = t('Amount', array(), $language);
  if ($language1) $o .= "\n". t('Amount', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  if ($node->tax1) {
  $o = variable_get('storm_tax1_name', 'Tax 1');
  if ($language1) $o .= "\n". variable_get('storm_tax1_name', 'Tax 1');
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);
  }

  if ($node->tax2) {
  $o = variable_get('storm_tax2_name', 'Tax 2');
  if ($language1) $o .= "\n". variable_get('storm_tax2_name', 'Tax 2');
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);
  }

  $o = t('Total', array(), $language);
  if ($language1) $o .= "\n". t('Total', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 1);

  $pdf->SetFont("times", "N", 10);
  $items = storminvoice_getitems($node->vid);
  $rows = array();
  $pdf->setFillColor(245, 245, 245);
  $c = 0;
  foreach ($items as $i) {
    if ($c==2) $c=0;
    $y = $pdf->getY();
    $h = $pdf->getY();
    $pdf->MultiCell($pageWidth * .4, 0, $i->description, 1, 'L', $c, 1);
    $h = $pdf->getY() - $h;
    $pdf->setY($y);
    $pdf->setX(PDF_MARGIN_LEFT + $pageWidth * .4);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->amount), 1, 0, 'R', $c);
    if ($node->tax1) {
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->tax1), 1, 0, 'R', $c);
    }
    if ($node->tax2) {
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->tax2), 1, 0, 'R', $c);
    }
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->total), 1, 1, 'R', $c);
    $c++;
  }

  if ($node->taxexempt && $language1=='it') {
    $o = t('Tax exempt art. 7', array(), $language);
    if ($language1) $o .= "\n". t('Tax exempt art. 7', array(), $language1);
    $pdf->MultiCell($pageWidth, 0, $o, 0, 'L', 0, 1, $pdf->getX(), $pdf->getY() + 5);
  }

  $y = $pdf->getY() + 10;
  $pdf->setY($y);
  $pdf->SetFont("times", "B", 10);
  $pdf->Cell($pageWidth, 5, t('Payment', array(), $language), 'B', 0, 'L');
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($pageWidth, 0, variable_get('storminvoice_payment_modes', ''), 0, 'L', 0, 1, PDF_MARGIN_LEFT, $pdf->getY() + 5, TRUE, 0, TRUE);

  if ($status=='paid') {
    $y = $pdf->getY() + 10;
    $pdf->setY($y);
    $pdf->SetFont("times", "B", 14);
    $pdf->Cell(0, 12, t('Paid in full', array(), $language), 0, 1, 'C');
  }

  $filename = drupal_strtolower('invoice_'. str_replace('/', '-', $node->number)) .'.pdf';

  // Close and output PDF document
  if ($output == 'screen') {
    $pdf->Output($filename, "I");
  }
  elseif ($output == 'email') {
    $dir = file_directory_path() .'/';
    variable_set('storminvoice_email_pdf_file_path', $dir . $filename);
    variable_set('storminvoice_email_pdf_file_name', $filename);
    return $pdf->Output($dir . $filename, "S");
  }
}

function theme_storminvoice_report_html($node, $language) {
  $languages = explode(',', $language);
  $language = $languages[0];
  $language1 = '';
  if (array_key_exists(1, $languages)) {
    $language1 = $languages[1];
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  elseif ($node->duedate < time()) {
    $status = 'overdue';
  }

  $countries = storm_attributes_bydomain('Country');
  $countries = $countries['values'];
  $currencies = storm_attributes_bydomain('Currency');
  $currencies = $currencies['values'];
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $mycurrency = $currencies[$myorg->currency];

  $organization = node_load($node->organization_nid);
  $project = node_load($node->project_nid);

  $o = '';
  $title = t('Invoice', array(), $language);
  if ($language1) $title .= '<br />'. t('Invoice', array(), $language1);
  $headtitle = $title . ' '. $myorg->title . ' : ' . $organization->title . ' - ' . $node->number;

  $o .= '<div>';
  $o .= '<div id="storminvoice_billto"><table>';
  $o .= '<tr><td class="storminvoice_billto_title">';
  $o .= t('Bill To :', array(), $language);
  if ($language1) $o .= '<br />'. t('Bill to :', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= '<strong>'. $organization->title .'</strong>';
  $o .= '<br />';
  $o .= $organization->address;
  $o .= '<br />';
  $o .= $organization->city;
  $o .= '<br />';
  $o .= $organization->provstate .' '. $organization->zip .' '. $countries[$organization->country];
  $o .= '<br />';
  if ($organization->taxid) {
    $o .= t('Tax ID', array(), $language);
    if ($language1) $o .= ' / '. t('Tax ID', array(), $language1);
    $o .= ' : '. $organization->taxid;
  }
  $o .= '</td></tr>';
  $o .= '</table></div>';

  $o .= '<div id="storminvoice_details"><table>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Invoice#', array(), $language);
  if ($language1) $o .= '<br />'. t('Invoice#', array(), $language1);
  $o .='</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Currency', array(), $language);
  if ($language1) $o .= '<br />'. t('Currency', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Date', array(), $language);
  if ($language1) $o .= '<br />'. t('Date', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Reference', array(), $language);
  if ($language1) $o .= '<br />'. t('Reference', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td>'. $node->number .'</td>';
  $o .= '<td>'. $mycurrency .'</td>';
  $o .= '<td>'. format_date($node->requestdate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. $node->reference .'</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title" colspan="2">';
  $o .= t('Due total', array(), $language);
  if ($language1) $o .= '<br />'. t('Due total', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Due date', array(), $language);
  if ($language1) $o .= '<br />'. t('Due date', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= t('Terms', array(), $language);
  if ($language1) $o .= '<br />'. t('Terms', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td style="text-align: center; font-weight: bold;" colspan="2">';
  $o .= $mycurrency .' '. sprintf('%.2f', $node->total);
  if ($organization->currency != $myorg->currency) {
    $o .= '<br />';
    $o .= '('. $organization->currency .' '. sprintf('%.2f', $node->totalcustomercurr) .')';
  }
  $o .= '</td>';
  $o .= '<td>'. format_date($node->duedate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. variable_get('storminvoice_payment_terms', '') .'</td>';
  $o .= '</tr>';
  $o .= '</table></div>';
  $o .= '</div>';

  $description = t('Description', array(), $language);
  if ($language1) $description .= '<br />'. t('Description', array(), $language1);

  $amount = t('Amount', array(), $language);
  if ($language1) $amount .= '<br />'. t('Amount', array(), $language1);

  if ($node->tax1) {
  $tax1 = variable_get('storm_tax1_name', 'Tax 1');
  if ($language1) $tax1 .= '<br />'. variable_get('storm_tax1_name', 'Tax 1');
  }

  if ($node->tax2) {
  $tax2 = variable_get('storm_tax2_name', 'Tax 2');
  if ($language1) $tax2 .= '<br />'. variable_get('storm_tax2_name', 'Tax 2');
  }

  $total = t('Total', array(), $language);
  if ($language1) $total .= '<br />'. t('Total', array(), $language1);

  $header = array(
    array(
      'data' => $description,
    ),
    array(
      'data' => $amount,
    ),
  );
  if ($node->tax1) {
    $header[] = array(
      'data' => $tax1,
    );
  }
  if ($node->tax2) {
    $header[] = array(
      'data' => $tax2,
    );
  }
    $header[] = array(
      'data' => $total,
    );

  $items = storminvoice_getitems($node->vid);
  $rows = array();
  foreach ($items as $i) {
    $rows[] = array(
      array(
        'data' => $i->description,
      ),
      array(
        'data' => sprintf('%.2f', $i->amount),
        'style' => 'text-align: right;',
      ),
    );
    if ($node->tax1) {
      $rows[end(array_keys($rows))][] = array(
        'data' => sprintf('%.2f', $i->tax1),
        'style' => 'text-align: right;',
      );
    }
    if ($node->tax2) {
      $rows[end(array_keys($rows))][] = array(
        'data' => sprintf('%.2f', $i->tax2),
        'style' => 'text-align: right;',
      );
    }
      $rows[end(array_keys($rows))][] = array(
        'data' => sprintf('%.2f', $i->total),
        'style' => 'text-align: right;',
      );
  }

  $total = t('Total', array(), $language);
  if ($language1) $total .= ' / '. t('Total', array(), $language1);

  $rows[] = array(
    array(
      'data' => $total,
      'style' => 'font-weight: bold; text-align: center; background-color: #e9e9e9;',
    ),
    array(
      'data' => sprintf('%.2f', $node->amount),
      'style' => 'text-align: right;',
    ),
  );

  if ($node->tax1) {
    $rows[end(array_keys($rows))][] = array(
      'data' => sprintf('%.2f', $node->tax1),
      'style' => 'text-align: right;',
    );
  }
  if ($node->tax2) {
    $rows[end(array_keys($rows))][] = array(
      'data' => sprintf('%.2f', $node->tax2),
      'style' => 'text-align: right;',
    );
  }
    $rows[end(array_keys($rows))][] = array(
      'data' => sprintf('%.2f', $node->total),
      'style' => 'text-align: right;',
    );

  $o .= '<div id="storminvoice_items">'. theme('table', $header, $rows) .'</div>';

  // Specific to Italian invoices
  if ($node->taxexempt && $language1=='it') {
    $o .= '<div id="storminvoice_tax_exempt">';
    $o .= t('Tax exempt art. 7', array(), $language);
    if ($language1) $o .= '<br />'. t('Tax exempt art. 7', array(), $language1);
    $o .= '</div>';
  }

  $o .= '<div id="storminvoice_payment_modes"><table>';
  $o .= '<tr><td class="storminvoice_payment_title">';
  $o .= t('Payment', array(), $language);
  if ($language1) $o .= '<br />'. t('Payment', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= variable_get('storminvoice_payment_modes', '');
  $o .= '</td></tr>';
  $o .= '</table></div>';

  if ($status=='paid') {
    $o .= '<div id="paid">';
    $o .= '<h2>'. t('Paid in full', array(), $language) .'</h2>';
    if ($language1) $o .= '<h2>'. t('Paid in full', array(), $language1) .'</h2>';
    $o .= '</div>';
  }

  return theme('storm_report', $header, $o, $title, $footer, $headtitle);
}

/**
 * @function
 * Code to create invoice auto_add link
 *
 * @return varchar
 *   html coding for the link
 */
function theme_storminvoice_autoadd_links($nid, $billable, $billed) {
  if (user_access('Storm invoice: add')) {
    if ($billable && !$billed) {
      $v = '';
      $v = '<strong>'. t('Invoice this item:') .'</strong>';
      $v .= '<ul>';
      $v .= '<li>'. l(t('Add to new invoice'), 'storm/invoice/auto_add/new/'. $nid) .'</li>';
      $v .= '<li>'. l(t('Add to existing invoice'), 'storm/invoice/auto_add/existing/'. $nid) .'</li>';
      $v .= '</ul>';
    }
    elseif ($billed) {
      $v = '';
      $v .= '<strong>'. t('Item already billed.') .'</strong>';
    }
    else {
      $v = '';
      $v .= '<strong>'. t('Item marked unbillable.') .'</strong>';
    }
  }
  return $v;
}
