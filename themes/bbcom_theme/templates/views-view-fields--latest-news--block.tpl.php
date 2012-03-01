<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<div class="clear-block">
  <div class="slide-video">
    <?php if (!empty($fields['field_emvideo_embed']->raw)): ?>
      <?php print $fields['field_emvideo_embed']->content ?>
    <?php else: ?>
      <div style="width: 425px; height: 290px; text-align: center; padding-top: 50px; background-color: LightBlue;">
        <img alt="BiblioBird logo" src="/logoLarge.png" />
      </div>
    <?php endif; ?>
  </div>
  <div class="slide-text">
    <div class="slide-text-title"><?php print $fields['title']->content ?></div>
    <div class="slide-text-date"><?php print $fields['created']->content ?></div>
    <div class="slide-text-body"><?php print $fields['teaser']->content ?></div>
    <?php print l(t('Read more'), 'node/'. $fields['nid']->content); ?>
  </div>
</div>

