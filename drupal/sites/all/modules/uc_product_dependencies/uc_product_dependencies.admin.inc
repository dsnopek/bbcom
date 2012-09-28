<?php
// $Id: uc_product_dependencies.admin.inc,v 1.1.2.1 2010/04/08 22:09:44 joelstein Exp $

/**
 * Configure module settings.
 */
function uc_product_dependencies_settings_form(&$form_state) {
  drupal_add_js(drupal_get_path('module', 'uc_product_dependencies') . '/uc_product_dependencies.js');
  
  // Node Reference field for each content type which Ubercart considers a product
  foreach (uc_product_types() as $type) {
    
    // Get list of Node Reference fields for this content type
    $node_reference_fields = array(NULL => t('<none>'));
    $results = db_query("SELECT field.field_name
      FROM {content_node_field} field
      LEFT JOIN {content_node_field_instance} instance ON field.field_name = instance.field_name
      WHERE field.type = 'nodereference' AND instance.type_name = '%s'
      ORDER BY field.field_name", $type);
    while ($row = db_fetch_array($results)) {
      $node_reference_fields[$row['field_name']] = $row['field_name'];
    }
    
    $type_name = node_get_types('name', array('type' => $type));
    $form['nodereferences']["uc_product_dependencies_{$type}_nodereference"] = array(
      '#type' => 'select',
      '#title' => t('Node Reference Field - !type', array('!type' => $type_name)),
      '#description' => t('Select the field containing the !type dependencies, or <a href="@link">add a Node Reference field</a>.', array(
        '!type' => $type_name,
        '@link' => url("admin/content/node-type/{$type}/fields"),
      )),
      '#options' => $node_reference_fields,
      '#default_value' => variable_get("uc_product_dependencies_{$type}_nodereference", ''),
    );
  }
  
  // How to handle multiple dependencies
  $form['uc_product_dependencies_multiple'] = array(
    '#type' => 'radios',
    '#title' => t('Multiple Dependencies'),
    '#options' => array(
      UC_PRODUCT_DEPENDENCIES_OR => t('OR - Product may be purchased if either dependency resolves'),
      UC_PRODUCT_DEPENDENCIES_AND => t('AND - Product may be purchased if both dependencies resolve'),
    ),
    '#description' => t('How should the dependencies be enforced if the Node Reference field can accept multiple values?'),
    '#default_value' => variable_get('uc_product_dependencies_multiple', UC_PRODUCT_DEPENDENCIES_OR),
  );
  
  // Location of dependency (cart, or previous order)
  $form['uc_product_dependencies_location'] = array(
    '#type' => 'radios',
    '#title' => t('Location of Products Dependencies'),
    '#options' => array(
      UC_PRODUCT_DEPENDENCIES_CART => t('Dependency must be in cart'),
      UC_PRODUCT_DEPENDENCIES_CART_OR_ORDER => t('Dependency must be in cart or have been purchased in a previous order'),
      UC_PRODUCT_DEPENDENCIES_ORDER => t('Dependency must have been purchased in a previous order'),
    ),
    '#description' => t('Where should the product dependency be? (Previous orders can only be determined for authenticated users.)'),
    '#default_value' => variable_get('uc_product_dependencies_location', UC_PRODUCT_DEPENDENCIES_CART_OR_ORDER),
  );
  
  // Order status (for previous orders only)
  $order_statuses = array();
  $results = db_query("SELECT order_status_id, title FROM {uc_order_statuses} ORDER BY weight");
  while ($row = db_fetch_array($results)) {
    $order_statuses[$row['order_status_id']] = $row['title'];
  }
  $form['uc_product_dependencies_order_status'] = array(
    '#type' => 'select',
    '#title' => t('Previous Purchase Order Status'),
    '#options' => $order_statuses,
    '#default_value' => variable_get('uc_product_dependencies_order_status', 'payment_received'),
    '#description' => t('Select the order status used to determine whether or not a product was purchased.'),
  );
  
  return system_settings_form($form);
}
