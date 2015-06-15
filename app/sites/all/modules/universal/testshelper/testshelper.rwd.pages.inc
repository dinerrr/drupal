<?php

/**
 * Page callback.
 */
function testshelper_rwd_page() {
  $output = '';
  $screen_sizes = _testshelper_get_screen_sizes(); // @TODO - form to change resolutions.

  $max_width = _testshelper_calculate_width($screen_sizes, $margin = 25);

  $url = url('<front>'); // @TODO - form to change url.

  $output .= '<div style="width:' . $max_width . '; clear: both; float: left; ">';
  foreach ($screen_sizes as $size) {
    $output .= _testshelper_iframe_generate($url, $size['width'], $size['height']);
  }

  $output .= '<div >';

  print $output;
  drupal_exit();
}

/**
 * @param $url
 * @param $width
 * @param $height
 * @return string
 */
function _testshelper_iframe_generate($url, $width, $height) {
  $output = '<div class="iframe-item" style="float:left; margin-right: 15px;" >';

  $output .= '<h2>' . $width . ' x ' . $height . '</h2>';

  $output .= '<iframe style="border: 3px solid #cccccc" width="' . $width . '" height="' . $height . '" src="' . $url . '"></iframe>';

  $output .= '</div>';
  return $output;
}

/**
 * Get resolutions.
 *
 * @return array
 */
function _testshelper_get_screen_sizes() {
  $ret_default = array(
    'mobile1' => array(
      'width' => 320,
      'height' => 480,
    ),
    'mobile2' => array(
      'width' => 360,
      'height' => 480,
    ),
    'mobile3' => array(
      'width' => 480,
      'height' => 800,
    ),
    'tablet1' => array(
      'width' => 768,
      'height' => 1024,
    ),
    'tablet2' => array(
      'width' => 1024,
      'height' => 768,
    ),
    'desktop1' => array(
      'width' => 960,
      'height' => 768,
    ),
    'desktop2' => array(
      'width' => 1200,
      'height' => 768,
    ),

  );
  return variable_get('testshelper_screen_sizes', $ret_default);
}

/**
 * Calculate width.
 *
 * @param $screen_sizes
 * @param $margin
 * @return int
 */
function _testshelper_calculate_width($screen_sizes, $margin) {
  $width = 0;

  foreach ($screen_sizes as $size) {
    $width += ($size['width'] + $margin);
  }

  return $width;
}