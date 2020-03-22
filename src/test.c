#include <stdio.h>
#include "get_ip_list.h"

int main()
{
	printf("Initializing whitelist test");

	int result = ipaddress_is_in_list("Server=localhost;Database=TestDb;User=test;Password=test", "SELECT * FROM allowedips", "127.0.0.1");

	printf("\n%s\n", result);
}
