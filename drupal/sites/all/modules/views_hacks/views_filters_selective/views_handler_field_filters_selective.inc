<?php
// $Id

class views_handler_field_filters_selective extends views_handler_field {
  function query() {
    foreach ($this->view->display_handler->handlers['filter'] as $name => $filter) {
      if ($filter->options['exposed'] && $filter->options['expose']['vfs_selective']) {
        $this->additional_fields['selective_' . $filter->options['expose']['identifier']] = array(
          'table' => $filter->table,
          'field' => $filter->real_field,
        );
      }
    }
    $this->add_additional_fields();
  }
}

