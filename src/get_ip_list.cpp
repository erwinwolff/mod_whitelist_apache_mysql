#include<string>
#include<algorithm>
#include<vector>

#include "CConfigUtil.h"
#include "CGetIpAddressList.h"

using namespace std;

extern "C" int ipaddress_is_in_list(const char* connectionString, const char* query, const char* ipAddress)
{
	string address(ipAddress);
	CGetIpAddressList * ipAddressList = new CGetIpAddressList(string(connectionString), string(query));

	auto result = ipAddressList->ToList();

	bool foundIp = false;
	for(size_t i = 0; i < result.size(); i++)
	{
		if(result[i] == address)
			foundIp = true;
	}

	if(ipAddressList)
		delete ipAddressList;

	return foundIp;
}
