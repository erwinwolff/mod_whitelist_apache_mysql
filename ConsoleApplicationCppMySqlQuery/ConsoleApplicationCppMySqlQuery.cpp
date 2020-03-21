#include <iostream>
#include "..\src\CGetIpAddressList.h"
#include <mysql/mysql.h>

using namespace std;

int main()
{
	cout << "MySQL client version:\t" << mysql_get_client_info() << endl;

	CGetIpAddressList getIpAddressList("Server=localhost;Database=*;User=*;Password=**;Port=3306", "SELECT Ip FROM allowedipaddresses");

	auto result = getIpAddressList.ToList();

	for (size_t i = 0; i < result.size(); i++)
	{
		cout << result[i] << endl;
	}
}