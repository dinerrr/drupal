<?php
/**
 * @file
 * application.features.inc
 */

/**
 * Implements hook_node_info().
 */
function application_node_info() {
  $items = array(
    'product' => array(
      'name' => t('Product'),
      'base' => 'node_content',
      'description' => t('A third party product'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'testowy_c_ontent_type' => array(
      'name' => t('testowy c ontent type'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
