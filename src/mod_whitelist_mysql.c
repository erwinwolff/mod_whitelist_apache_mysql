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

void          *mysql_whitelist_create_dir_conf(apr_pool_t *pool, char *context);
void          *mysql_whitelist_merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD);

module AP_MODULE_DECLARE_DATA whitelist_mysql_module;

typedef struct {
    char        context[256];
    int         enabled;
    char	query[32000];
    char	connectionString[256];
} whitelist_mysql_config;

void *mysql_whitelist_create_dir_conf(apr_pool_t *pool, char *context)
{
	context = context ? context : "(undefined context)";

	ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "Standard MySql Whitelist configuration being created ...");

	whitelist_mysql_config *cfg = apr_pcalloc(pool, sizeof(whitelist_mysql_config));
	if (cfg)
	{
		char * connectionString = "Server=localhost;Database=Testdb;User=test;Password=test;Port=3306";
		char * query =  "SELECT Ip FROM allowedips";

		strcpy(cfg->context, context);
		/* Set some default values */
		cfg->enabled = 1;
		strcpy(cfg->connectionString, connectionString);
		strcpy(cfg->query, query);

		//	cfg->query = "Server=localhost;Database=Testdb;User=test;Password=test;Port=3306";
		//	cfg->connectionString = "SELECT Ip FROM allowedips";
	}
	ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "Standard MySql Whitelist configuration initialized");
	return cfg;
}


void *mysql_whitelist_merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD)
{
/*	whitelist_mysql_config *base = (whitelist_mysql_config *) BASE ;
	whitelist_mysql_config *add = (whitelist_mysql_config *) ADD ;
	whitelist_mysql_config *conf = (whitelist_mysql_config *) mysql_whitelist_create_dir_conf(pool, "Merged configuration");

	conf->enabled = ( add->enabled == 0 ) ? base->enabled : add->enabled ;

	if (!strcasecmp(add->query, ""))
		strcpy(conf->query, add->query);
	else
		strcpy(conf->query, base->query);

	if (!strcasecmp(add->connectionString, ""))
		strcpy(conf->connectionString, add->connectionString);
	else
		strcpy(conf->connectionString, base->connectionString);
*/
//    conf->query = ( strcasecmp(add->query, "")) ? base->query : add->query;
//    conf->connectionString = ( strcasecmp(add->connectionString, "")) ? base->connectionString : add->connectionString;

//    ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "MySql Whitelist enabled: %d", conf->enabled);
//    ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "MySql Whitelist connectionString: %s", conf->connectionString);
//    ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "MySql Whitelist query: %s", conf->query);

    return mysql_whitelist_create_dir_conf(pool, "Merged configuration");
}

static int whitelist_check_access(request_rec *r, int lookup_uri)
{
	whitelist_mysql_config *config = (whitelist_mysql_config*) ap_get_module_config(r->per_dir_config, &whitelist_mysql_module);

	ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "Context: %s", config->connectionString);
	ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "Context: %s", config->query);
	ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "Context: %s", config->context);

	if (config->enabled == 0)
	{
		ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "MySql Whitelist not enabled");
		return DECLINED;
	}

	if (strcmp(config->query, "") == 0 ||
	    strcmp(config->connectionString, "") == 0)
	{
		ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "MySql Whitelist has no query nor connectionstring set");
		return DECLINED;
	}

	int isInList = ipaddress_is_in_list(config->connectionString, config->query, r->useragent_ip);
	if (isInList == 0)
	{
		ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "%s not in whitelist", r->useragent_ip);
		return HTTP_UNAUTHORIZED;
	}

	ap_log_error(APLOG_MARK, APLOG_INFO, 0, r->server, "Finished MySql Whitelist approved request");
	return DECLINED;
}

const char *whitelist_mysql_set_query(cmd_parms *cmd, void *cfg, const char *arg)
{
	whitelist_mysql_config *config = (whitelist_mysql_config *) cfg ;

        if (config)
                strcpy(config->query, arg);

	ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "MySql Whitelist Query: %s", config->query);
	return NULL;
}

const char *whitelist_mysql_set_connectionstring(cmd_parms *cmd, void *cfg, const char *arg)
{
	whitelist_mysql_config *config = (whitelist_mysql_config *) cfg ;

	if (config)
		strcpy(config->connectionString,arg);

	ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "MySql Whitelist ConnectionString: %s", config->connectionString);
	return NULL;
}

const char *whitelist_mysql_set_enable(cmd_parms *cmd, void *cfg, const char *arg)
{
	whitelist_mysql_config *config = (whitelist_mysql_config *) cfg ;

	if (config)
	{
	    if(strcasecmp(arg, "on"))
		config->enabled = 1;
	}

	ap_log_error(APLOG_MARK, APLOG_INFO, 0, 0, "MySql Whitelist enabled: %d", config->enabled);
	return NULL;
}


static const command_rec whitelist_mysql_directives[] =
{
    AP_INIT_TAKE1("WhiteListMySqlConnectrionstring",  whitelist_mysql_set_connectionstring, NULL, ACCESS_CONF, "MySql Whitelist Connectionstring"),
    AP_INIT_TAKE1("WhiteListMySqlQuery",  whitelist_mysql_set_query, NULL, ACCESS_CONF, "SQL Query to Whitelist"),
    AP_INIT_TAKE1("WhiteListMySqlEnabled",  whitelist_mysql_set_enable, NULL, ACCESS_CONF, "MySql Whitelist enable"),
    { NULL }
};

static void whitelist_mysql_register_hooks(apr_pool_t *p)
{
	ap_hook_quick_handler(whitelist_check_access, NULL, NULL, APR_HOOK_FIRST);
}

module AP_MODULE_DECLARE_DATA whitelist_mysql_module = {

    STANDARD20_MODULE_STUFF,
    mysql_whitelist_create_dir_conf,
    mysql_whitelist_merge_dir_conf,
    NULL,
    NULL,
    whitelist_mysql_directives,
    whitelist_mysql_register_hooks
};
