<?php
// $Id: views-flipped-table.tpl.php,v 1.1.2.1 2010/05/22 13:50:26 kratib Exp $
/**
 * @file views-flipped-table.tpl.php
 * Template to display a view as a table with rows and columns flipped.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $class: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 *   
 * @ingroup views_templates
 */
?>
<?php
  $row = array();
  foreach ($rows as $col){
    foreach ($col as $ltr => $value){
      $row[$ltr][] = $value;
    }
  }
  $first = TRUE;
  $element = 'odd';
?>
<table class="<?php print $class; ?>">
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>

  <?php if ($first):?>
  <thead>
    <tr class="<? echo $element; ?>">
      <th>
      </th>
      <?php foreach($row['title'] as $title): ?>
      <th>
      <?php echo $title; ?>
      </th>
      <?php endforeach; ?>
    </tr>
</thead>
<?php  $first = FALSE;
        endif; //$first
        $element = 'even';
  if (!$first): ?>
  <tbody>
    <?php foreach ($row as $cck_field => $rowname):?>
      <?php if ($cck_field != title): ?>
      <tr class="<? echo $element; ?>">
        <th>
          <?php echo $header[$cck_field]; ?>
        </th>
    <?php foreach($rowname as $count => $item): ?>
        <td>
          <?php echo $item; ?>
        </td>
      <?php endforeach; ?>
      </tr>
      <?php
          if ($element == 'odd'){
      $element = 'even';
    } else {
      $element = 'odd';
    }
     endif; //cck_field != title
  endforeach; ?>
  </tbody>
<?php endif; //!first ?>
</table>