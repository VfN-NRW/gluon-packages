#!/bin/sh

. /usr/share/libubox/jshn.sh

PRIMARY_MAC=$(cat /lib/gluon/core/sysconfig/primary_mac)

CUR_NAME=$(uci get system.@system[0].hostname)
CUR_LON=$(uci get gluon-node-info.@location[0].longitude)
CUR_LAT=$(uci get gluon-node-info.@location[0].latitude)

CHANGED=""
DB_RESULT="$(wget -q -O - "http://nodeapi.vfn-nrw.de/index.php/get/node/byGluonPrimaryMac/${PRIMARY_MAC}" || wget -q -O - "http://nodeapi.vfn/api/get/node/byGluonPrimaryMac/${PRIMARY_MAC}")"

json_load "$DB_RESULT"

json_get_var lat DbLatitude
json_get_var lon DbLongitude
json_get_var name DbName

if [ ! -z "$name" ]
then
	if [ "$name" != "$CUR_NAME" ]
	then
		pretty-hostname "$name"
		logger -t vfn-db-nodesync "Changed hostname from $CUR_NAME to $name"
		CHANGED="yes"
	fi
fi

if [ ! -z "$lat" ]
then
	if [ "$lat" != "$CUR_LAT" ]
	then
		uci set gluon-node-info.@location[0].share_location='1'
		uci set gluon-node-info.@location[0].latitude="$lat"
		logger -t vfn-db-nodesync "Changed latitude from $CUR_LAT to $lat"
		CHANGED="yes"
	fi
fi

if [ ! -z "$lon" ]
then
	if [ "$lon" != "$CUR_LON" ]
	then
		uci set gluon-node-info.@location[0].share_location='1'
		uci set gluon-node-info.@location[0].longitude="$lon"
		logger -t vfn-db-nodesync "Changed longitude from $CUR_LON to $lon"
		CHANGED="yes"
	fi
fi

if [ ! -z "$CHANGED" ]
then
	uci commit gluon-node-info
	# self destruct
	rm /usr/lib/micron.d/vfn-db-nodesync
	rm -Rf /lib/gluon/vfn-db-nodesync
	/etc/init.d/micrond restart
	logger -t vfn-db-nodesync "DB Nodesync done. Changes committed"
else
	logger -t vfn-db-nodesync "No changes"
fi
