/*      -*- linux-c -*-
 *
 * Copyright (c) 2003 by Intel Corp.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Tariq Shureih <tariq.shureih@intel.com>
 */

#include "ipmi.h"

static void close_done(void *cb_data)
{
	int *flag = cb_data;
	dbg("close_done");
	*flag = 1;
}

static void close_connection(ipmi_domain_t *domain, void *user_data)
{
	int rv, *flag = user_data;

	dbg("close flag:%d", *flag);

	
	rv = ipmi_close_connection(domain, close_done, user_data);
	if (rv) {
		dbg("ipmi_close_connection failed!");
		*flag = 1;
	}
}

void ohoi_close_connection(ipmi_domain_id_t domain_id, void *user_data)
{
	int rv, close_flag = 0;

	dbg("ohoi_close_connection");

	rv = ipmi_domain_pointer_cb(domain_id, close_connection, &close_flag);
	
	if (rv) {
		dbg("ipmi_domain_pointer_cb failed!");
		return;
	}

	/* Removed while(!close_flag)/sel_select loop, fix for bug 1029028. */
	sleep(1);   
}
