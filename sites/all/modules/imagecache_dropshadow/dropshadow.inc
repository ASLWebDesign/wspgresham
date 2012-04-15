<?php

/**
 * 
 */
function dropshadow_image(&$image, $action = array()) {

  _dropshadow_merge_default_actions($action);

  // Get the path of the images folder
  $path = $action['path'];
  $shadow_images = array('top_left', 'top', 'top_right', 'left', 'right', 'bottom_left', 'bottom', 'bottom_right');

  if (!is_dir($path)) {
    drupal_set_message(t("The folder containing the shadow images (!path) could not be determined!", array('!path' => $path)), 'error');
    return FALSE;
  }
  else {
    $errors = FALSE;
    foreach($shadow_images as $name) {
      if (!file_exists("$path/$name.png")) {
        drupal_set_message(t("!image could does not exists!", array('!image' => "$path/$name.png")), 'error');
        $errors = TRUE;
      }
    }
    if ($errors) {
      return FALSE;
    }
  }

  // Check compatibility
  if ($image->toolkit != 'imageapi_gd') {
    drupal_set_message("Unable to define canvas with {$image->toolkit}");
    return FALSE;
  }

  // Below I'm storing all 8 dropshadow images into memory.  
  foreach($shadow_images as $name) {
    ${$name} = imagecreatefrompng("$path/$name.png");
  }

  // We now have stored all images that we are going to use. 
  // Next we have to create one more (the canvas) to put them together. 
  // Remember, that the canvas has to be larger than the image, otherwise the image will hide the shadow images.
   
  // First need to store the dimensions of each shadow image
  foreach($shadow_images as $name) {
    ${"width_". $name} = imagesx(${$name});
    ${"height_". $name} = imagesy(${$name});
  }

  // Width of Canvas = Width of Image + width of left shadow + width of right shadow
  $adj_height = $height_top + $height_bottom;
  $adj_width = $width_left + $width_right;

  // Next we get our new heights and widths for the canvas. This creates an image that is slightly larger than 
  // your image.
  $height_canvas = $image->info['height'] + $adj_height;
  $width_canvas  = $image->info['width'] + $adj_width;
   
  // Third we create a blank canvas with these new dimensions
  $canvas = imagecreatetruecolor($width_canvas, $height_canvas);
  $rgb = imageapi_hex2rgba($action['background_color']);

  $background = imagecolorallocate($canvas, $rgb[0], $rgb[1], $rgb[2]);

  imagefilledrectangle($canvas, 0, 0, $height_canvas, $height_canvas, $background);

  // We have our graphic image, shadow images and canvas ready. Now it's time to paint! 
  // In the first section we're adding the top, left, bottom and then right sections. 

  imagecopyresized($canvas, $top, $width_left, 0, 0, 0, $width_canvas - $adj_width, $height_top, 1, $height_top);
  imagecopyresized($canvas, $left, 0, $height_top, 0, 0, $width_left, $height_canvas - $adj_height, $width_left, 1);
  imagecopyresized($canvas, $right, $width_canvas - $width_right, $height_top, 0, 0, $width_right, $height_canvas - $adj_height, $width_right, 1);
  imagecopyresized($canvas, $bottom, $width_left, $height_canvas - $height_bottom, 0, 0, $width_canvas - $adj_width, $height_bottom, 1, $height_bottom);

  imagecopyresized($canvas, $top_left, 0, 0, 0, 0, $width_left, $height_top, $width_left, $height_top);
  imagecopyresized($canvas, $top_right, $width_canvas - $width_right, 0, 0, 0, $width_right, $height_top, $width_right, $height_top);
  imagecopyresized($canvas, $bottom_left, 0, $height_canvas - $height_bottom, 0, 0, $width_left, $height_bottom, $width_left, $height_bottom);
  imagecopyresized($canvas, $bottom_right, $width_canvas - $width_right, $height_canvas - $height_bottom, 0, 0, $width_right, $height_bottom, $width_right, $height_bottom);

  // At this point the canvas has all the drop shadow images attached, all we need to do now is attach the image and we're done! 
  // I changed $w back to the size of the perimeter in order to properly place the image.
  $w = imagesx($left);
  imagecopyresampled($canvas, $image->resource, $w,$w,0,0,  imagesx($image->resource), imagesy($image->resource), imagesx($image->resource),imagesy($image->resource));
 
  // Notice that I used imagecopyresampled, the reason for this is I get a much higher quality image than with imagecopy
  // Resized. However, I sacrifice speed for this quality. 
  // ----------------------------------------
  // OUTPUTTING THE IMAGE
  // ----------------------------------------
  $image->resource = $canvas;
  return TRUE;
}

function dropshadow_form($action) {
  _dropshadow_merge_default_actions($action);

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Shadow images folder'),
    '#description' => t('Specify in which folder, relative to Drupal\'s root directory, the shadow images are stored. If you\'re not sure, use: %path', array('%path' => $defaults['path'])),
    '#default_value' => $action['path'],
  );
  $form['background_color'] = array(
    '#type' => (module_exists('colorpicker') ? 'colorpicker_textfield' : 'textfield'),
    '#title' => t('Background color'),
    '#description' => t('The canvas\' background color'),
    '#default_value' => $action['background_color'],
  );
  $form['#submit'][] = 'dropshadow_form_submit';
  return $form;
}


/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_dropshadow($element) {
  foreach ($element['#value'] as $key => $value) {
    $str[] = str_replace('_', ' ', $key) .": !$key";
    $replacements["!$key"] =  $value;
  }
  return t(join(", ", $str), $replacements);
}

/**
 * Helper function for setting default values
 */
function _dropshadow_merge_default_actions(&$action) {
  $defaults = array(
    'path' => drupal_get_path('module', 'imagecache_dropshadow') .'/images',
    'background_color' => '#ffffff', 
  );
  $action = array_merge($defaults, (array)$action);
}
