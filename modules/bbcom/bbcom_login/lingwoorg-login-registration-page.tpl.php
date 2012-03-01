<!DOCTYPE html>
<html>
<head>
  <title><?php print $batch->series ?> <?php if($batch->notes) print ' - '. $batch->notes; ?></title>
</head>
<body onload="setTimeout(function () { print(); }, 250);">
<?php print $content ?>
</body>
</html>

