<?php

/**
 * @file
 *  Pages callbacks.
 */

/**
 *
 */
function testshelper_main_page() {
  $output = '';

  $linklist = array(
    'item1' => array(
      'title' => 'Tests helper - Pages map',
      'href' => 'testshelper/pages-map'
    ),
    'item2' => array(
      'title' => 'RWD test',
      'href' => 'testshelper/rwd'
    ),
  );

  $output .= theme('links', array(
    'links' => $linklist,
    'attributes' => array('class' => array('links'))
  ));


  return $output;
}

/**
 * Page callback.
 */
function testshelper_pagesmap_page() {

  module_load_include('inc', 'testshelper', 'inc/DrupalPagesUrls.class');

  $drupal_pages = new DrupalPagesUrls();
  $drupal_pages->addNodes();
  $drupal_pages->addTaxonomyTerms();
  $drupal_pages->addUsers();
  $drupal_pages->addViews();
  $drupal_pages->addMenuLinks();

  $output = testshelper_redner_url_list($drupal_pages->getAll());

  return $output;
}

function testshelper_redner_url_list($urls_list) {


  $output = '<ul class="list testshelper">';
  foreach ($urls_list as $item) {
    $output .= '<li class="item">' . $item['label'] . ': ' . l($item['url'], $item['url']) . '</li>';
  }
  $output .= "</ul>";

  // Add link to open all items.
  $output .= '<h3 class="open-all-links"><a href="#">' . t('Open all links') . '</a></h3>';

  drupal_add_js(array('testshelper' => $urls_list), 'setting');
  drupal_add_js(drupal_get_path('module', 'testshelper') . '/js/testshelper.js');
  drupal_add_css(drupal_get_path('module', 'testshelper') . '/css/testshelper.css');

  return $output;
}