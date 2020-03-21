#pragma once
#include<string>
#include<vector>

class CConfigUtil {
public:
	const std::vector<std::string> Explode(const std::string& s, const char& c);
	const std::string GetConfigItem(std::vector<std::string> &configs, const std::string configName);
};