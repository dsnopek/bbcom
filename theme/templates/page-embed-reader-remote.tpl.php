<?php
// $Id: page.tpl.php,v 1.1.2.1 2009/02/24 15:34:45 dvessel Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <!--[if IE]>
  <meta http-equiv="Page-Exit" content="Alpha(opacity=100)" />
  <![endif]-->
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $conditional_styles; ?>
  <?php print $scripts; ?>

  <!--[if lte IE 6]>
  <link type="text/css" rel="stylesheet" media="all" href="/<?php print drupal_get_path('theme', 'lingwoorg_theme') .'/styles/ie6.css' ?>" />
  <![endif]-->

  <style>
    html, body { height: 100%; overflow: hidden; }
  </style>
  <script>
    $(document).ready(function () {
      function layout() {
        $('#remoteFrame').css('height', $(window).height() - 75);
      };
      $(window).resize(layout);
      layout();
    });
  </script>
</head>

<body class="<?php print $body_classes; ?>">
  <div id="page">
    <div id="site-header-wrapper">
      <div id="site-header" class="container-12 clear-block">
        <div id="branding" class="clear-block">
        <?php if ($linked_logo_img): ?>
          <span id="logo" class="grid-1"><?php print $linked_logo_img; ?></span>
        <?php endif; ?>
        <?php if ($linked_site_name): ?>
          <div id="site-name" class="grid-2"><?php print $linked_site_name; ?></div>
        <?php endif; ?>

          <div class="grid-5 <?php print ns('prefix-7', $linked_logo_img, 1, $linked_site_name, 2); ?>">
            <?php print $account_links; ?>
          </div>
        </div>

      <?php if ($main_menu_links): ?>
        <div id="site-menu" class="<?php print ns('grid-8', $linked_logo_img, 1); ?><?php if ($linked_logo_img) print ' prefix-1'; ?>">
          <?php print $main_menu_links; ?>
        </div>
      <?php endif; ?>

        <!-- Replace language switch with return link -->
        <div id="language-switcher" class="grid-4 <?php print ns('prefix-8', $main_menu_links, 8); ?>">
          <a href="javascript:window.history.back();"><?php print t('Return to BiblioBird'); ?> &gt;&gt;</a>
        </div>
      </div>
    </div>

    <?php print $content ?>
  </div>

  <?php print $closure; ?>
</body>
</html>
