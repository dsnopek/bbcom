<?php
// $Id: views-view-field.tpl.php,v 1.1 2008/05/16 22:22:32 merlinofchaos Exp $
 /**
  * This template is used to print a single field in a view. It is not
  * actually used in default Views, as this is registered as a theme
  * function which has better performance. For single overrides, the
  * template is perfectly okay.
  *
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, this construct should be used:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified.
  */

global $language;

if (!empty($row->{$view->field['nid']->field_alias})) {
  // edit the fully existing node (might be source, might be translation)
  print l($row->{$view->field['language']->field_alias} == $language->language ?
    t('edit source') : t('edit translation'),
    'node/'. $row->{$view->field['nid']->field_alias} .'/edit'
  );
}
elseif (!empty($row->{$view->field['nid_1']->field_alias})) {
  if ($row->{$view->field['language']->field_alias} == $language->language) {
    // edit the source when we are using the language of the korpus (translation is not appropriate)
    print l(t('edit source'), 'node/'. $row->{$view->field['nid_1']->field_alias} .'/edit');
  }
  else {
    // add the translation when we've got a node, but no translation
    print l(t('add translation'), 'node/add/entry', array('query' => array(
      'language'    => $language->language,
      'pos'         => $row->{$view->field['pos']->field_alias},
      'headword'    => $row->{$view->field['headword']->field_alias},
      'translation' => $row->{$view->field['nid_1']->field_alias},
    )));
  }
}
else {
  // if we've got nothing, then add the source
  print l(t('add source'), 'node/add/entry', array('query' => array(
    'language' => $row->{$view->field['language']->field_alias},
    'pos'      => $row->{$view->field['pos']->field_alias},
    'headword' => $row->{$view->field['headword']->field_alias},
  )));
}

