


#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>

//namespace fs = std::filesystem;
#include "util.h"

bool util::hasKey(std::unordered_map<std::string, std::string>& map, const std::string& key) {
    return map.find(key) != map.end();
}

bool util::hasKey(std::map<std::string, std::string>& map, const std::string& key) {
    return map.find(key) != map.end();
}

bool util::hasKey(std::map<std::string, std::vector<std::string>>& map, const std::string& key) {
    return map.find(key) != map.end();
}
/*
Old write command below:
void write(const std::string& filename, const std::vector<std::string>& content) {


    std::ofstream file(filename);
    if (file.is_open()) {


       for(const std::string& line : content){
            file << line << std::endl;
        }

        file.close();

    }
}
*/


void util::write(const std::string& filename, const std::vector<std::string>& content) {
    // Extract directory from filename
    std::filesystem::path filePath(filename);
    std::filesystem::path directory = filePath.parent_path();

    // Create directory if it doesn't exist
    if (!std::filesystem::exists(directory))
        std::filesystem::create_directories(directory);

    // Open file for writing
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const std::string& line : content) {
            file << line << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
    }
}

std::vector<std::string> util::readLines(const std::string& filepath) {
    std::vector<std::string> lines;
    std::ifstream file(filepath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filepath << std::endl;
    }

    return lines;
}



std::vector<std::string> util::vecCompare(const std::vector<std::string>& one, const std::vector<std::string>& two) {
    std::vector<std::string> newitems;

    int i = 0;
    int j = 0;

    while (i < one.size()) {
        while (j < two.size()) {
            if (i < one.size() && one[i] == two[j]) {
                i++;
                j++;
            }
            else if (i < one.size() && one[i].compare(two[j]) <= 0) {
                newitems.push_back(one[i]);
                i++;
            }
            else if (i < one.size() && one[i].compare(two[j]) > 0) {
                j++;
            }
            else {
                j = two.size();
            }
        }

        if (i != one.size() && j == two.size()) {
            newitems.push_back(one[i]);
        }
        i++;
    }

    return newitems;
}






std::vector<std::string> util::split(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::size_t prev = 0, pos;
    while ((pos = input.find(delimiter, prev)) != std::string::npos) {
        if (pos > prev) {
            tokens.push_back(input.substr(prev, pos - prev));
        }
        prev = pos + delimiter.length();
    }
    if (prev < input.length()) {
        tokens.push_back(input.substr(prev, std::string::npos));
    }
    return tokens;
}



std::string util::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos)
        return "";
    return str.substr(first, last - first + 1);
}

// Case-insensitive comparison function for strings
bool util::compareIgnoreCase(const std::string& str1, const std::string& str2) {
    if (str1.length() != str2.length())
        return false;
    return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
        return std::tolower(a) == std::tolower(b);
    });
}

std::string util::toLowerCase(const std::string& str) {
    std::string result;
    for (char c : str) {
        result += std::tolower(c);
    }
    return result;
}