<?php
$stem = $fields['*stem']['value'];

$forms = $fields['nominative.singular.masculine']['value'];
$forms .= ', '. str_replace($stem, '-', $fields['nominative.singular.feminine']['value']);
$forms .= ', '. str_replace($stem, '-', $fields['nominative.singular.neuter']['value']);
?>
<p><? print $forms;?></p>

