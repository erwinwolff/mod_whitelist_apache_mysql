#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_protocol.h"
#include "http_request.h"
#include "util_script.h"
#include "http_connection.h"
#ifdef HAVE_UNIX_SUEXEC
#include "unixd.h"
#endif
#include "scoreboard.h"
#include "mpm_common.h"

#include "apr_strings.h"

/* module AP_MODULE_DECLARE_DATA black_list_mysql_module;

static int x_check_access(request_rec *r)
{
    return DECLINED;
}

static void x_register_hooks(apr_pool_t *p)
{
	ap_hook_check_access(x_check_access, NULL, NULL, APR_HOOK_MIDDLE, AP_AUTH_INTERNAL_PER_CONF);
}*/
