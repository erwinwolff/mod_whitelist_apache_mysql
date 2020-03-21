#include "CConfigUtil.h"

#include<algorithm>

using namespace std;

const vector<string> CConfigUtil::Explode(const string& s, const char& c)
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

const string CConfigUtil::GetConfigItem(vector<string> &configs, const string configName)
{
	return find_if(configs.begin(), configs.end(), [configName](string config) { return config.find(configName) != string::npos; })[0];
}