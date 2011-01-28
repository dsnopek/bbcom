<?php
$forms = implode(', ', array(
  $fields['nonpast.singular.1p']['value'],
  $fields['nonpast.singular.3p']['value'],
  $fields['nonpast.plural.3p']['value']
));
?>
<p><? print $forms;?></p>
