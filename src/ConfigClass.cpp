#include <iostream>
#include <string>
#include <fstream>
#include "ConfigClass.h"
#include <map>
#include <list>
#include <algorithm>
#include <cmath> // For std::ceil

#include <iostream>
#include <string>

// using namespace std;
using namespace rapidjson;

ConfigClass::ConfigClass()
    : singlePagescraperSettingsStringOptions{"link", "channel", "channelTelegram", "filter", "replace", "browser", "requestType", "headers","pythonScriptName","pyFunctionName"}, // doesnt't include name as that'll come from key not element inside json
      singlePagescraperSettingsIntOptions{"frequency", "offset", "offsetMax", "timeout", "maxOutput", "threadAllocType"},
      singlePagescraperSettingsVectorOptions{"prefix", "suffix", "functions", "selectors"},
      singlePagescraperSettingsVectorVectorPairStringsOptions{"botDetector"},

      // multiPagescraperSettingsSingleStringOptions{"link"},
      multiPagescraperSettingsSingleIntOptions{"timeout"},
      multiPagescraperSettingsSingleStringOptions{"requestType","pythonScriptName","pyFunctionName"},

      multiPagescraperSettingsStringOptions{"keywords", "channel", "channelTelegram", "filter", "replace", "browser", "headers"}, // Need to handle link better. // doesnt't include name as that'll come from key not element inside json
      multiPagescraperSettingsIntOptions{"frequency", "offset", "offsetMax", "maxOutput", "threadAllocType"},
      multiPagescraperSettingsVectorOptions{"prefix", "suffix", "functions", "selectors"},
      multiPagescraperSettingsVectorVectorPairStringsOptions{"botDetector"},

      requiredScraperSettings{"link", "frequency", "channelTelegram", "channel", "offset", "offsetMax", "maxOutput", "timeout", "functions", "selectors"}
// need another requiredScraperSettings for keywords in multiPage. maybe just add an and given it's only one.

// split these out for multi as some do become multiple some stay the same

{

    sameLengthSettings.push_back({"frequency", "channelTelegram", "offset", "offsetMax", "maxOutput", "channel", "keywords", "filter", "headers"});
    sameLengthSettings.push_back({"prefix", "suffix", "functions", "selectors"});
    // setting default vars.
    rawConfig = "";

    // read config file
    try
    {

        // move this to seperate function as might want to make one from another sources.
        std::ifstream file("config.json");
        if (!file.is_open())
        {
            std::cout << "hereE" << std::endl;
            throw std::runtime_error("Could not open config file");
        }
        // Read the file content into a string
        rawConfig = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // Close the file
        file.close();
    }
    catch (const std::exception &e)
    {
        // Handle exceptions properly, maybe log or throw
        std::cout << "here2" << std::endl;
        throw std::runtime_error("Could not open config file due to: " + std::string(e.what()));
    }

    if (rawConfig != "")
    {

        config.Parse(rawConfig.c_str());
    }
    std::cout << "here1" << std::endl;

    // setting up default values;
    this->stringConfigs["VPNrebootCommand"] = "";
   // this->stringConfigs["VPNsinglePage_rebootscript"] = "";
    this->stringConfigs["dataOutputCommand"] = "";
    this->stringConfigs["releaseDateMode"] = "";
    this->stringConfigs["discord_token"] = "";
    this->stringConfigs["telegram_token"] = "";
    this->stringConfigs["moderator_channel"] = "";
    this->stringConfigs["webServer"] = "";
    this->stringConfigs["pageTitle"] = "";
    this->stringConfigs["telegram_moderator_channel"] = "";
    this->stringConfigs["telegram_user"] = "";
    this->stringConfigs["stealthMode"] = "";

    this->intConfigs["autoThreads"] = 1;
    this->intConfigs["manualThreads"] = 1;
    this->intConfigs["port"] = 8080;

    this->longConfigs["discord_guildID"] = 0L;
    this->scraperSettingsAll["singlePage"]; // all settings split by multi or single //these dont need to be class vars anymore. they can be temp vars
    this->scraperSettingsAll["multiPage"];  // all settings split by multi or single //these dont need to be class vars anymore. they can be temp vars
    this->threadAllocType["auto"];          // all settings but split by manual auto
    this->threadAllocType["manual"];        // all settings but split by manual auto
    this->splitConfigs["manual"];           // threadsplit
    this->splitConfigs["auto"];             // threadplsit

    this->stringArrConfigs["VPNmultiPage"] = {""};
    this->stringArrConfigs["VPNsinglePage"] = {""};

    this->stringArrConfigs["schedule_start"] = {"07:00"};
    this->stringArrConfigs["schedule_stop"] = {"22:00"};
    this->stringArrConfigs["discord_guildIDs"] = {""};

    // ConfigClassmainConfig;
    // std::cout << "hello world" << std::endl;

    // loading actual data into memory, need to check what happens if there is no data, to make sure above defaults remain in place.

    configImportStrings(stringConfigs);
    configImportStringArr(stringArrConfigs);
    configImportlongs(longConfigs);
    configImportints(intConfigs);

    configImportJSON(scraperSettingsAll, threadAllocType);

    // ifConfig equals to test i'd say stop here and then we can just use scraperSettingsAll["singlePage"] spliced together with "multiPage" and pass that into a scraper. The rest isnt needed.
    /*
        if (scraperSettingsAll["singlePage"].empty() && scraperSettingsAll["multiPage"].empty())
        {

            std::cout << "Error empty scraper config" << std::endl; // make this ok so that people can start with no scrapers and add with ui.
        }
        else if (scraperSettingsAll["singlePage"].empty())
        {
            singlePage = false;
        }
        else if (!scraperSettingsAll["singlePage"].empty())
        {

            singlePage = true;
        }

        if (scraperSettingsAll["singlePage"].empty() && scraperSettingsAll["multiPage"].empty())
        {

            std::cout << "Error empty scraper config" << std::endl; // make this ok so that people can start with no scrapers and add with ui.
        }
        else if (scraperSettingsAll["multiPage"].empty())
        {
            multiPage = false;
        }
        else if (!scraperSettingsAll["multiPage"].empty())
        {

            multiPage = true;
        }
        */

    // if singlePage missing set singlePage = false;
    // similarly for multiPage

    splitConfig(threadAllocType, this->intConfigs["autoThreads"], this->intConfigs["manualThreads"], splitConfigs);

    // Useful debug printers

    // prints config to check it.
    /*
    for (const auto &pair : stringConfigs)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    for (const auto &pair : intConfigs)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    for (const auto &pair : longConfigs)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    for (const auto &pair : stringArrConfigs)
    {
        std::cout << pair.first << ":" << std::endl;
        for (const auto &innerVector : pair.second)
        {
            for (const auto &element : innerVector)
            {
                std::cout << element << " ";
            }
            std::cout << std::endl; // Print new line after each inner vector
        }
    }

    for (const auto &settings : scraperSettingsAll["singlePage"])
    {
        std::cout << "Name: " << settings.name << std::endl;

        std::cout << "String Settings:" << std::endl;
        for (const auto &pair : settings.stringSettings)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }

        std::cout << "Int Settings:" << std::endl;
        std::cout << settings.intSettings.size() << std::endl;
        for (const auto &pair : settings.intSettings)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }

        std::cout << "Vector Settings:" << std::endl;
        for (const auto &pair : settings.vectorSettings)
        {
            std::cout << pair.first << ":" << std::endl;
            for (const auto &element : pair.second)
            {
                std::cout << "  - " << element << std::endl;
            }
        }

        std::cout << std::endl; // Add a blank line between objects for clarity
    }

    for (const auto &settings : scraperSettingsAll["multiPage"])
    {
        std::cout << "Name: " << settings.name << std::endl;

        std::cout << "String Settings:" << std::endl;
        for (const auto &pair : settings.stringSettings)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }

        std::cout << "Int Settings:" << std::endl;
        std::cout << settings.intSettings.size() << std::endl;
        for (const auto &pair : settings.intSettings)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }

        std::cout << "Vector Settings:" << std::endl;
        for (const auto &pair : settings.vectorSettings)
        {
            std::cout << pair.first << ":" << std::endl;
            for (const auto &element : pair.second)
            {
                std::cout << "  - " << element << std::endl;
            }
        }
std::cout << std::endl; // Add a blank line between objects for claritystd::cout << "done" << std::endl; // Add a blank line between objects for clarity

    }

*/

    /*
        for (const auto& settings : splitConfigs["auto"][0])
        {
            std::cout << "Name: " << settings.name << std::endl;

            std::cout << "String Settings:" << std::endl;
            for (const auto& pair : settings.stringSettings)
            {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }

            std::cout << "Int Settings:" << std::endl;
            std::cout << settings.intSettings.size() << std::endl;
            for (const auto& pair : settings.intSettings)
            {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }

            std::cout << "Vector Settings:" << std::endl;
            for (const auto& pair : settings.vectorSettings)
            {
                std::cout << pair.first << ":" << std::endl;
                for (const auto& element : pair.second)
                {
                    std::cout << "  - " << element << std::endl;
                }
            }
            std::cout << std::endl; // Add a blank line between objects for claritystd::cout << "done" << std::endl; // Add a blank line between objects for clarity
        }

        std::cout << "done" << std::endl; // Add a blank line between objects for clarity
        */
}

