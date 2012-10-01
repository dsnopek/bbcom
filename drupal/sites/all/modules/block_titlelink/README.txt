=====
Block Title Link

-----
Block Title Link is a simple module that creates a link field in the Block Admin page. It works by creating a new template variable in the $block object called $block->title_link. It then uses hook_preprocess_block to wrap a link around the block->subject variable. 

=====
Updates
=====
Feb 11, 2010
 This module initially relied on a theme level template modifcation for wrapping the title link around the $block->subject. As of the latest dev release this wrapping is performed on the module level and a new template is no longer required.  
