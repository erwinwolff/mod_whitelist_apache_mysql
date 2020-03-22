#include<string>
#include<algorithm>
#include<vector>

#include "CConfigUtil.h"
#include "CGetIpAddressList.h"

using namespace std;

bool ipaddress_is_in_list(const char* connectionString, const char* query, const char* ipAddress)
{
   string address(ipAddress);

   CGetIpAddressList ipAddressList(connectionString, query);

   auto result = ipAddressList.ToList();
   auto resultSet = find_if(result.begin(), result.end(), [address](string ip) { return ip.find(address) != string::npos; });

   return resultSet.size() > 0;
}