//------------------------import Functions------------------------
void ConfigClass::configImportStrings(std::map<std::string, std::string> &keys)
{
    std::cout << "here" << std::endl;

    for (std::map<std::string, std::string>::value_type &key : keys)
    {

        if (config.HasMember(key.first.c_str()))
        {

            if (config[key.first.c_str()].IsString())
            {

                keys[key.first] = std::move(config[key.first.c_str()].GetString());
            }
            else
            {

                std::cout << "Config error" << key.first << "need to be a string" << std::endl;
            }
        }
    }
}

void ConfigClass::configImportints(std::map<std::string, int> &keys)
{

    for (std::map<std::string, int>::value_type &key : keys)
    {

        if (config.HasMember(key.first.c_str()))
        {

            if (config[key.first.c_str()].IsInt())
            {

                keys[key.first] = std::move(config[key.first.c_str()].GetInt());
            }
            else
            {

                std::cout << "Config error" << key.first << "need to be a int" << std::endl;
            }
        }
    }
}

void ConfigClass::configImportlongs(std::map<std::string, long> &keys)
{

    for (std::map<std::string, long>::value_type &key : keys)
    {

        if (config.HasMember(key.first.c_str()))
        {

            if (config[key.first.c_str()].IsInt64())
            {

                keys[key.first] = std::move(config[key.first.c_str()].GetInt64());
            }
            else
            {

                std::cout << "Config error" << key.first << "need to be a Long" << std::endl;
            }
        }
    }
}

void ConfigClass::configImportStringArr(std::map<std::string, std::vector<std::string>> &keys)
{

    for (std::map<std::string, std::vector<std::string>>::value_type &key : keys)
    {

        std::vector<std::string> &value = key.second;
        if (config.HasMember(key.first.c_str()))
        {

            if (config[key.first.c_str()].IsArray())

            {

                const rapidjson::Value &jsonArray = config[key.first.c_str()];

                for (rapidjson::SizeType x = 0; x < jsonArray.Size(); ++x)
                {
                    value.push_back(jsonArray[x].GetString());
                }
            }
            else
            {

                std::cout << "Config error" << key.first << "need to be a String Array" << std::endl;
            }
        }
    }
}

