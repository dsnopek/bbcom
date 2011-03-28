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
  <link type="text/css" rel="stylesheet" media="all" href="/<?php print drupal_get_path('theme', 'bbcom_theme') .'/styles/ie6.css' ?>" />
  <![endif]-->
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
        <div id="site-menu" class="<?php print ns('grid-9', $linked_logo_img, 1); ?><?php if ($linked_logo_img) print ' prefix-1'; ?>">
          <?php print $main_menu_links; ?>
        </div>
      <?php endif; ?>
      <?php if ($language_switcher): ?>
        <div id="language-switcher" class="grid-3 <?php print ns('prefix-9', $main_menu_links, 9); ?>">
          <?php print $language_switcher; ?>
        </div>
      <?php endif; ?>
      </div>
    </div>

    <div id="main-wrapper" class="container-12 clear-block">
      <div id="main" class="column <?php print ns('grid-12', $right || $always_right, 4); ?> <?php print ns('push-2', !$left, 2); ?>">
        <?php if ($header): ?>
          <div id="header-region" class="region">
            <?php print $header; ?>
          </div>
        <?php endif; ?>

        <?php if ($title && !$inner_title): ?>
          <h1 class="title" id="page-title"><?php print $lang_spec; ?><?php print $title; ?></h1>
        <?php endif; ?>

        <?php if ($tabs): ?>
          <div class="tabs"><?php print $tabs; ?></div>
        <?php endif; ?>
        <div id="content-wrapper" class="<?php print $tabs ? 'with-tabs' : 'without-tabs'?> clear-block">
            <?php if ($content_top): ?>
              <div id="content-top" class="region content-region alpha omega <?php print ns('grid-12', $right || $always_right, 4); ?>">
                <div id="content-top-inner">
                  <?php print $content_top; ?>
                </div>
              </div>
            <?php endif; ?>

            <div class="clear-block content-region">
              <div id="content-main" class="region alpha <?php if (!$right && !$always_right && !$content_right) print "omega" ?> <?php print ns('grid-12', $right || $always_right, 4, $content_right, 4); ?>">
                <div id="content-main-inner">
                  <?php if ($title && $inner_title): ?>
                    <h1 class="title" id="page-title"><?php print $title; ?></h1>
                  <?php endif; ?>

                  <?php print $messages; ?>
                  <?php print $help; ?>

                  <?php print $content; ?>
                </div>
              </div>

              <?php if($content_right): ?>
                <div id="content-right" class="region omega grid-4">
                  <div id="content-right-inner">
                    <?php print ($content_right); ?>
                  </div>
                </div>
              <?php endif; ?>
            </div>

            <?php if($content_bottom): ?>
              <div id="content-bottom" class="content-region region alpha omega <?php print ns('grid-12', $right || $always_right, 4); ?>">
                <div id="content-bottom-inner">
                  <?php print ($content_bottom); ?>
                </div>
              </div>
            <?php endif; ?>
          </div>

        <div id="content-footer">
          <?php print $feed_icons; ?>
        </div>
      </div>

    <?php if ($left): ?>
      <div id="sidebar-left" class="column sidebar region grid-2 <?php print ns('pull-8', !$right, 2); ?>">
        <?php print $left; ?>
      </div>
    <?php endif; ?>

      <div id="sidebar-right" class="column sidebar region <?php print ns('grid-4', $left, 2); ?>">
        <?php print $right; ?>
      </div>
    </div>

    <div id="footer-wrapper">
      <div id="footer" class="container-12 prefix-1 suffix-1">
        <div id="copyright">Copyright &copy; 2010 Lingwo International S.C.</div>
        <?php if ($footer): ?>
          <div id="footer-region" class="region grid-10 clear-block">
            <?php print $footer; ?>
          </div>
        <?php endif; ?>

        <?php if ($footer_message): ?>
          <div id="footer-message" class="grid-10">
            <?php print $footer_message; ?>
          </div>
        <?php endif; ?>
      </div>
    </div>
  </div>
  <?php print $closure; ?>
</body>
</html>
