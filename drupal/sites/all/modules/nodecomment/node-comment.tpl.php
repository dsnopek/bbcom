<?php
/**
 * For comments as nodes to work, we need to know in advance what node types
 * will be used as comments and theme them differently. In particular, pay
 * attention to the way the title is output. This tpl.php assumes a content
 * type called "comment".
 */
 ?>

<div id="comment-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } if (!$status) { print ' node-unpublished'; } if (!empty($new_class)) { print ' ' . $new_class; } ?>">
  <?php if ($first_new): ?>
    <?php print $first_new; ?>
  <?php endif; ?>

  <?php if ($new_output): ?>
    <?php print $new_output; ?>
  <?php endif; ?>

  <?php print $picture ?>

  <div class="subject">
    <?php print $comment_link; ?>
  </div>
  <?php if ($submitted): ?>
    <span class="submitted"><?php print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
  <?php endif; ?>

  <div class="content">
    <?php print $content ?>
  </div>
  <?php if ($signature): ?>
  <div class="user-signature clear-block">
    <?php print $signature ?>
  </div>
  <?php endif; ?>

  <div class="clear-block clear">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links clear-block"><?php print $links; ?></div>
    <?php endif; ?>
  </div>
</div>