void ConfigClass::configImportJSON(std::map<std::string, std::list<scraperSettings>> &keys, std::map<std::string, std::list<scraperSettings>> &threadAllocType)
{
    for (auto &key : keys)
    {
        if (config.HasMember(key.first.c_str()))
        {
            const rapidjson::Value &outerObject = config[key.first.c_str()];

            if (outerObject.IsObject() && key.first == "singlePage")

            {

                singlePageLoad(key, outerObject, threadAllocType);
            }
            else if (outerObject.IsObject() && key.first == "multiPage")
            {

                multiPageLoad(key, outerObject, threadAllocType);
            }
            else
            {
                std::cout << "Config error " << key.first << " needs to be a valid object." << std::endl;
            }
        }
    }

    // looping both again to find any duplicate names
    std::list<std::string> settingsNames;
    for (auto &key : scraperSettingsAll)
    {

        if (!key.second.empty())
        {

            for (auto &setting : key.second)
            {

                auto it = std::find(settingsNames.begin(), settingsNames.end(), setting.name);

                // If element is not found, add it to the list
                if (it == settingsNames.end())
                {
                    settingsNames.push_back(setting.name);
                    std::cout << "Added element " << setting.name << " to the list." << std::endl;
                }
                else
                {
                    std::cout << "Element " << setting.name << " already exists, please ensure no duplicate names are used!" << std::endl;
                }
            }
        }
    }
}

//------------------------Ready check functions------------------------

bool ConfigClass::ready()
{

    if (config.Empty() || !importComplete.load())
    {

        return false;
    }
    else if (importComplete.load())
    {

        return true;
    }
    else
    {

        return false;
    }
}

//------------------------Scraper Setting memory Loader------------------------

int ConfigClass::singlePageLoad(std::pair<const std::string, std::list<ConfigClass::scraperSettings>> &key, const rapidjson::Value &outerObject, std::map<std::string, std::list<ConfigClass::scraperSettings>> &threadAllocType)
{

    for (auto it = outerObject.MemberBegin(); it != outerObject.MemberEnd(); ++it)
    {
        const std::string &innerObjectName = it->name.GetString();
        // const bool& innerObjectEnabled = it->enabled.GetBool();
        const rapidjson::Value &innerObject = it->value;

        if (innerObject.IsObject() && ((innerObject.HasMember("enabled") && innerObject["enabled"].IsBool() && innerObject["enabled"].GetBool()) || (!innerObject.HasMember("enabled"))))
        {
            scraperSettings setting;

            setting.name = innerObjectName;
            if (parseObject(innerObject, setting) == 1)
            {

                // return 1;
            }
            else
            {

                std::cout << "Error loading: " << setting.name << std::endl;
            }

            // can never change this to and as it->second might cause issues if first is true
            key.second.push_back(setting);

            // if threadAllocType is included and not equal to -1 then auto allocate threads otherwise do it manually.
            if (setting.intSettings.find("threadAllocType") != setting.intSettings.end() && setting.intSettings["threadAllocType"] != -1)
            {

                threadAllocType["manual"].push_back(setting);
            }
            else
            {
                std::cout << setting.name << std::endl;
                threadAllocType["auto"].push_back(setting);
            }
        }
        else if (((innerObject.HasMember("enabled") && innerObject["enabled"].IsBool() && innerObject["enabled"].GetBool()) || (!innerObject.HasMember("enabled"))))

        {

            std::cout << "Skipping " << innerObjectName << "as disabled" << std::endl;
        }
        else
        {
            std::cout << innerObjectName << " is not an object!" << std::endl;
            return 1;
        }
        return 0;
    }
    return 0;
}

