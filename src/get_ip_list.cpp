#include<string>
#include<algorithm>
#include<vector>

#include "CConfigUtil.h"
#include "CGetIpAddressList.h"

using namespace std;

int ipaddress_is_in_list(const char* connectionString, const char* query, const char* ipAddress)
{
   string address(ipAddress);
   CGetIpAddressList * ipAddressList = new CGetIpAddressList(string(connectionString), string(query));

   auto result = ipAddressList->ToList();
   auto resultSet = find_if(result.begin(), result.end(), [address](string ip) { return ip.find(address) != string::npos; });

   if(ipAddressList)
	delete ipAddressList;

   return 0;
}
