<?php

define("DRUPALPAGESURL_NODES_COUNT", 1);
define("DRUPALPAGESURL_USERS_COUNT", 6);
define("DRUPALPAGESURL_TAXONOMY_COUNT", 2);

//@TODO: users, taxonomy.

class DrupalPagesUrls {
  private $urlsList;
  private $nodesCount;
  private $usersCount;
  private $taxonomyCount;
  private $excludedMenus;

  function __construct() {
    $this->addFrontPage();
    $this->setNodesCount(DRUPALPAGESURL_NODES_COUNT);
    $this->setUsersCount(DRUPALPAGESURL_USERS_COUNT);
    $this->setTaxonomyCount(DRUPALPAGESURL_TAXONOMY_COUNT);

    $this->excludedMenus = array('features', 'devel', 'management', 'user-menu', 'navigation');
  }

  /**
   * Set attribute.
   *
   * @param $count
   */
  public function setNodesCount($count) {
    $this->nodesCount = $count;
  }

  /**
   * Set attribute.
   * @param $count
   */
  public function setUsersCount($count) {
    $this->usersCount = $count;
  }

  /**
   * Set attribute.
   * @param $count
   */
  public function setTaxonomyCount($count) {
    $this->taxonomyCount = $count;
  }

  /**
   * Set attribute.
   * @param $excludedMenus
   */
  public function setExcludedMenus($excludedMenus) {
    $this->excludedMenus = $excludedMenus;
  }

  /**
   * Get list of all links.
   *
   * @return mixed
   */
  public function getAll() {
    return $this->urlsList;
  }



  /**
   * Add list of views pages.
   *
   */
  public function addViews() {
    $views = views_get_all_views();

    foreach ($views as $view_name => $view) {
      // $var['events']->display['page']->display_plugin
      $displays = $view->display;

      foreach ($displays as $display) {
        if ($display->display_plugin == 'page' && $display->handler->options['enabled'] && $display->handler->view->disabled == FALSE) {
          $this->addItem($display->display_options['path'], t('View @view', array('@view' => $view_name)));
        }
      }
    }
  }

  /**
   * Add all node types.
   */
  public function addNodes() {
    $nodeTypes = node_type_get_types();
    foreach ($nodeTypes as $type => $info) {
      $this->addNodeType($type);
    }
  }

  /**
   * Add nodes by type.
   *
   * @param $type
   */
  public function addNodeType($type) {
    // Get X nodes.
    $query = db_select('node', 'n');
    $query->fields('n', array('nid', 'title'));
    $query->condition('type', $type);
    $query->orderBy('RAND()');
    $query->range(0, $this->nodesCount);
    $result = $query->execute();

    foreach ($result as $row) {
      $this->addItem('node/' . $row->nid, t('Node type: @type', array('@type' => $type)));
    }
  }

  /**
   * Add all node types.
   */
  public function addTaxonomyTerms() {
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $vid => $info) {
      $this->addTaxonomyTermsType($vid);
    }
  }

  public function addTaxonomyTermsType($vid) {
    // Get X nodes.
    $query = db_select('taxonomy_term_data', 't');
    $query->fields('t', array('tid', 'name', 'vid'));
    $query->condition('vid', $vid);
    $query->orderBy('RAND()');
    $query->range(0, $this->taxonomyCount);
    $result = $query->execute();

    foreach ($result as $row) {
      $vocab = taxonomy_vocabulary_load($row->vid);
      $vocab_name = $vocab->name;

      $this->addItem('taxonomy/term/' . $row->tid, t('Taxonomy [@type] term name: @name ', array('@type' => $vocab_name, '@name' => $row->name)));
    }
  }

  /**
   * Add random users.
   *
   * @TODO - per role.
   *
   */
  public function addUsers() {
    // Get X nodes.
    $query = db_select('users', 'u');
    $query->fields('u', array('uid', 'name'));
    //$query->condition('type', $type);
    $query->orderBy('RAND()');
    $query->range(0, $this->usersCount);
    $result = $query->execute();

    foreach ($result as $row) {
      $this->addItem('user/' . $row->uid, t('User: @name', array('@name' => $row->name)));
    }
  }

  /**
   * Add front page url.
   */
  public function addFrontPage() {
    $this->addItem(variable_get('site_frontpage'), t('Front page'));
  }

  /**
   * Add one item to list.
   *
   * @param $url
   * @param $label
   * @param array $query
   */
  private function addItem($url, $label, $query = array()) {
    if (!$this->checkIfExist($url)) {
      $this->urlsList[] = array(
        'label' => $label,
        'url' => $url,
        'url_absolute' => url($url, array('absolute' => TRUE)),
        'query' => $query,
      );
    }
  }

  /**
   * @param $url
   * @return bool
   */
  private function checkIfExist($url) {
    if ($this->urlsList) {
      foreach ($this->urlsList as $items) {
        if ($items['url'] == $url) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Add all links from custom menus.
   */
  public function addMenuLinks() {
    $query = db_select('menu_custom', 'm');
    $query->fields('m');
    $query->condition('menu_name', $this->excludedMenus, 'NOT IN');
    $result = $query->execute();

    foreach ($result as $row) {
      $this->addMenuLinksType($row->menu_name);
    }
  }

  /**
   * Add links form menu $menu_name.
   *
   * @param $menu_name
   */
  public function addMenuLinksType($menu_name) {
    $query = db_select('menu_links', 'm');
    $query->fields('m');
    $query->condition('menu_name', $menu_name);
    $result = $query->execute();

    foreach ($result as $row) {
      $this->addItem($row->link_path, t('Menu [@menu]: @title', array('@menu' => $menu_name, '@title' => $row->link_title)));
    }
  }


}