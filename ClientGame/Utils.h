/* Adapted from pre-existing code written by Adam Livesley / @sixones */

#pragma once

#include <fstream>
#include <sstream>
#include <functional>
#include <cctype>

#include "stdafx.h"

#define BUFFER_SIZE 4096

class Utils
{
public:
	static std::string format(const std::string &format, ...);

	static std::vector<std::string> split(const std::string &content, char delimiter);
	static std::vector<std::string> split(const std::string &content, char delimiter, std::vector<std::string> &elements);

	static std::string &trim(std::string &content) { return Utils::ltrim(Utils::rtrim(content)); }
	static std::string &ltrim(std::string &content);
	static std::string &rtrim(std::string &content);

	static void loadConfigFile(std::string path, std::map<std::string, int>& outMap);
};