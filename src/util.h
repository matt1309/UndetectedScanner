#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
//namespace fs = std::filesystem;


class util
{


	
public:
	static bool hasKey(std::map<std::string, std::string>& map, const std::string& key);
	static bool hasKey(std::unordered_map<std::string, std::string>& map, const std::string& key);
	static bool hasKey(std::map<std::string, std::vector<std::string>>& map, const std::string& key);

	static void write(const std::string& filename, const std::vector<std::string>& content);
	static std::vector<std::string> readLines(const std::string& filepath);
	static std::vector<std::string> vecCompare(const std::vector<std::string>& one, const std::vector<std::string>& two);
	static std::vector<std::string> split(const std::string& input, const std::string& delimiter);

	bool compareIgnoreCase(const std::string& str1, const std::string& str2);
	std::string trim(const std::string& str);
	static std::string toLowerCase(const std::string& str);
};

#endif








