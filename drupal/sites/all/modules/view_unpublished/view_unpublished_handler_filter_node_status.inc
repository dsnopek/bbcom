<?php

/**
 * Provides integration with the Views module.
 *
 * Filter by view all unpublished permissions granted by view_unpublished
 * Takes over the Published or Admin filter query.
 */

class view_unpublished_handler_filter_node_status extends views_handler_filter_node_status {
  function query() {
    $table = $this->ensure_my_table();
    $where_per_type = array('0');
    foreach (node_get_types() as $type => $name) {
      $where_per_type[] = "($table.type = '$type' AND ***VIEWUNPUBLISHED_$type*** = 1)";
    }
    $where_per_type = implode(' OR ', $where_per_type);
    $this->query->add_where($this->options['group'], "$table.status <> 0 OR ($table.uid = ***CURRENT_USER*** AND ***CURRENT_USER*** <> 0) OR ***ADMINISTER_NODES*** = 1 OR ***VIEWALLUNPUBLISHED_NODES*** = 1 OR $where_per_type" );
  }
}
