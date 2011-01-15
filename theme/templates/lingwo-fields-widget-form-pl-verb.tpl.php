
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
      <td width="24%">Person</td>
      <td width="24%">Form</td>
      <td width="24%">Person</td>
      <td width="24%">Form</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td class="tcol-label" style="background-color: yellow;">
        ja <span style="font-weight: bold; color: red;">(<?php print t('key form') ?>)</span>
      </td>
      <td><?php print bbcom_render_no_title($fields['nonpast.singular.1p']) ?></td>
      <td class="tcol-label">my</td>
      <td><?php print bbcom_render_no_title($fields['nonpast.plural.1p']) ?></td>
    </tr>
    <tr>
      <td class="tcol-label">ty</td>
      <td><?php print bbcom_render_no_title($fields['nonpast.singular.2p']) ?></td>
      <td class="tcol-label">wy</td>
      <td><?php print bbcom_render_no_title($fields['nonpast.plural.2p']) ?></td>
    </tr>
    <tr>
      <td class="tcol-label" style="background-color: yellow;">
        on/ona/ono <span style="font-weight: bold; color: red;">(<?php print t('key form') ?>)</span>
      </td>
      <td><?php print bbcom_render_no_title($fields['nonpast.singular.3p']) ?></td>
      <td class="tcol-label" style="background-color: yellow;">
        oni/one <span style="font-weight: bold; color: red;">(<?php print t('key form') ?>)</span>
      </td>
      <td><?php print bbcom_render_no_title($fields['nonpast.plural.3p']) ?></td>
    </tr>
  </tbody>
</table>

<?php
  if ($widget) {
    print drupal_render($fields);
  }
?>

