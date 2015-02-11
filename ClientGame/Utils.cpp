/* Adapted from pre-existing code written by Adam Livesley / @sixones */

#include "stdafx.h"
#include "Utils.h"

std::string Utils::format(const std::string &msg, ...)
{
	va_list ap;
	char text[BUFFER_SIZE] = { 0, };

	va_start(ap, msg);

	// CHANGE: Replaced vsnprintf with vsnprintf_s
	vsnprintf_s(text, BUFFER_SIZE, msg.c_str(), ap);
	va_end(ap);

	return std::string(text);
}

std::vector<std::string> Utils::split(const std::string &content, char delimiter) {
	std::vector<std::string> elements;

	Utils::split(content, delimiter, elements);

	return elements;
}

std::vector<std::string> Utils::split(const std::string &content, char delimiter, std::vector<std::string> &elements)
{
	std::stringstream stream(content);
	std::string item;

	while (std::getline(stream, item, delimiter))
	{
		elements.push_back(item);
	}

	return elements;
}

std::string &Utils::ltrim(std::string &content)
{
	content.erase(content.begin(), std::find_if(content.begin(), content.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

	return content;
}

std::string &Utils::rtrim(std::string &content)
{
	content.erase(std::find_if(content.rbegin(), content.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), content.end());

	return content;
}

void Utils::loadConfigFile(std::string path, std::map<std::string, int>& outMap)
{
	sysLog.Log(Utils::format("Loading Resource file from '%s'.", path.c_str()));

	std::ifstream stream;
	stream.open(path);

	std::string line;

	while (std::getline(stream, line))
	{
		if (line.compare(0, 1, "#") == 0)
			continue;

		if (line.find("=") != std::string::npos)
		{
			std::vector<std::string> values = Utils::split(line, '=');

			std::string key = Utils::trim(values[0]);
			int value = StringToInt(Utils::trim(values[1]));

			outMap.insert(std::pair<std::string, int>(key, value));
		}
	}
}