int ConfigClass::multiPageLoad(std::pair<const std::string, std::list<ConfigClass::scraperSettings>> &key, const rapidjson::Value &outerObject, std::map<std::string, std::list<ConfigClass::scraperSettings>> &threadAllocType)
{

    const std::string &keyString = key.first;

    for (auto it = outerObject.MemberBegin(); it != outerObject.MemberEnd(); ++it)

    {
        const std::string &innerObjectName = it->name.GetString();
        const rapidjson::Value &innerObject = it->value;

        // skips if enabled is missing or false.
        if (innerObject.IsObject() && ((innerObject.HasMember("enabled") && innerObject["enabled"].IsBool() && innerObject["enabled"].GetBool()) || (!innerObject.HasMember("enabled"))))
        {

            int arrSize = 0;
            multiPageCheckArr(innerObject, innerObjectName, arrSize);

            std::vector<scraperSettings> currentSiteSettings;
            // //not going to call external function to check the types as I'll add them to the settings file as I'm going rather than checking then looping then checking again.

            bool stringChecks = false;
            bool intChecks = false;
            bool vectorChecks = false;

            // the below is equivalent to singlePage's parse method just more advanced for  the multiple lines.

            for (int x = 0; x < sizeof(multiPagescraperSettingsStringOptions) / sizeof(multiPagescraperSettingsStringOptions[0]); x++)
            {

                bool elementIsStringArr;
                std::vector<std::string> stringEntries;

                if (innerObject.HasMember(multiPagescraperSettingsStringOptions[x].c_str()))
                {

                    isStringArray(innerObject[multiPagescraperSettingsStringOptions[x].c_str()], elementIsStringArr, stringEntries);
                }
                if (innerObject.HasMember(multiPagescraperSettingsStringOptions[x].c_str()) && elementIsStringArr)
                {

                    stringChecks = true; // not needed anymore as assuming true and function exits if not.

                    for (int i = 0; i < stringEntries.size(); i++)
                    {

                        if (i >= currentSiteSettings.size())
                        {
                            ConfigClass::scraperSettings temp;
                            currentSiteSettings.push_back(temp);
                        }

                        if (currentSiteSettings[i].name.empty())
                        {
                            currentSiteSettings[i].name = std::to_string(i) + "_" + innerObjectName;
                        }
                        // need to change to pushback if doesnt exist.
                        // dont need to worry about being 2 away as loop always iterates up by 1.
                        std::string tlink = "link";
                        std::string tdata = "requestData";
                        // need some more error checking here.

                        std::vector<std::string> RequestData;
                        // need to add a check for string array
                        if (multiPagescraperSettingsStringOptions[x] == "keywords" && innerObject.HasMember(multiPagescraperSettingsStringOptions[x].c_str()) && innerObject.HasMember(tlink.c_str()) && innerObject[tlink.c_str()].IsString())
                        {

                            std::string link = innerObject[tlink.c_str()].GetString();

                            std::string delim = "::::";
                            replaceString(link, stringEntries[i], delim);

                            currentSiteSettings[i].stringSettings[tlink] = link;

                            // it's stringArray need some more checks here. ie need to check it's a string array and of the right size etc if not done above.

                            if (innerObject.HasMember(tdata.c_str()))
                            {

                                // std::string requestData = innerObject[tdata.c_str()].GetString();

                                std::string requestData = innerObject[tdata.c_str()].GetString();

                                std::string delim = "::::";
                                replaceString(requestData, stringEntries[i], delim);

                                currentSiteSettings[i].stringSettings[tdata] = requestData;
                            }
                        }
                        else
                        {
                            currentSiteSettings[i].stringSettings[multiPagescraperSettingsStringOptions[x]] = stringEntries[i];
                        }
                    }
                }
                else
                {

                    // check the setting is actually needed and not just optional.
                    if (requiredScraperSettings.find(multiPagescraperSettingsStringOptions[x]) != requiredScraperSettings.end())
                    {

                        std::cout << "Error with:  " << multiPagescraperSettingsStringOptions[x] << " Is either missing or of the wrong type, please check settings" << std::endl;
                        stringChecks = false;
                        return 1;
                    }
                }
            }

            for (int x = 0; x < sizeof(multiPagescraperSettingsSingleStringOptions) / sizeof(multiPagescraperSettingsSingleStringOptions[0]); x++)
            {

                bool elementIsIntgArr;
                std::vector<int> IntEntries;
                isIntArray(innerObject["frequency"], elementIsIntgArr, IntEntries); // catch error here for missing frequency, think i'd have already flagged but just double check.

                if (innerObject.HasMember(multiPagescraperSettingsSingleStringOptions[x].c_str()) && innerObject[multiPagescraperSettingsSingleStringOptions[x].c_str()].IsString())
                {

                    stringChecks = true; // not needed anymore as assuming true and function exits if not.

                    for (int i = 0; i < IntEntries.size(); i++) // this loop is what needs changed

                    {

                        if (i >= currentSiteSettings.size())
                        {
                            ConfigClass::scraperSettings temp;
                            currentSiteSettings.push_back(temp);
                        }

                        if (currentSiteSettings[i].name.empty())
                        {
                            currentSiteSettings[i].name = std::to_string(i) + "_" + innerObjectName;
                        }

                        currentSiteSettings[i].stringSettings[multiPagescraperSettingsSingleStringOptions[x]] = innerObject[multiPagescraperSettingsSingleStringOptions[x].c_str()].GetString();

                        // currentSiteSettings[y].stringSettings[multiPagescraperSettingsStringOptions[x]] = innerObject[multiPagescraperSettingsSingleStringOptions[x].c_str()].GetString();

                        //(currentSiteSettings[y].stringSettings[multiPagescraperSettingsSingleStringOptions[x]]) = innerObject[multiPagescraperSettingsSingleStringOptions[x].c_str()];

                        // y++;
                    }
                }
                else
                {

                    // check the setting is actually needed and not just optional.
                    if (requiredScraperSettings.find(multiPagescraperSettingsSingleStringOptions[x]) != requiredScraperSettings.end())
                    {

                        std::cout << "Error with:  " << multiPagescraperSettingsSingleStringOptions[x] << " Is either missing or of the wrong type, please check settings" << std::endl;
                        stringChecks = false;
                        return 1;
                    }
                }
            }

            //    y = 0;
            for (int x = 0; x < sizeof(multiPagescraperSettingsIntOptions) / sizeof(multiPagescraperSettingsIntOptions[0]); x++)
            {

                bool elementIsIntArr;
                std::vector<int> intEntries;

                if (innerObject.HasMember(multiPagescraperSettingsIntOptions[x].c_str()))
                {
                    isIntArray(innerObject[multiPagescraperSettingsIntOptions[x].c_str()], elementIsIntArr, intEntries);
                }

                if (innerObject.HasMember(multiPagescraperSettingsIntOptions[x].c_str()) && elementIsIntArr)
                {

                    intChecks = true; // not needed anymore as assuming true and function exits if not.
                    // intSettings[scraperSettingsIntOptions[x]] = intEntries;

                    for (int i = 0; i < intEntries.size(); i++)
                    { // dont really need i swap to iterator.

                        if (i >= currentSiteSettings.size())
                        {
                            ConfigClass::scraperSettings temp;
                            temp.intSettings[multiPagescraperSettingsIntOptions[x]] = intEntries[i];
                            ;
                            currentSiteSettings.push_back(temp);
                        }
                        else
                        {
                            currentSiteSettings[i].intSettings[multiPagescraperSettingsIntOptions[x]] = intEntries[i];
                        }

                        // currentSiteSettings[i].intSettings[multiPagescraperSettingsIntOptions[x]] = intEntries[i];

                        // y++;
                    }
                }
                else
                {

                    // check the setting is actually needed and not just optional.
                    if (requiredScraperSettings.find(multiPagescraperSettingsIntOptions[x]) != requiredScraperSettings.end())
                    {

                        std::cout << "Error with:  " << multiPagescraperSettingsIntOptions[x] << " Is either missing or of the wrong type, please check settings" << std::endl;
                        intChecks = false;
                        return 1;
                    }
                }
            }

            for (int x = 0; x < sizeof(multiPagescraperSettingsSingleIntOptions) / sizeof(multiPagescraperSettingsSingleIntOptions[0]); x++)
            {

                bool elementIsIntArr;
                std::vector<int> intEntries;
                isIntArray(innerObject["frequency"], elementIsIntArr, intEntries);

                if (innerObject.HasMember(multiPagescraperSettingsSingleIntOptions[x].c_str()) && innerObject[multiPagescraperSettingsSingleIntOptions[x].c_str()].IsInt())
                {

                    intChecks = true; // not needed anymore as assuming true and function exits if not.
                    // intSettings[scraperSettingsIntOptions[x]] = intEntries;

                    for (int i = 0; i < intEntries.size(); i++)
                    { // dont really need i swap to iterator.

                        if (i >= currentSiteSettings.size())
                        {
                            ConfigClass::scraperSettings temp;
                            temp.intSettings[multiPagescraperSettingsSingleIntOptions[x]] = innerObject[multiPagescraperSettingsSingleIntOptions[x].c_str()].GetInt();
                            currentSiteSettings.push_back(temp);
                        }
                        else
                        {
                            currentSiteSettings[i].intSettings[multiPagescraperSettingsSingleIntOptions[x]] = innerObject[multiPagescraperSettingsSingleIntOptions[x].c_str()].GetInt();
                        }

                        // currentSiteSettings[i].intSettings[multiPagescraperSettingsIntOptions[x]] = intEntries[i];

                        // y++;
                    }
                }
                else
                {

                    // check the setting is actually needed and not just optional.
                    if (requiredScraperSettings.find(multiPagescraperSettingsIntOptions[x]) != requiredScraperSettings.end())
                    {

                        std::cout << "Error with:  " << multiPagescraperSettingsIntOptions[x] << " Is either missing or of the wrong type, please check settings" << std::endl;
                        intChecks = false;
                        return 1;
                    }
                }
            }

            for (int x = 0; x < sizeof(multiPagescraperSettingsVectorOptions) / sizeof(multiPagescraperSettingsVectorOptions[0]); x++)
            {
                std::cout << "testmiddle3 " << sizeof(multiPagescraperSettingsVectorOptions) << std::endl;

                bool elementIsStringArr2;
                bool elementIsIntArr;
                std::vector<int> intEntries;
                std::vector<std::string> vectorEntries;
                isIntArray(innerObject["frequency"], elementIsIntArr, intEntries);

                if (innerObject.HasMember(multiPagescraperSettingsVectorOptions[x].c_str()))
                {
                    isStringArray(innerObject[multiPagescraperSettingsVectorOptions[x].c_str()], elementIsStringArr2, vectorEntries);
                }

                if (innerObject.HasMember(multiPagescraperSettingsVectorOptions[x].c_str()) && elementIsStringArr2)
                {

                    vectorChecks = true; // not needed anymore as assuming true and function exits if not.
                    // vecSettings[scraperSettingsVectorOptions[x]] = vectorEntries;

                    for (int i = 0; i < intEntries.size(); i++)
                    { // dont really need i swap to iterator.

                        if (i >= currentSiteSettings.size())
                        {
                            ConfigClass::scraperSettings temp;
                            temp.vectorSettings[multiPagescraperSettingsVectorOptions[x]] = vectorEntries;
                            currentSiteSettings.push_back(temp);
                        }
                        else
                        {
                            currentSiteSettings[i].vectorSettings[multiPagescraperSettingsVectorOptions[x]] = vectorEntries;
                        }

                        //    currentSiteSettings[i].vectorSettings[multiPagescraperSettingsVectorOptions[x]] = vectorEntries;

                        // y++;
                    }
                }
                else
                {

                    // check the setting is actually needed and not just optional.
                    if (requiredScraperSettings.find(multiPagescraperSettingsVectorOptions[x]) != requiredScraperSettings.end())
                    {

                        std::cout << "Error with:  " << multiPagescraperSettingsVectorOptions[x] << " Is either missing or of the wrong type, please check settings" << std::endl;
                        vectorChecks = false;
                        return 1;
                    }
                }
            }

            //////////////New untested code here for botDetection inputs.

            for (int x = 0; x < sizeof(multiPagescraperSettingsVectorVectorPairStringsOptions) / sizeof(multiPagescraperSettingsVectorVectorPairStringsOptions[0]); x++)
            {

                bool elementIsIntgArr;
                std::vector<int> IntEntries;
                isIntArray(innerObject["frequency"], elementIsIntgArr, IntEntries); // catch error here for missing frequency, think i'd have already flagged but just double check.

                if (innerObject.HasMember(multiPagescraperSettingsVectorVectorPairStringsOptions[x].c_str()) && innerObject[multiPagescraperSettingsVectorVectorPairStringsOptions[x].c_str()].IsArray())
                {

                    stringChecks = true; // not needed anymore as assuming true and function exits if not.

                    const rapidjson::Value &array = innerObject[multiPagescraperSettingsVectorVectorPairStringsOptions[x].c_str()];
                    std::vector<std::pair<std::string, std::string>> tempVector;

                    for (rapidjson::SizeType i = 0; i < array.Size(); ++i)
                    {

                        if (array[i].IsArray() && array.Size() != 2)
                        {

                            const Value &firstElement = array[i][0];
                            const Value &secondElement = array[i][2];
                            std::pair<std::string, std::string> tempPair;
                            tempPair.first = firstElement.GetString();
                            tempPair.second = secondElement.GetString();

                            tempVector.push_back(tempPair);
                        }
                        else
                        {
                            std::cout << "Array not correct format." << std::endl;
                            tempVector.clear();
                            break;
                        }
                    }

                    for (int i = 0; i < IntEntries.size(); i++) // this loop is what needs changed

                    {

                        if (i >= currentSiteSettings.size())
                        {
                            ConfigClass::scraperSettings temp;
                            currentSiteSettings.push_back(temp);
                        }

                        if (currentSiteSettings[i].name.empty())
                        {
                            currentSiteSettings[i].name = std::to_string(i) + "_" + innerObjectName;
                        }

                        currentSiteSettings[i].vectorVectorPair[multiPagescraperSettingsVectorVectorPairStringsOptions[x]] = tempVector;

                        // currentSiteSettings[y].stringSettings[multiPagescraperSettingsStringOptions[x]] = innerObject[multiPagescraperSettingsSingleStringOptions[x].c_str()].GetString();

                        //(currentSiteSettings[y].stringSettings[multiPagescraperSettingsSingleStringOptions[x]]) = innerObject[multiPagescraperSettingsSingleStringOptions[x].c_str()];

                        // y++;
                    }
                }
                else
                {

                    // check the setting is actually needed and not just optional.
                    if (requiredScraperSettings.find(multiPagescraperSettingsVectorVectorPairStringsOptions[x]) != requiredScraperSettings.end())
                    {

                        std::cout << "Error with:  " << multiPagescraperSettingsVectorVectorPairStringsOptions[x] << " Is either missing or of the wrong type, please check settings" << std::endl;
                        stringChecks = false;
                        return 1;
                    }
                }
            }

            // Need to do the above loops for vector of vector<strings> (unless i think of a new format for headers)
            // doesnt add to config if it's disabled. Decent optimisation by doing this at the start but can think of clean way of doing it preparsing.

            key.second.insert(key.second.end(), currentSiteSettings.begin(), currentSiteSettings.end());

            // above adds to total settings list the below splits out into the two thread types
            for (auto &setting : currentSiteSettings)
            {

                if (setting.intSettings.find("threadAllocType") != setting.intSettings.end() && setting.intSettings["threadAllocType"] != -1)
                {

                    // THINK THIS IS WRONG AND SHOULD INSTEAD BE
                    //  splitConfigs["manual"].pushback(setting);
                    threadAllocType["manual"].push_back(setting);
                }
                else
                {
                    std::cout << setting.name << std::endl;
                    // splitConfigs["auto"].pushback(setting);
                    threadAllocType["auto"].push_back(setting);
                }
            }
            //  std::string sN = util::split(currentSiteSettings[0].name,"_")[1];
            // std::cout << "Skipping " << sN << "as disabled" << std::endl; //edit this to output side name. will need to strip the numbers off . ie 0_sitename
        }
        else if (((innerObject.HasMember("enabled") && innerObject["enabled"].IsBool() && innerObject["enabled"].GetBool()) || (!innerObject.HasMember("enabled"))))

        {

            std::cout << "Skipping " << innerObjectName << "as disabled" << std::endl;
        }

        else
        {
            std::cout << innerObjectName << " is not an object!" << std::endl;
            return 1;
        }
        // return 0;
    }
    return 0;
}

