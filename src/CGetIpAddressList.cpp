#include "CGetIpAddressList.h"
#include<mysql/mysql.h>
#include<iostream>
#include<algorithm>
#include <string>

using namespace std;

const vector<string> Explode(const string& s, const char& c)
{
	string buff{ "" };
	vector<string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

const vector<string> ExitInError(MYSQL * conn)
{
	cout << mysql_error(conn) << endl;
	mysql_close(conn);
	return vector<string>();
}

const string GetConfigItem(vector<string> &configs, const string configName)
{
	return find_if(configs.begin(), configs.end(), [configName](string config) { return config.find(configName) != string::npos; })[0];
}

CGetIpAddressList::CGetIpAddressList(std::string connectionString, std::string query)
{
	CGetIpAddressList::setQuery(query);
	CGetIpAddressList::setConnectionString(connectionString);
}

void CGetIpAddressList::setConnectionString(string connectionString)
{
	auto configs = Explode(connectionString, ';');

	server = Explode(GetConfigItem(configs, "Server="), '=')[1];
	database = Explode(GetConfigItem(configs, "Database="), '=')[1];
	user = Explode(GetConfigItem(configs, "User="), '=')[1];
	password = Explode(GetConfigItem(configs, "Password="), '=')[1];
	auto portString = Explode(GetConfigItem(configs, "Port="), '=')[1];
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