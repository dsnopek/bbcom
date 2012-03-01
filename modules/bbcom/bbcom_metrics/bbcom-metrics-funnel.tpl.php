<div class="funnel">
<?php if ($title): ?>
  <div class="funnel-title"><?php echo $title; ?></div>
<?php endif; ?>
  <div class="funnel-steps">
  <?php foreach ($steps as $idx => $step): ?>
    <div class="funnel-step funnel-step-<?php echo $idx; ?>">
      <span class="funnel-step-title"><?php echo $step['event']; ?></span>
      <span class="funnel-step-metrics">
        <span class="funnel-count"><?php echo $step['count']; ?></span>
        <span class="funnel-percent">(<?php echo round(100 * $step['step_conv_ratio']); ?>%)</span>
      </span>
    </div>
  <?php endforeach; ?>
  </div>
</div>