//------------------------Userful functions------------------------

// Used to parse singlePage content from JSON file into a struct for faster access.
int ConfigClass::parseObject(const rapidjson::Value &object, scraperSettings &setting)

{

    bool stringChecks = false;
    bool intChecks = false;
    bool vectorChecks = false;
    for (const std::string &stringSetting : singlePagescraperSettingsStringOptions)
    {

        if (object.HasMember(stringSetting.c_str()) && object[stringSetting.c_str()].IsString())
        {
            setting.stringSettings[stringSetting] = object[stringSetting.c_str()].GetString();
        }
        else
        {

            // Error with setting stringSetting either missing or in wrong format

            if (requiredScraperSettings.find(stringSetting) != requiredScraperSettings.end())
            {

                std::cout << "Error with:  " << stringSetting << " Is either missing or of the wrong type, please check settings" << std::endl;
                stringChecks = false;
                return 1;
            }
            // return 1;
        }
    }

    for (const std::string &intSetting : singlePagescraperSettingsIntOptions)
    {

        if (object.HasMember(intSetting.c_str()) && object[intSetting.c_str()].IsInt())
        {
            setting.intSettings[intSetting] = object[intSetting.c_str()].GetInt();
        }
        else
        {

            if (requiredScraperSettings.find(intSetting) != requiredScraperSettings.end())
            {

                std::cout << "Error with:  " << intSetting << " Is either missing or of the wrong type, please check settings" << std::endl;
                intChecks = false;
                return 1;
            }
            // Error with setting stringSetting either missing or in wrong format
        }
    }

    std::cout << "test" << sizeof(singlePagescraperSettingsVectorOptions) / sizeof(singlePagescraperSettingsVectorOptions[0]) << std::endl;

    for (int i = 0; i < sizeof(singlePagescraperSettingsVectorOptions) / sizeof(singlePagescraperSettingsVectorOptions[0]); ++i)
    {
        const std::string &vecSetting = singlePagescraperSettingsVectorOptions[i];

        std::cout << "test duringstart " << i << " " << sizeof(singlePagescraperSettingsVectorOptions) / sizeof(singlePagescraperSettingsVectorOptions[0]) << std::endl;
        if (object.HasMember(vecSetting.c_str()) && object[vecSetting.c_str()].IsArray())
        {
            const rapidjson::Value &array = object[vecSetting.c_str()];
            std::vector<std::string> tempVector;

            for (rapidjson::SizeType i = 0; i < array.Size(); ++i)
            {
                if (array[i].IsString())
                {
                    tempVector.push_back(array[i].GetString());
                }
                else
                {
                    std::cout << "Array contains non-string elements." << std::endl;
                    tempVector.clear();
                    break;
                }
            }

            setting.vectorSettings[vecSetting] = tempVector;
        }
        else
        {

            if (requiredScraperSettings.find(vecSetting) != requiredScraperSettings.end())
            {

                std::cout << "Error with:  " << vecSetting << " Is either missing or of the wrong type, please check settings" << std::endl;
                vectorChecks = false;
                return 1;
            }
            // Error with setting stringSetting either missing or in wrong format
        }

        std::cout << "test duringend" << i << " " << sizeof(singlePagescraperSettingsVectorOptions) / sizeof(singlePagescraperSettingsVectorOptions[0]) << std::endl;
    }

    //////////////////New untested code here for botDetection inputs.

    for (int i = 0; i < sizeof(singlePagescraperSettingsVectorVectorPairStringsOptions) / sizeof(singlePagescraperSettingsVectorVectorPairStringsOptions[0]); ++i)
    {
        const std::string &vecSetting = singlePagescraperSettingsVectorVectorPairStringsOptions[i];

        std::cout << "test duringstart " << i << " " << sizeof(singlePagescraperSettingsVectorVectorPairStringsOptions) / sizeof(singlePagescraperSettingsVectorVectorPairStringsOptions[0]) << std::endl;
        if (object.HasMember(vecSetting.c_str()) && object[vecSetting.c_str()].IsArray())
        {
            const rapidjson::Value &array = object[vecSetting.c_str()];
            std::vector<std::pair<std::string, std::string>> tempVector;

            for (rapidjson::SizeType i = 0; i < array.Size(); ++i)
            {

                if (array[i].IsArray() && array.Size() != 2)
                {

                    const Value &firstElement = array[i][0];
                    const Value &secondElement = array[i][2];
                    std::pair<std::string, std::string> tempPair;
                    tempPair.first = firstElement.GetString();
                    tempPair.second = secondElement.GetString();

                    tempVector.push_back(tempPair);
                }
                else
                {
                    std::cout << "Array not correct format." << std::endl;
                    tempVector.clear();
                    break;
                }
            }

            setting.vectorVectorPair[vecSetting] = tempVector;
        }
        else
        {

            if (requiredScraperSettings.find(vecSetting) != requiredScraperSettings.end())
            {

                std::cout << "Error with:  " << vecSetting << " Is either missing or of the wrong type, please check settings" << std::endl;
                vectorChecks = false;
                return 1;
            }
            // Error with setting stringSetting either missing or in wrong format
        }

        std::cout << "test duringend" << i << " " << sizeof(singlePagescraperSettingsVectorVectorPairStringsOptions) / sizeof(singlePagescraperSettingsVectorVectorPairStringsOptions[0]) << std::endl;
    }

    return 0;
}

