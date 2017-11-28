/*
  Copyright (c) 2017, Felix Kaechele <felix@kaechele.ca>
  All rights reserved.

  Derived from: gluon-mesh-batman-adv/src/respondd.c
  Copyright (c) 2016, Matthias Schiffer <mschiffer@universe-factory.net>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <respondd.h>

#include <iwinfo.h>
#include <json-c/json.h>
#include <libgluonutil.h>

#include <glob.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


static struct json_object * get_wifi_neighbours(const char *ifname) {
	const struct iwinfo_ops *iw = iwinfo_backend(ifname);
	if (!iw)
		return NULL;

	int len;
	char buf[IWINFO_BUFSIZE];
	if (iw->assoclist(ifname, buf, &len) < 0)
		return NULL;

	struct json_object *neighbours = json_object_new_object();

	struct iwinfo_assoclist_entry *entry;
	for (entry = (struct iwinfo_assoclist_entry *)buf; (char*)(entry+1) <= buf + len; entry++) {
		struct json_object *obj = json_object_new_object();

		struct iwinfo_rate_entry *tx_rate_entry = &entry->tx_rate;
		json_object_object_add(obj, "txrate", json_object_new_int(tx_rate_entry->rate));

		char mac[18];
		snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
			 entry->mac[0], entry->mac[1], entry->mac[2],
			 entry->mac[3], entry->mac[4], entry->mac[5]);

		json_object_object_add(neighbours, mac, obj);
	}

	struct json_object *ret = json_object_new_object();

	if (json_object_object_length(neighbours))
		json_object_object_add(ret, "neighbours", neighbours);
	else
		json_object_put(neighbours);

	return ret;
}

static struct json_object * get_wifi(void) {
	const char *mesh = "bat0";

	struct json_object *ret = json_object_new_object();

	const char *format = "/sys/class/net/%s/lower_*";
	char pattern[strlen(format) + strlen(mesh)];
	snprintf(pattern, sizeof(pattern), format, mesh);

	size_t pattern_len = strlen(pattern);

	glob_t lower;
	if (!glob(pattern, GLOB_NOSORT, NULL, &lower)) {
		size_t i;
		for (i = 0; i < lower.gl_pathc; i++) {
			const char *ifname = lower.gl_pathv[i] + pattern_len - 1;
			char *ifaddr = gluonutil_get_interface_address(ifname);
			if (!ifaddr)
				continue;

			struct json_object *neighbours = get_wifi_neighbours(ifname);
			if (neighbours)
				json_object_object_add(ret, ifaddr, neighbours);

			free(ifaddr);
		}

		globfree(&lower);
	}

	return ret;
}

static struct json_object * respondd_provider_neighbours(void) {
	struct json_object *ret = json_object_new_object();

	struct json_object *wifi = get_wifi();
	if (wifi)
		json_object_object_add(ret, "wifi", wifi);

	return ret;
}


const struct respondd_provider_info respondd_providers[] = {
	{"neighbours", respondd_provider_neighbours},
	{}
};
