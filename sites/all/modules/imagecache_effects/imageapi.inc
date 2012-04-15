<?php
function imageapi_color($c) {
  if (is_string($c)) {
    if (substr($c, 0, 1) == '#') {
      $c = substr($c, 1);
      if (strlen($c) <= 3) {
        $t = '';
        foreach ($c as $s) {
          $t .= $s . $s;
        }
        $c = $t;
      }
      else if (strlen($c) > 6) {
        $c = substr($c, 0, 6);
      }
      $c = str_pad($c, 6, '0');
      
      return hexdec($c);
    }
    else {
      $c = strtolower(trim($c));
      foreach (_imageapi_colors() as $name => $value) {
        if ($c == strtolower($name)) {
          return $value;
        }
      }
    }
  }
  return 0x00000;
}

function imageapi_image_watermark($image, $marker, $options = array()) {
  if ($image->toolkit != $marker->toolkit) {
    watchdog('imageapi', 'Image Water Mark cannot use two different images loaded from two different toolkits.');
    return false;
  }
  
  $options = array_merge(array(
    'x' => 0,
    'y' => 0,
    'opacity' => 100,
  ), $options);
  
  if (!is_numeric($options['x'])) {
    switch (strtolower($options['x'])) {
      case 'center':
        $options['x'] = ceil(($image->info['width'] / 2) - ($marker->info['width'] / 2));
        break;
      case 'right':
        $options['x'] = $image->info['width'] - $marker->info['width'];
        break;
      default:
        $options['x'] = 0;
    }
  }
  
  if (!is_numeric($options['y'])) {
    switch (strtolower($options['y'])) {
      case 'middle':
        $options['y'] = ceil(($image->info['height'] / 2) - ($marker->info['height'] / 2));
        break;
      case 'bottom':
        $options['y'] = $image->info['height'] - $marker->info['height'];
        break;
      default:
        $options['y'] = 0;
    }
  }
  
  $options['opacity'] = intval($options['opacity']);
  $options['opacity'] = $options['opacity'] > 0 ? min($options['opacity'], 100) : 0;
  
  $function = $image->toolkit .'_image_watermark';
  if (function_exists($function)) {
    return call_user_func($function, $image, $marker, $options);
  }
  
  watchdog('imageapi', 'No Water Mark function for image toolkit %toolkit', array('%toolkit' => $image->toolkit));
  return false;
}

function imageapi_gd_image_watermark($image, $marker, $options = array()) {
  // because of a bug? in gd with png images, we have to use another function
  if ($marker->info['mime_type'] == 'image/png' && $options['opacity'] == 100) {
    imagealphablending($image->resource, true);
    imagealphablending($marker->resource, true);
    return imagecopy($image->resource, $marker->resource, $options['x'], $options['y'], 0, 0, $marker->info['width'], $marker->info['height']);
  }
  else {
    return imagecopymerge($image->resource, $marker->resource, $options['x'], $options['y'], 0, 0, $marker->info['width'], $marker->info['height'], $options['opacity']);
  }
}

/** @todo: add watermark support for imagemagick **/

function imageapi_image_reflection($image, $options = array()) {
  $options = array_merge(array(
    'color' => 'white',
    'position' => 'bottom',
    'size' => 30,
  ), $options);
  
  $is_vertical = in_array($options['position'], array('top', 'bottom'));
  
  if (preg_match('/^\d\%$/', $options['size'])) {
    $image_size = $is_vertical ? $image->info['height'] : $image->info['width'];
    $options['size'] = $image_size * (floatval($options['size']) / 100);
  }
  else {
    $options['size'] = intval($options['size']);
  }
  
  $options['color'] = imageapi_color($options['color']);
  
  $function = $image->toolkit .'_image_reflection';
  if (function_exists($function)) {
    return call_user_func($function, $image, $options);
  }
  
  watchdog('imageapi', 'No Reflection function for image toolkit %toolkit', array('%toolkit' => $image->toolkit));
  return false;
}