// Checks the length of arrays in multiPage config to make sure they're the same length. If not an error returned
int ConfigClass::multiPageCheckArr(const rapidjson::Value &innerObject, const std::string &name, const int &arrSize)
{

    for (auto &it : sameLengthSettings)
    {
        int sameLengthSettingChecker = -1;

        // int counter = 0;
        for (const std::string &itception : it)
        {

            if (innerObject.IsObject() && innerObject.HasMember(itception.c_str()) && innerObject[itception.c_str()].IsArray())
            {

                if (sameLengthSettingChecker == -1)
                {
                    sameLengthSettingChecker = static_cast<int>(innerObject[itception.c_str()].Size());
                }
                else
                {

                    if (sameLengthSettingChecker != static_cast<int>(innerObject[itception.c_str()].Size()))
                    {

                        std::cout << "Error with config entry, incorrect number of enteries for: " << itception << " in " << name << std::endl;
                        return 1;
                    }
                }
            }
            else if (requiredScraperSettings.find(itception) != requiredScraperSettings.end())
            {

                std::cout << name << ": " << itception << " is either missing or not in array format " << name << std::endl;
                return 1;
            }
            else
            {

                std::cout << "Error with config entry: " << itception << " in " << name << std::endl;
                //  return 1;
            }
        }
    }
    return 0;
}
void ConfigClass::isStringArray(const rapidjson::Value &element, bool &output, std::vector<std::string> &entries)
{
    output = true;
    try
    {
        for (rapidjson::SizeType i = 0; i < element.Size(); ++i)
        {
            if (!element[i].IsString())
            {
                output = false;
                break;
            }
            else
            {

                entries.push_back(element[i].GetString());
            }
        }
    }
    catch (const std::exception &e)
    {

        output = false;
    }
}

