<?php
$stem = $fields['*stem']['value'];

$gender = $fields['gender']['value'];
if ($gender == 'masculine') {
  $stem = $fields['*stem']['value'];
  $forms = $fields['nominative.singular']['value'];
  $forms .= ', '. str_replace($stem, '-', $fields['nominative.plural']['value']);
  $forms .= ', '. str_replace($stem, '-', $fields['genitive.singular']['value']);
}
?>
<p><i><?print $gender ?></i>. <? print $forms;?></p>
