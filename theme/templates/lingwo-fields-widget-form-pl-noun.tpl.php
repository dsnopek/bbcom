
<?php print drupal_render($fields['gender']); ?>
<?php print drupal_render($fields['virile']); ?>
<?php print drupal_render($fields['animate']); ?>

<?php print drupal_render($fields[':classes']); ?>
<?php print drupal_render($fields[':options']); ?>
<?php
  if ($widget) {
    print drupal_render($fields['*stem']);
  }
?>

<table width="100%" class="lingwo-fields-table">
  <thead>
    <tr>
      <td width="32%">Form</td>
      <td width="32%">Singular</td>
      <td width="32%">Plural</td>
    </tr>
  </thead>
  <tbody>
    <?php if ($widget): ?>
    <tr>
      <td class="tcol-label" style="background-color: yellow">*<?php print t('Stem') ?></td>
      <td style="background-color: yellow;"><?php print bbcom_render_no_title($fields['*stem.singular']) ?>
      <td style="background-color: yellow;"><?php print bbcom_render_no_title($fields['*stem.plural']) ?>
    </tr>
    <?php endif; ?>
    <tr>
      <td class="tcol-label"><?php print t('Nominative') ?></td>
      <td><?php print bbcom_render_no_title($fields['nominative.singular']) ?>
      <td><?php print bbcom_render_no_title($fields['nominative.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Genitive') ?></td>
      <td><?php print bbcom_render_no_title($fields['genitive.singular']) ?>
      <td><?php print bbcom_render_no_title($fields['genitive.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Accusative') ?></td>
      <td><?php print bbcom_render_no_title($fields['accusative.singular']) ?>
      <td><?php print bbcom_render_no_title($fields['accusative.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Instrumental') ?></td>
      <td><?php print bbcom_render_no_title($fields['instrumental.singular']) ?>
      <td><?php print bbcom_render_no_title($fields['instrumental.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Locative') ?></td>
      <td><?php print bbcom_render_no_title($fields['locative.singular']) ?>
      <td><?php print bbcom_render_no_title($fields['locative.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Dative') ?></td>
      <td><?php print bbcom_render_no_title($fields['dative.singular']) ?>
      <td><?php print bbcom_render_no_title($fields['dative.plural']) ?>
    </tr>
  </tbody>
</table>

<?php
  if ($widget) {
    print drupal_render($fields);
  }
?>