void ConfigClass::isIntArray(const rapidjson::Value &element, bool &output, std::vector<int> &entries)
{
    output = true;

    try
    {
        for (rapidjson::SizeType i = 0; i < element.Size(); ++i)
        {
            if (!element[i].IsInt())
            {
                output = false;
                break;
            }
            else
            {

                entries.push_back(element[i].GetInt());
            }
        }
    }
    catch (std::exception e)
    {

        output = false;
    }
}

void ConfigClass::replaceString(std::string &link, std::string &keyword, std::string &delim)
{

    size_t pos = 0;
    while ((pos = link.find(delim, pos)) != std::string::npos)
    {
        link.replace(pos, delim.length(), keyword);
        pos += keyword.length(); // Move past the replaced part
    }
}

void ConfigClass::splitConfig(std::map<std::string, std::list<scraperSettings>> &input, int &autoThreads, int &manualThreads, std::map<std::string, std::vector<std::list<scraperSettings>>> &output)
{

    // looping to go through manual and auto sitesSettings
    for (auto &key : input)
    {

        if (!key.second.empty())
        {

            if (key.first == "manual" && !key.second.empty())
            {

                actualManualThreads = 0; // looping the settings to check to get the actualNUmber of threads. If actual number used is less than  update the actuals.
                for (auto &setting : key.second)
                {
                    if (setting.intSettings["threadAllocType"] < this->intConfigs["autoThreads"] && setting.intSettings["threadAllocType"] > actualManualThreads)
                    {
                        actualManualThreads = setting.intSettings["threadAllocType"];
                    }
                    else if (setting.intSettings["threadAllocType"] > this->intConfigs["autoThreads"])
                    {

                        std::cout << "Error not enough manual threads setup in settings, please adjust this setting" << std::endl;
                    }
                }

                // check this plus 1.
                if (actualManualThreads + 1 <= intConfigs["manualThreads"])
                {
                    output["manual"].resize(actualManualThreads + 1);

                    for (const auto &setting : key.second)
                    {
                        int temp = setting.intSettings.at("threadAllocType");
                        output["manual"][temp].push_back(setting);
                    }
                }
                else
                {

                    std::cout << "Error not enough manualThreads available please expand in config" << std::endl;
                }

                //  output["manual"].push_back(tempList)
            }
            else
            {

                std::list<scraperSettings> &config = key.second;
                // 123 issues here!!
                int numberSettings = config.size();
                int perThread = std::ceil(static_cast<double>(numberSettings) / autoThreads);
                int remaining = numberSettings % autoThreads;
                int actualThreads = (autoThreads > numberSettings) ? numberSettings : autoThreads;

                std::list<scraperSettings>::iterator it = config.begin();
                for (int i = 0; i < actualThreads; i++)
                {

                    std::list<scraperSettings> tempList;
                    // int itemsToAdd;

                    int itemsToAdd = (i < remaining) ? perThread : (perThread - 1);

                    if (i >= remaining && remaining == 0)
                    {
                        itemsToAdd = perThread; // For the case when remaining == 0
                    }

                    for (int j = 0; j < itemsToAdd; j++)
                    {

                        //  int index = i * perThread + j;

                        // std::advance(it, index);

                        if (it != config.end())
                        {
                            tempList.push_back(*it);

                            Threadmap[it->name] = i;
                            it++;
                        }
                        else
                        {
                            std::cout << "Index out of range" << std::endl;
                        }
                    }

                    output["auto"].push_back(tempList);
                }
            }
        }
    }
}

