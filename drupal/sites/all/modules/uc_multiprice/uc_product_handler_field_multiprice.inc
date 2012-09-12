<?php

/**
 * @file
 * Views handler: Product price field.
 */

/**
 * Return a formatted price value to display in the View.
 */
class uc_product_handler_field_multiprice extends views_handler_field {
  function render($values) {
    $multiprice = db_fetch_object(db_query("SELECT * FROM {uc_multiprice} WHERE nid = %d AND country_id = %d", $values->nid, uc_multiprice_country_id()));
    if ($multiprice) {
      if ($values->uc_products_sell_price)
        $values->uc_products_sell_price = check_plain($multiprice->sell_price);
      if ($values->uc_products_cost)
        $values->uc_products_cost = check_plain($multiprice->cost);
      if ($values->uc_products_list_price)
        $values->uc_products_list_price = check_plain($multiprice->list_price);
    }

    $context = array(
      'location' => 'views-price-handler',
      'class' => $this->field,
      'subject' => array(
      'node' => $values,
      'field' => $this->real_field,
      ),
    );
    $table_alias_len = drupal_strlen($this->table_alias);
    foreach ($values as $key => $value) {
      if (drupal_substr($key, 0, $table_alias_len) == $this->table_alias) {
        $values->{drupal_substr($key, $table_alias_len + 1)} = $value;
      }
    }
    
    return uc_price($values->{$this->field_alias}, $context);
  }
}
