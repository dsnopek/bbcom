<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function bbcom_dictionary_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_sources
  $groups['entry-group_sources'] = array(
    'group_type' => 'multigroup',
    'type_name' => 'entry',
    'group_name' => 'group_sources',
    'label' => 'Sources',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 1,
        ),
        'full' => array(
          'format' => 'fieldset_collapsed',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
        ),
        'weight' => 6,
        'label' => 'above',
        'description' => '',
      ),
      'multigroup' => array(
        'multiple' => 1,
        'required' => FALSE,
        'subgroup' => array(
          'weight' => -20,
          'label' => 'above',
          'teaser' => array(
            'format' => 'fieldset',
            'exclude' => 0,
          ),
          'full' => array(
            'format' => 'simple',
            'exclude' => 0,
          ),
        ),
      ),
    ),
    'weight' => '9',
    'parent' => '',
    'fields' => array(
      '0' => 'field_source_name',
      '1' => 'field_source_url',
      '2' => 'field_source_copyright',
      '3' => 'field_source_license',
      '4' => 'field_source_raw',
      '5' => 'field_source_timestamp',
    ),
    'depth' => 0,
    'parents' => array(
      '0' => '',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Sources');

  return $groups;
}
