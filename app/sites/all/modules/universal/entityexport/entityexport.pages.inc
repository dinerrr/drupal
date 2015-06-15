<?php

/**
 * Page callback.
 */
function entityexport_node_page($node) {
  $output = '';
  $output .= '<h2>'. t('Node object') . '</h2>';

  $file = DRUPAL_ROOT . '/includes/utility.inc';
  if (is_file($file)) {
    require_once $file;
  }
  $output .= '<pre>' . drupal_var_export($node) .'</pre>';

  return $output;
}
