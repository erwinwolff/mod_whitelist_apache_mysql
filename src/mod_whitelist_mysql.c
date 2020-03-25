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
#include "get_ip_list.h"

void          *create_dir_conf(apr_pool_t *pool, char *context);
void          *mysql_whitelist_merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD);

module AP_MODULE_DECLARE_DATA whitelist_mysql_module;

typedef struct {
    int         enabled;
    const char *query;
    const char *connectionString;
} whitelist_mysql_config;


void *create_dir_conf(apr_pool_t *pool, char *context)
{
    context = context ? context : "Newly created configuration";

    whitelist_mysql_config *cfg = apr_pcalloc(pool, sizeof(whitelist_mysql_config));
    if(cfg)
    {
        {
            /* Set some default values */
		cfg->query = "";
		cfg->connectionString = "";
		cfg->enabled = 0;
        }
    }
    ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "Standard MySql Whitelist configuration initialized");
    return cfg;
}


void *mysql_whitelist_merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD) {
    whitelist_mysql_config *base = (whitelist_mysql_config *) BASE ;
    whitelist_mysql_config *add = (whitelist_mysql_config *) ADD ;  
    whitelist_mysql_config *conf = (whitelist_mysql_config *) create_dir_conf(pool, "Merged configuration"); 

    conf->enabled = ( add->enabled == 0 ) ? base->enabled : add->enabled ;

    return conf ;
}


static whitelist_mysql_config config;

static int whitelist_check_access(request_rec *r)
{
	if (config.enabled == 0)
		return DECLINED;

	if (strcasecmp(config.query, "") || strcasecmp(config.connectionString, ""))
		return DECLINED;

        ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "Ip Address -> %s", r->useragent_ip);
	int isInList = ipaddress_is_in_list(config.connectionString, config.query, r->useragent_ip);

	if (isInList == 0)
		return HTTP_UNAUTHORIZED;

	return DECLINED;
}

const char *whitelist_mysql_set_query(cmd_parms *cmd, void *cfg, const char *arg)
{
    config.query = arg;
    return NULL;
}

const char *whitelist_mysql_set_connectionstring(cmd_parms *cmd, void *cfg, const char *arg)
{
    config.connectionString = arg;
    return NULL;
}

const char *whitelist_mysql_set_enable(cmd_parms *cmd, void *cfg, const char *arg)
{
    if(!strcasecmp(arg, "on"))
	config.enabled = 1;
    else
	config.enabled = 0;
    return NULL;
}


static const command_rec whitelist_mysql_directives[] =
{
    AP_INIT_TAKE1("WhiteListMySqlConnectrionstring",  whitelist_mysql_set_connectionstring, NULL, RSRC_CONF, "MySql Whitelist Connectionstring"),
    AP_INIT_TAKE1("WhiteListMySqlQuery",  whitelist_mysql_set_query, NULL, RSRC_CONF, "SQL Query to Whitelist"),
    AP_INIT_TAKE1("WhiteListMySqlEnabled",  whitelist_mysql_set_enable, NULL, RSRC_CONF, "MySql Whitelist enable"),
    { NULL }
};

static void whitelist_mysql_register_hooks(apr_pool_t *p)
{
	ap_hook_check_access(whitelist_check_access, NULL, NULL, APR_HOOK_MIDDLE, AP_AUTH_INTERNAL_PER_CONF);
}

AP_DECLARE_MODULE(whitelist_mysql_module) =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    mysql_whitelist_merge_dir_conf,  /* Merge handler for per-directory configurations */
    NULL, 
    NULL, 
    whitelist_mysql_directives,  
    whitelist_mysql_register_hooks 
};

