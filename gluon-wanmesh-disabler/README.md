# gluon-wanmesh-disabler

On nodes where Mesh-on-WAN (MoW) is enabled this package monitors the WAN
interface for mesh partners. If none are found for six hours MoW is disabled.

This is useful to disable MoW on nodes that previously had MoW enabled by
default (eg. through the entry in site.conf) while keeping Mesh-on-WAN
enabled where it is actually being used.