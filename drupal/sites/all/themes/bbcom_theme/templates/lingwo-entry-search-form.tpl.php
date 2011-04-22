<?php
unset($form['search_word']['#title']);
$form['search_word']['#attributes']['style'] = 'float: left';
print drupal_render($form['search_word']);
print drupal_render($form['submit']);
print drupal_render($form);
?>