function imageapi_gd_image_reflection(&$image, $options = array()) {
  $is_vertical = in_array($options['position'], array('top', 'bottom'));
  
  $width = $image->info['width'] + ($is_vertical ? 0 : $options['size']);
  $height = $image->info['height'] + ($is_vertical ? $options['size'] : 0);
  
  $x = $options['position'] == 'left' ? $options['size'] : 0;
  $y = $options['position'] == 'top' ? $options['size'] : 0;
  
  $background = imagecreatetruecolor($width, $height);
  imagefill($background, 0, 0, $options['color']);
  imagecopy($background, $image->resource, $x, $y, 0, 0, $image->info['width'], $image->info['height']);
  
  $return = false;
  switch ($options['position']) {
    case 'top':
      $steps = min($options['size'], $image->info['height']);
      for ($i = 0, $opacity = 50; $i < $steps; ++$i, $opacity = ceil(((($steps - $i) / $steps) * 100) / 2)) {
        imagecopymerge($background, $image->resource, 0, $y - $i, 0, $i, $image->info['width'], 1, $opacity);
      }
      $return = true;
      break;
    case 'bottom':
      $steps = min($options['size'], $image->info['height']);
      for ($i = 0, $opacity = 50; $i < $steps; ++$i, $opacity = ceil(((($steps - $i) / $steps) * 100) / 2)) {
        imagecopymerge($background, $image->resource, 0, $image->info['height'] + $i, 0, $image->info['height'] - $i, $image->info['width'], 1, $opacity);
      }
      $return = true;
      break;
    case 'left':
      $steps = min($options['size'], $image->info['width']);
      for ($i = 0, $opacity = 50; $i < $steps; ++$i, $opacity = ceil(((($steps - $i) / $steps) * 100) / 2)) {
        imagecopymerge($background, $image->resource, $x - $i, 0, $i, 0, 1, $image->info['height'], $opacity);
      }
      $return = true;
      break;
    case 'right':
      $steps = min($options['size'], $image->info['height']);
      for ($i = 0, $opacity = 50; $i < $steps; ++$i, $opacity = ceil(((($steps - $i) / $steps) * 100) / 2)) {
        imagecopymerge($background, $image->resource, $image->info['width'] + $i, 0, $image->info['width'] - $i, 0, 1, $image->info['height'], $opacity);
      }
      $return = true;
      break;
  }
  
  if ($return) {
    imagedestroy($image->resource);
    $image->resource = $background;
    $image->info['width'] = $width;
    $image->info['height'] = $height;
  }
  else {
    imagedestryoy($background);
  }
  return $return;
}

