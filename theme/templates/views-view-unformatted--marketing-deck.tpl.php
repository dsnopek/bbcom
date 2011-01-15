<?php
// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */

jquery_ui_add(array('ui.tabs'));
drupal_add_js(drupal_get_path('theme', 'lingwoorg_theme') .'/js/marketing-deck.js', 'theme');
//drupal_add_css(drupal_get_path('module', 'jquery_ui') .'/jquery.ui/themes/default/ui.all.css');
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="tabs">
  <ul class="tabNavigation clear-block">
    <?php foreach ($rows as $id => $row): ?>
      <li><a href="#tab-<?php print $id ?>"><?php print $view->result[$id]->node_title ?></a></li>
    <?php endforeach; ?>
  </ul>

  <?php foreach ($rows as $id => $row): ?>
    <div id="tab-<?php print $id ?>" class="<?php print $classes[$id]; ?>">
      <?php print $row; ?> 
    </div>
  <?php endforeach; ?>
</div>

