<?php

/**
 * [audit_report_page description]
 * @return [type] [description]
 */
function audit_report_page() {
  $output = 'audit';

  $data = audit_get_data();

  foreach ($data as $item) {
    $rows[] = array($item['label'], $item['value']);
  }

  $header = array(
    array(
      "data" => t('Label'),
    ),
     array(
      "data" => t('Value'),
    ),
  );

  $output = theme_table(
    array(
      "header" => $header,
      "rows" => $rows,
      "attributes" => array("width"=>"100%"),
      "sticky" => true, // Table header will be sticky
      "caption" => "",
      "colgroups" => array(),
      "empty" => t("Table has no row!") // The message to be displayed if table is empty
    )
  );


  return $output;
}

/**
 * [audit_get_data description]
 * @return [type] [description]
 */
function audit_get_data() {
  $data = array();
  $data[] = audit_get_database_tables_count();
  $data[] = audit_get_database_size();
  $data[] = audit_get_node_types_count();
  $data[] = audit_get_fields_count();
  $data[] = audit_get_views_count();
  $data[] = audit_get_table_system_count();
  $data[] = audit_get_table_system_enabled_count();


  return $data;
}

/**
 * [audit_get_database_tables_count description]
 * @return [type] [description]
 */
function audit_get_database_tables_count() {
  global $databases;
  $db_name = $databases['default']['default']['database'];
  $result = db_query("SELECT COUNT(*) as count
    FROM information_schema.tables
    WHERE table_schema = :dbname;", array(':dbname' => $db_name))->fetchAssoc();

  $data = array(
    'label' => t('Database tables count'),
    'value' => $result['count'],
    );
  return $data;
}

/**
 * [audit_get_database_tables_count description]
 * @return [type] [description]
 */
function audit_get_database_size() {
  global $databases;
  $db_name = $databases['default']['default']['database'];
  $result = db_query("SELECT ROUND( SUM( data_length + index_length ) /1024 /1024, 1 ) as size
FROM information_schema.tables
WHERE table_schema =  :dbname;", array(':dbname' => $db_name))->fetchAssoc();

  $data = array(
    'label' => t('Database size'),
    'value' => $result['size'] . 'M',
    );
  return $data;
}

/**
 * [audit_get_database_tables_count description]
 * @return [type] [description]
 */
function audit_get_node_types_count() {
/*  global $databases;
  $db_name = $databases['default']['default']['database'];
  $result = db_query("SELECT COUNT(*) as count
    FROM node_type ")->fetchAssoc();*/

  $data = array(
    'label' => t('Node types'),
    'value' => count(node_type_get_types()),
    );
  return $data;
}

/**
 * [audit_get_fields_count description]
 * @return [type] [description]
 */
function audit_get_fields_count() {
  $fields = field_info_fields();
  $data = array(
    'label' => t('Fields count'),
    'value' => count($fields),
   );
  return $data;
}

function audit_get_views_count() {
  $views = views_get_all_views();
  $data = array(
    'label' => t('Views count'),
    'value' => count($views),
   );
  return $data;
}

function audit_get_table_system_count() {
  $result = db_query("SELECT COUNT(*) as count FROM system")->fetchAssoc();
  $data = array(
    'label' => t('Table system count'),
    'value' => $result['count'],
    );
  return $data;
}

function audit_get_table_system_enabled_count() {
  $result = db_query("SELECT COUNT(*) as count FROM system WHERE status = 1")->fetchAssoc();
  $data = array(
    'label' => t('Table system enabled count'),
    'value' => $result['count'],
    );
  return $data;
}