function _imageapi_colors() {
  static $colors = array(
    'AliceBlue' => 0xF0F8FF,
    'AntiqueWhite' => 0xFAEBD7,
    'Aqua' => 0x00FFFF,
    'Aquamarine' => 0x7FFFD4,
    'Azure' => 0xF0FFFF,
    'Beige' => 0xF5F5DC,
    'Bisque' => 0xFFE4C4,
    'Black' => 0x000000,
    'BlanchedAlmond' => 0xFFEBCD,
    'Blue' => 0x0000FF,
    'BlueViolet' => 0x8A2BE2,
    'Brown' => 0xA52A2A,
    'BurlyWood' => 0xDEB887,
    'CadetBlue' => 0x5F9EA0,
    'Chartreuse' => 0x7FFF00,
    'Chocolate' => 0xD2691E,
    'Coral' => 0xFF7F50,
    'CornflowerBlue' => 0x6495ED,
    'Cornsilk' => 0xFFF8DC,
    'Crimson' => 0xDC143C,
    'Cyan' => 0x00FFFF,
    'DarkBlue' => 0x00008B,
    'DarkCyan' => 0x008B8B,
    'DarkGoldenRod' => 0xB8860B,
    'DarkGray' => 0xA9A9A9,
    'DarkGrey' => 0xA9A9A9,
    'DarkGreen' => 0x006400,
    'DarkKhaki' => 0xBDB76B,
    'DarkMagenta' => 0x8B008B,
    'DarkOliveGreen' => 0x556B2F,
    'Darkorange' => 0xFF8C00,
    'DarkOrchid' => 0x9932CC,
    'DarkRed' => 0x8B0000,
    'DarkSalmon' => 0xE9967A,
    'DarkSeaGreen' => 0x8FBC8F,
    'DarkSlateBlue' => 0x483D8B,
    'DarkSlateGray' => 0x2F4F4F,
    'DarkSlateGrey' => 0x2F4F4F,
    'DarkTurquoise' => 0x00CED1,
    'DarkViolet' => 0x9400D3,
    'DeepPink' => 0xFF1493,
    'DeepSkyBlue' => 0x00BFFF,
    'DimGray' => 0x696969,
    'DimGrey' => 0x696969,
    'DodgerBlue' => 0x1E90FF,
    'FireBrick' => 0xB22222,
    'FloralWhite' => 0xFFFAF0,
    'ForestGreen' => 0x228B22,
    'Fuchsia' => 0xFF00FF,
    'Gainsboro' => 0xDCDCDC,
    'GhostWhite' => 0xF8F8FF,
    'Gold' => 0xFFD700,
    'GoldenRod' => 0xDAA520,
    'Gray' => 0x808080,
    'Grey' => 0x808080,
    'Green' => 0x008000,
    'GreenYellow' => 0xADFF2F,
    'HoneyDew' => 0xF0FFF0,
    'HotPink' => 0xFF69B4,
    'IndianRed' => 0xCD5C5C,
    'Indigo' => 0x4B0082,
    'Ivory' => 0xFFFFF0,
    'Khaki' => 0xF0E68C,
    'Lavender' => 0xE6E6FA,
    'LavenderBlush' => 0xFFF0F5,
    'LawnGreen' => 0x7CFC00,
    'LemonChiffon' => 0xFFFACD,
    'LightBlue' => 0xADD8E6,
    'LightCoral' => 0xF08080,
    'LightCyan' => 0xE0FFFF,
    'LightGoldenRodYellow' => 0xFAFAD2,
    'LightGray' => 0xD3D3D3,
    'LightGrey' => 0xD3D3D3,
    'LightGreen' => 0x90EE90,
    'LightPink' => 0xFFB6C1,
    'LightSalmon' => 0xFFA07A,
    'LightSeaGreen' => 0x20B2AA,
    'LightSkyBlue' => 0x87CEFA,
    'LightSlateGray' => 0x778899,
    'LightSlateGrey' => 0x778899,
    'LightSteelBlue' => 0xB0C4DE,
    'LightYellow' => 0xFFFFE0,
    'Lime' => 0x00FF00,
    'LimeGreen' => 0x32CD32,
    'Linen' => 0xFAF0E6,
    'Magenta' => 0xFF00FF,
    'Maroon' => 0x800000,
    'MediumAquaMarine' => 0x66CDAA,
    'MediumBlue' => 0x0000CD,
    'MediumOrchid' => 0xBA55D3,
    'MediumPurple' => 0x9370D8,
    'MediumSeaGreen' => 0x3CB371,
    'MediumSlateBlue' => 0x7B68EE,
    'MediumSpringGreen' => 0x00FA9A,
    'MediumTurquoise' => 0x48D1CC,
    'MediumVioletRed' => 0xC71585,
    'MidnightBlue' => 0x191970,
    'MintCream' => 0xF5FFFA,
    'MistyRose' => 0xFFE4E1,
    'Moccasin' => 0xFFE4B5,
    'NavajoWhite' => 0xFFDEAD,
    'Navy' => 0x000080,
    'OldLace' => 0xFDF5E6,
    'Olive' => 0x808000,
    'OliveDrab' => 0x6B8E23,
    'Orange' => 0xFFA500,
    'OrangeRed' => 0xFF4500,
    'Orchid' => 0xDA70D6,
    'PaleGoldenRod' => 0xEEE8AA,
    'PaleGreen' => 0x98FB98,
    'PaleTurquoise' => 0xAFEEEE,
    'PaleVioletRed' => 0xD87093,
    'PapayaWhip' => 0xFFEFD5,
    'PeachPuff' => 0xFFDAB9,
    'Peru' => 0xCD853F,
    'Pink' => 0xFFC0CB,
    'Plum' => 0xDDA0DD,
    'PowderBlue' => 0xB0E0E6,
    'Purple' => 0x800080,
    'Red' => 0xFF0000,
    'RosyBrown' => 0xBC8F8F,
    'RoyalBlue' => 0x041690,
    'SaddleBrown' => 0x8B4513,
    'Salmon' => 0xFA8072,
    'SandyBrown' => 0xF4A460,
    'SeaGreen' => 0x2E8B57,
    'SeaShell' => 0xFFF5EE,
    'Sienna' => 0xA0522D,
    'Silver' => 0xC0C0C0,
    'SkyBlue' => 0x87CEEB,
    'SlateBlue' => 0x6A5ACD,
    'SlateGray' => 0x708090,
    'SlateGrey' => 0x708090,
    'Snow' => 0xFFFAFA,
    'SpringGreen' => 0x00FF7F,
    'SteelBlue' => 0x4682B4,
    'Tan' => 0xD2B48C,
    'Teal' => 0x008080,
    'Thistle' => 0xD8BFD8,
    'Tomato' => 0xFF6347,
    'Turquoise' => 0x40E0D0,
    'Violet' => 0xEE82EE,
    'Wheat' => 0xF5DEB3,
    'White' => 0xFFFFFF,
    'WhiteSmoke' => 0xF5F5F5,
    'Yellow' => 0xFFFF00,
    'YellowGreen' => 0x9ACD32,
  );
  return $colors;
}