#include "CGetIpAddressList.h"
#include "CConfigUtil.h"

#ifdef WIN32
#include <mysql/mysql.h>
#else
#include <mariadb/mysql.h>
#endif

#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

const vector<string> ExitInError(MYSQL * conn)
{
	cout << mysql_error(conn) << endl;
	mysql_close(conn);
	return vector<string>();
}

CGetIpAddressList::CGetIpAddressList(std::string connectionString, std::string query)
{
	CGetIpAddressList::setQuery(query);
	CGetIpAddressList::setConnectionString(connectionString);
}

void CGetIpAddressList::setConnectionString(string connectionString)
{
	CConfigUtil util;
	auto configs = util.Explode(connectionString, ';');

	server = util.Explode(util.GetConfigItem(configs, "Server="), '=')[1];
	database = util.Explode(util.GetConfigItem(configs, "Database="), '=')[1];
	user = util.Explode(util.GetConfigItem(configs, "User="), '=')[1];
	password = util.Explode(util.GetConfigItem(configs, "Password="), '=')[1];
	auto portString = util.Explode(util.GetConfigItem(configs, "Port="), '=')[1];
	port = stoi(portString);
}


void CGetIpAddressList::setQuery(string query)
{
	this->query = query;
}

vector<string> CGetIpAddressList::ToList()
{
	if (CGetIpAddressList::query == "") 
	{
		return vector<string>();
	}

	MYSQL *conn = mysql_init(NULL);

	if (conn == NULL)
	{
		return vector<string>();
	}

	if (mysql_real_connect(conn, server.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0) == NULL)
	{
		return ExitInError(conn);
	}

	if (mysql_query(conn, query.c_str()))
	{
		return ExitInError(conn);
	}

	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL)
	{
		return ExitInError(conn);
	}

	vector<string> resultset;

	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result)))
	{
		resultset.push_back(row[0]);
	}

	mysql_close(conn);

	return resultset;
}
