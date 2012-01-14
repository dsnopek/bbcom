This module is designed for site admins looking for a easier way to manage
a large number of coded blocks in a uniform manner. This is accomplished by
using the well tested ctools plugin system.

Out of the box, you can begin creating blocks using ctools_block to expose
your blocks. If the block ID's are important or you need more control in some
way, the module has a proxy layer that can be called in your own module.

Examples of both methods can be found in ctools_block_example.module.

NOTE: Do NOT put theme functions in your plugin includes. Do to the way ctools
$plugin arrays work, this can lead to race conditions between theme registry
building and plugin registry building where your block will disapear.
