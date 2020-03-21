#pragma once
#include<vector>
#include<string>

class CGetIpAddressList {
public:
	CGetIpAddressList(std::string connectionString, std::string query);
	void setConnectionString(std::string connectionString);
	void setQuery(std::string query);
	std::vector<std::string> ToList();
private:
	std::string query;

	std::string database;
	std::string server;
	std::string user;
	std::string password;
	int port;
};