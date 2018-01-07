#!/bin/sh
# Checks if there are a mesh clients on WAN (Mesh on WAN, MoW)
# If no WAN mesh partners are found after a defined time of runs disable MoW

MAX_RUNS=6

WAN_MESH_ENABLED=$(uci get network.mesh_wan.auto)

if [ "$WAN_MESH_ENABLED" = "0" ]
then
	logger -t mow-disabler "Mesh on WAN disabled, doing nothing"
	exit 0
fi

NUM_WAN_MESH=$(batctl o | grep -c br-wan)

if [ "$NUM_WAN_MESH" = "0" ]
then
	logger -t mow-disabler "No mesh partners on WAN, increasing counter"
	date >> /tmp/wan_mesh_empty
	NUM_WAN_MESH_EMPTY=$(wc -l < /tmp/wan_mesh_empty)
	if [ "$NUM_WAN_MESH_EMPTY" -ge "$MAX_RUNS" ]
	then
		logger -t mow-disabler "No mesh partners on WAN after $MAX_RUNS runs, disabling MoW"
		rm /tmp/wan_mesh_empty
		uci set network.mesh_wan.auto=0
		uci commit network
		/etc/init.d/network restart
	fi
else
	logger -t mow-disabler "Mesh partners on WAN, doing nothing"
	[ -f /tmp/wan_mesh_empty ] && rm /tmp/wan_mesh_empty
	exit 0
fi
