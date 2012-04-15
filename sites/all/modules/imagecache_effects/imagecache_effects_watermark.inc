<?php
function imagecache_effects_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'imagecache_ui_action_add_form' || $form_id == 'imagecache_ui_action_form') {
    if (isset($form['data']['image'])) {
      $form['#attributes']['enctype'] = 'multipart/form-data';
      $form['#validate'][] = 'imagecache_effects_watermark_form_validate';
    }
  }
  else if ($form_id == 'imagecache_ui_action_delete_form') {
    $action = imagecache_action_load($form['actionid']['#value']);
    if ($action['action'] == 'imagecache_effects_watermark') {
      $form['#submit'][] = 'imagecache_effects_watermark_delete';
    }
  }
  //drupal_set_message(var_export($form, true));
}

function imagecache_effects_watermark_form($data) {
  $form['image_fid'] = array(
    '#type' => 'hidden',
    '#value' => $data['image_fid'],
  );
  $form['image_preview_fid'] = array(
    '#type' => 'hidden',
    '#value' => $data['image_preview_fid'],
  );
  $form['image'] = array(
    '#type' => 'file',
    '#title' => t('Image'),
    '#description' => t('A valid image to be imposed on as a watermark. The image may contain transparency or an alpha channel if the image toolkit installed supports it.'),
    '#attributes' => array(
      'accept' => 'jpg|jpeg|gif|png',
    ),
    '#name' => 'files[image]'
  );
  $form['opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Opacity'),
    '#default_value' => isset($data['opacity']) ? $data['opacity'] : 100,
    '#description' => t('A value between 0 to 100.'),
    '#size' => 3,
    '#maxlength' => 3,
  );
  $form['x'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontal position'),
    '#default_value' => isset($data['x']) ? $data['x'] : 0,
    '#description' => t('Give a value in pixels. Negative values are referenced from the right. You may use the words <em>left, center & right</em> as substitutes for positions.'),
    '#size' => 6,
    '#maxlength' => 10,
  );
  $form['y'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical position'),
    '#default_value' => isset($data['y']) ? $data['y'] : 0,
    '#description' => t('Give a value in pixels. Negative values are referenced from the bottom. You may use the words <em>top, middle & bottom</em> as substitutes for positions.'),
    '#size' => 6,
    '#maxlength' => 10,
  );
  return $form;
}

function imagecache_effects_watermark_form_validate($form, &$form_state) {
  $edit = &$form_state['values']['data'];
  
  if (!is_numeric($edit['x']) && !empty($edit['x'])) {
    $s = strtolower(trim($edit['x']));
    if (!in_array($s, array('left', 'center', 'right'))) {
      form_set_error('x', t('Invalid horizontal position'));
    }
    else {
      $edit['x'] = $s;
    }
  }
  else {
    $edit['x'] = intval($edit['x']);
  }
  
  if (!is_numeric($edit['y']) && !empty($edit['y'])) {
    $s = strtolower(trim($edit['y']));
    if (!in_array($s, array('top', 'middle', 'bottom'))) {
      form_set_error('y', t('Invalid vertical position'));
    }
    else {
      $edit['y'] = $s;
    }
  }
  else {
    $edit['y'] = intval($edit['y']);
  }
  
  $edit['opacity'] = intval($edit['opacity']);
  if ($edit['opacity'] < 0 || $edit['opacity'] > 100) {
    form_set_error('opacity', t('Opacity value out of range'));
  }
  
  $validators = array(
    'file_validate_is_image' => array(),
    'file_validate_extensions' => array('jpg jpeg gif png'),
  );
  if ($file = file_save_upload('image', $validators, file_directory_path())) {
    if (!empty($edit['image_fid'])) {
      _imagecache_effects_file_delete($edit['image_fid']);
    }
    if (!empty($edit['image_preview_fid'])) {
      _imagecache_effects_file_delete($edit['image_preview_fid']);
    }
    
    file_set_status($file, FILE_STATUS_PERMANENT);
    $edit['image_fid'] = $file->fid;
  }
  
  if (empty($edit['image_fid'])) {
    form_set_error('image', t('You must upload an image'));
  }
}

function imagecache_effects_watermark_delete($form, &$form_state) {
  $action = imagecache_action_load($form['actionid']['#value']);
  _imagecache_effects_file_delete($action['data']['image_fid']);
}

function theme_imagecache_effects_watermark($element) {
  $data = $element['#value'];
  $file = _imagecache_effects_file_load($data['image_fid']);
  return t('Image: !image, Opacity: @opacity, Horizontal: @x, Vertical: @y', array(
    '!image' => l($file->filename, file_create_url($file->filename)),
    '@opacity' => $data['opacity'],
    '@x' => $data['x'],
    '@y' => $data['y'],
  ));
}

function imagecache_effects_watermark_image(&$image, $data) {
  if (($file = _imagecache_effects_file_load($data['image_fid'])) && ($marker = imageapi_image_open(file_create_path($file->filename)))) {
    module_load_include('inc', 'imagecache_effects', 'imageapi');
    return imageapi_image_watermark($image, $marker, $data);
  }
  return false;
}

function _imagecache_effects_file_load($fid) {
  return db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $fid));
}

function _imagecache_effects_file_delete($fid) {
  $file = new StdClass();
  $file->fid = $fid;
  return file_set_status($file, FILE_STATUS_TEMPORARY);
}