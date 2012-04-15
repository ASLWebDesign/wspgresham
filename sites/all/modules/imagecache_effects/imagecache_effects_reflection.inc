<?php
function imagecache_effects_reflection_form($data) {
  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => isset($data['color']) ? $data['color'] : 'white',
    '#description' => t('A hexadecimal color code preceded by a # or the name of the color can be used. eg. <em>white, blue, green, black, etc</em>'),
    '#required' => true,
    '#size' => 10,
    '#maxlength' => 20,
  );
  $form['position'] = array(
    '#type' => 'radios',
    '#title' => t('Position'),
    '#default_value' => isset($data['position']) ? $data['position'] : 'bottom',
    '#options' => array(
      'top' => t('Top'),
      'right' => t('Right'),
      'bottom' => t('Bottom'),
      'left' => t('Left'),
    ),
    '#description' => t('The position of the image reflection. Default is bottom.'),
    '#required' => true,
  );
  $form['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Size'),
    '#default_value' => isset($data['size']) ? $data['size'] : 30,
    '#description' => t('The size of the reflection in pixels. You may append % to the integer to represent percentages.'),
    '#required' => true,
  );
  return $form;
}

function theme_imagecache_effects_reflection($element) {
  $data = $element['#value'];
  return t('Background Color: !color, Position: !position, Size: @size', array(
    '!color' => is_numeric($data['color']) ? intval($data['color']) : theme('placeholder', check_plain($data['color'])),
    '!position' => is_numeric($data['position']) ? intval($data['position']) : theme('placeholder', check_plain($data['position'])),
    '@size' => $data['size'],
  ));
}

function imagecache_effects_reflection_image(&$image, $data) {
  module_load_include('inc', 'imagecache_effects', 'imageapi');
  return imageapi_image_reflection($image, $data);
}