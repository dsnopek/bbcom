Description:
------------
This module allows you to grant access for users to see unpublished nodes.

Usage:
------
view_unpublished looks for the "view all unpublished content" permission and
then the "view unpublished (node type) content" permission when a user tries to
view a node that is unpublished.

After installing the module, navigate to your user access page and assign the
appropriate permissions to the roles you wish to be able to view unpublished nodes.

Views integration:
------------------
If you use the "Node: Published or Admin" filter in Views, your view_unpublished
settings will carry over to Views. Thanks to Manuel Garcia for the Views filter.
Note: this only works for the "View all unpublished" permission, no support yet
for the per-content-type permissions.


Code Contributions:
-------------------
Brad Bowman/beeradb - Aten Design Group
Caleb Delnay/calebd
Domenic Santangelo/dsantangelo - WorkHabit
