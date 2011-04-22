<?php
// $Id: nodecomment_handler_relationship_nid_or_self.inc,v 1.1.2.1 2009/06/04 21:14:55 merlinofchaos Exp $

/**
 * Field handler to display the depth of a comment
 */
class nodecomment_handler_relationship_nid_or_self extends views_handler_relationship {
  function option_definition() {
    $options = parent::option_definition();
    unset($options['required']);
    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['required']);
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);
    $base_field = empty($this->definition['base field']) ? $table_data['table']['base']['field'] : $this->definition['base field'];

    $this->ensure_my_table();

    // We need to figure out the alias of our base table based upon our relationship:
    if ($this->relationship) {
      $base_table = $this->relationship;
    }
    else {
      $base_table = $this->query->base_table;
    }

    $def = $this->definition;
    $def['table'] = $this->definition['base'];
    $def['field'] = $base_field;
    $def['left_table'] = '';
    $def['left_field'] = "COALESCE($this->table.$this->real_field, $base_table.$this->real_field)";
    $def['type'] = 'INNER';

    $join = new views_join();

    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }

}