/*
void ConfigClass::loadNew(ConfigClass &newConfig)
{

    // Flagging and updating general Configs
    for (auto &stringConfig : this->stringConfigs)
    {

        if (stringConfig.second() != newConfig.stringConfigs[stringConfig])
        {

            // stringConfig changed //maybe do a check to what type of restart is needed

            stringConfig.second = newConfig.stringConfigs[stringConfig];
        }
    }

    for (auto &intConfig : this->intConfigs)
    {

        if (intConfig.second() != newConfig.intConfigs[intConfig])
        {

            // stringConfig changed //maybe do a check to what type of restart is needed

            intConfig.second = newConfig.intConfigs[intConfig];
        }
    }

    for (auto &longConfig : this->longConfigs)
    {

        if (longConfig.second() != newConfig.longConfigs[longConfig])
        {

            // stringConfig changed //maybe do a check to what type of restart is needed

            longConfig.second = newConfig.longConfigs[longConfig];
        }
    }

    for (auto &stringArrConfig : this->stringArrConfigs)
    {

        if (stringArrConfig.second() != newConfig.stringArrConfigs[stringArrConfig])
        {

            // stringConfig changed //maybe do a check to what type of restart is needed

            stringArrConfig.second = newConfig.stringArrConfigs[stringArrConfig];
        }
    }

    std::list<scaperSettings> allSettingsCurrent = this.scraperSettingsAll["singlePage"].insert(this.scraperSettingsAll["singlePage"].end(), this.scraperSettingsAll["multiPage"], this.scraperSettingsAll["multiPage"].end());
    std::list<scraperSettings> allSettingsNew = newConfig.scraperSettingsAll["singlePage"].insert(newConfig.scraperSettingsAll["singlePage"].end(), newConfig.scraperSettingsAll["multiPage"], newConfig.scraperSettingsAll["multiPage"].end());

    std::list<scraperSettings> removed;
    std::list<scraperSettings> added;

    allSettingsCurrent.sort(compareByName);
    allSettingsNew.sort(compareByName);

    std::scraperSettings *currentItem;
    std::scraperSettings *newItem;

    int i = 0;
    int j = 0;
    ;
    while (i < allSettingsCurrent.size() && j < allSettingsNew.size())
    {
        currentItem = allSettingsCurrent[i];
        newItem = allSettingsNew[j];

        if (currentItem.name == newItem.name)
        {
            // Item found in both lists, but check for differences elsewhere in struct.
            // Your logic for comparing the rest of the struct goes here.

            // Move to the next items in both lists
            ++i;
            ++j;
        }
        else if (currentItem.name > newItem.name)
        {
            // Item in current but not in new list
            removed.push_back(currentItem);
            ++i; // Move to the next item in current list
        }
        else if (currentItem.name < newItem.name)
        {
            // Item in new list but not in current
            added.push_back(newItem);
            ++j; // Move to the next item in new list
        }
    }

    // Add remaining elements from current and new lists if any
    while (i < allSettingsCurrent.size())
    {
        removed.push_back(allSettingsCurrent[i]);
        ++i;
    }

    while (j < allSettingsNew.size())
    {
        added.push_back(allSettingsNew[j]);
        ++j;
    }
}

*/
bool ConfigClass::compareByName(const scraperSettings &a, const scraperSettings &b)
{
    return a.name < b.name;
}
