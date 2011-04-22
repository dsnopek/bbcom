
<?php if($widget): ?>
<div style="display: none;">
  <?php
    # We want these to continue to recalculate and to be sent to the server,
    # we just don't want to see them on the form
    print drupal_render($fields['soft']);
  ?>
</div>
<?php endif; ?>

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
      <td width="24%" colspan="2">Singular Forms</td>
      <td width="24%">Masculine</td>
      <td width="24%">Feminine</td>
      <td width="24%">Neuter</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td colspan="2" class="tcol-label"><?php print t('Nominative Singular') ?></td>
      <td><?php print bbcom_render_no_title($fields['nominative.singular.masculine']) ?>
      <td><?php print bbcom_render_no_title($fields['nominative.singular.feminine']) ?>
      <td><?php print bbcom_render_no_title($fields['nominative.singular.neuter']) ?>
    </tr>
    <tr>
      <td colspan="2" class="tcol-label"><?php print t('Genitive Singular') ?></td>
      <td><?php print bbcom_render_no_title($fields['genitive.singular.masculine']) ?>
      <td><?php print bbcom_render_no_title($fields['genitive.singular.feminine']) ?>
      <td><?php print bbcom_render_no_title($fields['genitive.singular.neuter']) ?>
    </tr>
    <tr>
      <td rowspan="2" class="tcol-label"><?php print t('Accusative Singular') ?></td>
      <td class="tcol-label"><?php print t('Animate') ?></td>
      <td><?php print bbcom_render_no_title($fields['accusative.singular.masculine.animate']) ?>
      <td rowspan="2"><?php print bbcom_render_no_title($fields['accusative.singular.feminine']) ?>
      <td rowspan="2"><?php print bbcom_render_no_title($fields['accusative.singular.neuter']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Inanimate') ?></td>
      <td><?php print bbcom_render_no_title($fields['accusative.singular.masculine.inanimate']) ?>
    </tr>
    <tr>
      <td colspan="2" class="tcol-label"><?php print t('Instrumental Singular') ?></td>
      <td><?php print bbcom_render_no_title($fields['instrumental.singular.masculine']) ?>
      <td><?php print bbcom_render_no_title($fields['instrumental.singular.feminine']) ?>
      <td><?php print bbcom_render_no_title($fields['instrumental.singular.neuter']) ?>
    </tr>
    <tr>
      <td colspan="2" class="tcol-label"><?php print t('Locative Singular') ?></td>
      <td><?php print bbcom_render_no_title($fields['locative.singular.masculine']) ?>
      <td><?php print bbcom_render_no_title($fields['locative.singular.feminine']) ?>
      <td><?php print bbcom_render_no_title($fields['locative.singular.neuter']) ?>
    </tr>
    <tr>
      <td colspan="2" class="tcol-label"><?php print t('Dative Singular') ?></td>
      <td><?php print bbcom_render_no_title($fields['dative.singular.masculine']) ?>
      <td><?php print bbcom_render_no_title($fields['dative.singular.feminine']) ?>
      <td><?php print bbcom_render_no_title($fields['dative.singular.neuter']) ?>
    </tr>
  </tbody>
</table>

<table width="100%" class="lingwo-fields-table">
  <thead>
    <tr>
      <td width="24%">Plural Forms</td>
      <td width="24%">Non-virile (women, animals, things, etc)</td>
      <td width="24%">Virile (men)</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td class="tcol-label"><?php print t('Nominative Plural') ?></td>
      <td><?php print bbcom_render_no_title($fields['nominative.plural.non_virile']) ?>
      <td><?php print bbcom_render_no_title($fields['nominative.plural.virile']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Genitive Plural') ?></td>
      <td colspan="2"><?php print bbcom_render_no_title($fields['genitive.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Accusative Plural') ?></td>
      <td><?php print bbcom_render_no_title($fields['accusative.plural.non_virile']) ?>
      <td><?php print bbcom_render_no_title($fields['accusative.plural.virile']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Instrumental Plural') ?></td>
      <td colspan="2"><?php print bbcom_render_no_title($fields['instrumental.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Locative Plural') ?></td>
      <td colspan="2"><?php print bbcom_render_no_title($fields['locative.plural']) ?>
    </tr>
    <tr>
      <td class="tcol-label"><?php print t('Dative Plural') ?></td>
      <td colspan="2"><?php print bbcom_render_no_title($fields['dative.plural']) ?>
    </tr>
  </tbody>
</table>

<?php
  if ($widget) {
    print drupal_render($fields);
  }
?>

