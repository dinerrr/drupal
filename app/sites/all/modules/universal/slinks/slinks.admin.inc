<?php

/**
 * @file
 * Administration settings for SLinks.
 */

/**
 * Administration settings form.
 *
 * @return
 *   The completed form definition.
 */
function slinks_admin_settings() {
  $form = array();

  $form['slinks_general_settings'] = array(
    '#type'  => 'markup',
    '#title' => t('TODO :)'),
  );

  return system_settings_form($form);
}