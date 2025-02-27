#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include <cstdbool>
#include <atomic>
#include <unordered_set>
//#include "siteScraper.h"
#include "util.h"

#include <mutex>

#ifndef CONFIGCLASS_H
#define CONFIGCLASS_H
class ConfigClass
{

public:

    struct scraperSettings
    {
        std::string name;

        std::map<std::string, std::string> stringSettings;
        std::map<std::string, int> intSettings;
        std::map<std::string, std::vector<std::string>> vectorSettings;
        std::map<std::string, std::vector<std::pair<std::string, std::string>>> vectorVectorPair;
    };



private:
    // General setting memory objects

    // locks for accessing the generic config settings, These wont be used until scrapers are written
    std::mutex stringConfigsLock;
    std::mutex intConfigsLock;
    std::mutex stringArrConfigsLock;

    std::mutex LongConfigsLock;

    //   std::mutex intConfigsLock;

    // Boolean for checking when config fully loaded in. Probably should make that atomic.
    std::atomic<bool> importComplete;



    // this is where the list of potential settings are stored. (need another one for each of required meaning the rest are optional, then need an if to check if they are not optional)
    std::string singlePagescraperSettingsStringOptions[10];
    std::string singlePagescraperSettingsIntOptions[6];
    std::string singlePagescraperSettingsVectorOptions[4];
    std::string singlePagescraperSettingsVectorVectorPairStringsOptions[1];

    std::string multiPagescraperSettingsSingleStringOptions[3];
    std::string multiPagescraperSettingsSingleIntOptions[1];
    std::string multiPagescraperSettingsStringOptions[7];
    std::string multiPagescraperSettingsIntOptions[5];
    std::string multiPagescraperSettingsVectorOptions[4];
    std::string multiPagescraperSettingsVectorVectorPairStringsOptions[1];


    std::unordered_set<std::string> requiredScraperSettings;

    // list used for checking if settings in mutliPage are the same length ie frequency = channels etc. but also prefix= suffix which need to be equal but different number hence the list of vectors and not just a vector
    std::vector<std::vector<std::string>> sameLengthSettings;

    // functions for loading the scraperSettings in inc helpful ones.
    int singlePageLoad(std::pair<const std::string, std::list<ConfigClass::scraperSettings>>& key, const rapidjson::Value& object, std::map<std::string, std::list<ConfigClass::scraperSettings>>& threadAllocType);
    int multiPageLoad(std::pair<const std::string, std::list<ConfigClass::scraperSettings>>& key, const rapidjson::Value& object, std::map<std::string, std::list<ConfigClass::scraperSettings>>& threadAllocType);
    int checkMultiPage(std::pair<const std::string, std::list<ConfigClass::scraperSettings>>& key, const rapidjson::Value& object);
    int multiPageCheckArr(const rapidjson::Value& innerObject, const std::string& name, const int& arrSize);



    void loadNew(ConfigClass& newConfig);
    bool compareByName(const scraperSettings& a, const scraperSettings& b);
    std::string getInnerObjectAsString(const rapidjson::Value& object);


    // Add any lock-related declarations here

    // string rawConfig; // This seems to be a member variable, not a function
public:
    // ConfigClass();

    std::map<std::string, std::string> stringConfigs;
    std::map<std::string, int> intConfigs;
    std::map<std::string, std::vector<std::string>> stringArrConfigs;
    std::map<std::string, long> longConfigs;

    std::map<std::string, int> Threadmap;
    int actualManualThreads;
    int actualAutoThreads;



    ConfigClass(); // Constructor declarationF

    // raw String of config file, used for initial .equals checks once written
    std::string rawConfig; // Member variable declaration

    bool singlePage;
    bool multiPage;

    // all scraperSettings so singlePage and multiPage are the possible keys, and the lsit is scraperSettings in their generated form ie multiPage has been format already.
    std::map<std::string, std::list<scraperSettings>> scraperSettingsAll;
    std::map<std::string, std::list<scraperSettings>> threadAllocType;

    std::map<std::string, std::vector<std::list<scraperSettings>>> splitConfigs;


    // entire config in json format.
    rapidjson::Document config;

    void isStringArray(const rapidjson::Value& element, bool& output, std::vector<std::string>& entries);

    void isIntArray(const rapidjson::Value& element, bool& output, std::vector<int>& entries);

    // functions for importnig the various settings top ones being general settings with JSON import being the scraper settings which is more involved due to formatting.
    void configImportStrings(std::map<std::string, std::string>&);
    void configImportints(std::map<std::string, int>&);
    void configImportlongs(std::map<std::string, long>&);
    void configImportStringArr(std::map<std::string, std::vector<std::string>>&);
    void configImportJSON(std::map<std::string, std::list<scraperSettings>>&, std::map<std::string, std::list<scraperSettings>>&);
    int parseObject(const rapidjson::Value&, scraperSettings&);
    void replaceString(std::string& link, std::string& keyword, std::string& delim);

    void splitConfig(std::map<std::string, std::list<scraperSettings>>& input, int& autoThreads, int& manualThreads, std::map<std::string, std::vector<std::list<scraperSettings>>>& output);

    // function that it used for other functions to see if Config has finished constructing itself.
    bool ready();














};

#endif // CONFIGCLASS_H