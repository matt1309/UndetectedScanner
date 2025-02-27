// #include "ConcurrentHashMap.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>
// #include "ConcurrentHashMap.cpp" //change this to .h
#include "Logger.h"

std::mutex Logger::logLock;
ConcurrentHashMap<std::string, std::string> Logger::status;
ConcurrentHashMap<std::string, std::string> Logger::logsByName;
ConcurrentHashMap<int, std::string> Logger::manuallogs;
ConcurrentHashMap<int, std::string> Logger::autologs;
ConcurrentHashMap<int, std::string> Logger::manualogsWebFormat;
ConcurrentHashMap<int, std::string> Logger::autologsWebFormat;
std::stringstream Logger::generalLog;
std::stringstream Logger::generalLogWebFormat;
dpp::cluster *Logger::discordBot = nullptr;
WebServer *Logger::webserver = nullptr;
bool Logger::ready;

std::shared_mutex Logger::discordMutex;
std::shared_mutex Logger::telegramMutex;
std::shared_mutex Logger::slackMutex;

std::string Logger::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm timeinfo;
    localtime_s(&timeinfo, &in_time_t); // Use localtime_s instead of localtime

    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%d-%m-%Y %H.%M.%S"); // Use std::put_time with tm object
    return ss.str();
}

/*s tatic bool ready;

static std::vector<std::mutex> mutexes;

static std::stringstream generalLog;
static std::stringstream generalLogWebFormat;
static std::queue<std::pair<std::string, std::string>> queue;
*/
void Logger::logToFile(std::string &type, int key, std::string &contents)
{
    std::string formattedDateTime = getCurrentTime();
    std::string filePath = "logs/";

    if (type == "manual")
    {
        filePath += "manual/manualLog-" + std::to_string(key) + "-" + formattedDateTime + ".txt";
        Logger::writeToFile(contents, filePath);
    }
    else if (type == "auto")
    {
        filePath += "auto/autoLog-" + std::to_string(key) + "-" + formattedDateTime + ".txt";
        Logger::writeToFile(contents, filePath);
    }
    else if (type == "general")
    {
        filePath += "generalLog-" + formattedDateTime + ".txt";
        Logger::writeToFile(contents, filePath);
    }

    std::cout << "Finished writing logs to file: " << type << " " << key << std::endl;
}

void Logger::writeToFile(const std::string &content, const std::string &filePath)
{
    std::ofstream outputFile(filePath);
    if (outputFile.is_open())
    {
        outputFile << content;
        outputFile.close();
        std::cout << "Writing String to File: " << filePath << std::endl;
    }
    else
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
}

// static ConcurrentHashMap<std::string, std::string> logsByName;

void Logger::logSetup()
{
    std::atexit(shutdownHook);
    ready = true;
}

bool Logger::isReady()
{
    return ready;
}

void Logger::shutdownHook()
{
    std::cout << "Shutting down please wait as logs are written to file" << std::endl;
    std::string type1 = "auto";
    std::string type2 = "manual";
    std::string type3 = "general";

    for (const auto &entry : autologs)
    {
        std::string logvalue = autologs.get(entry.first);
        logToFile(type1, entry.first, logvalue);
    }
    for (const auto &entry : manuallogs)
    {
        std::string logvalue = manuallogs.get(entry.first);
        logToFile(type1, entry.first, logvalue);
    }

    {
        // fix this
        std::lock_guard<std::mutex> lock(logLock);
        std::string logvalue = generalLog.str();

        logToFile(type3, 0, logvalue);
    }
}

void Logger::nightlyLogReset()
{
    std::cout << "Writing logs to file and resetting" << std::endl;
    Logger::logger(0, "general", "general", "Writing logs to file and resetting");

    std::string type1 = "auto";
    std::string type2 = "manual";
    std::string type3 = "general";

    for (const auto &entry : manuallogs)
    {
        std::string logvalue = manuallogs.get(entry.first);
        logToFile(type1, entry.first, logvalue);
    }
    for (const auto &entry : autologs)
    {
        std::string logvalue = autologs.get(entry.first);
        logToFile(type2, entry.first, logvalue);
    }

    std::string logvalue = generalLog.str();

    logToFile(type3, 0, logvalue);

    // logToFile("general", 0, generalLog.str());

    clearLogs();
}

void Logger::logger(int threadnumber, std::string &name, const std::string &type, const std::string &input)
{
    std::string time = getCurrentTime();
    std::string logContent = time + ":  " + input;

    if (type == "manual")
    {

        if (!manuallogs.contains(threadnumber))
        {
            manuallogs.put(threadnumber, "");
        }
        if (!manualogsWebFormat.contains(threadnumber))
        {
            manualogsWebFormat.put(threadnumber, "");
        }
        if (!logsByName.contains(name))
        {
            logsByName.put(name, "");
        }

        manuallogs.edit(threadnumber, logContent, 30000000, false, type);
        manualogsWebFormat.edit(threadnumber, logContent, 30000000, true, type);
        logsByName.edit(name, logContent, 30000000, true, type);

        /*
                // need to add some safeties here for the event where the element doesnt exist yet.
                std::lock_guard<std::mutex> lock1(manuallogs.getMutexForKey(threadnumber));
                std::lock_guard<std::mutex> lock2(manualogsWebFormat.getMutexForKey(threadnumber));
                std::lock_guard<std::mutex> lock3(logsByName.getMutexForKey(name));



                auto &oldLog = manuallogs.getRef(threadnumber);
                auto &oldLogWebFormat = manualogsWebFormat.getRef(threadnumber);
                auto &logsByNameContent = logsByName.getRef(name);

                if (oldLog.empty() || oldLog.length() > 50000000)
                {
                    oldLog = "";
                }
                else if (oldLog.length() > 200000) //make this configurable in settings.
                {
                    logToFile(type, threadnumber, oldLog);

                    oldLog = "To view older log content please view log files\n" + std::move(logContent);
                    oldLogWebFormat = "To view older log content please view log files <br>" + std::move(logContent);
                }
                else
                {
                    oldLog += "\n" + std::move(logContent);
                    oldLogWebFormat += "<br>" + std::move(logContent);
                    logsByNameContent += "<br>" + std::move(logContent);
                }
                */

        if (webserver != nullptr)
        {

            std::string content = "{\"ManualLog\": \n{ \"" + std::to_string(threadnumber) + "\" : \"";
            content.append(logContent);
            content.append("<br>\"}}");
            webserver->sendWebSocketMessage(content);
        }

        // manuallogs.put(threadnumber, oldLog);
        // manualogsWebFormat.put(threadnumber, oldLogWebFormat);
    }
    else if (type == "auto")
    {

        if (!autologs.contains(threadnumber))
        {
            autologs.put(threadnumber, "");
        }
        if (!autologsWebFormat.contains(threadnumber))
        {
            autologsWebFormat.put(threadnumber, "");
        }
        if (!logsByName.contains(name))
        {
            logsByName.put(name, "");
        }

        autologs.edit(threadnumber, logContent, 30000000, false, type);
        autologsWebFormat.edit(threadnumber, logContent, 30000000, true, type);
        logsByName.edit(name, logContent, 30000000, true, type);

        if (webserver != nullptr)
        {

            std::string content = "{\"AutoLog\": \n{ \"" + std::to_string(threadnumber) + "\" : \"";
            content.append(logContent);
            content.append("<br>\"}}");
            webserver->sendWebSocketMessage(content);
        }

        /*
                std::lock_guard<std::mutex> lock1(autologs.getMutexForKey(threadnumber));
                std::lock_guard<std::mutex> lock2(autologsWebFormat.getMutexForKey(threadnumber));
                std::lock_guard<std::mutex> lock3(logsByName.getMutexForKey(name));

                auto &oldLog = autologs.getRef(threadnumber);
                auto &oldLogWebFormat = autologsWebFormat.getRef(threadnumber);
                auto &logsByNameContent = logsByName.getRef(name);

                if (oldLog.empty() || oldLog.length() > 30000000)
                {
                    oldLog = "";
                }
                else if (oldLog.length() > 200000)
                {
                    logToFile(type, threadnumber, oldLog);

                    oldLog = "To view older log content please view log files\n" + std::move(logContent);
                    oldLogWebFormat = "To view older log content please view log files <br>" + std::move(logContent);
                }
                else
                {
                    oldLog += "\n" + std::move(logContent);
                    oldLogWebFormat += "<br>" + std::move(logContent);
                }
                */
    }
    else if (threadnumber == 0 && type == "general")
    {

        write2generalLog(logContent);
        std::cout << logContent << std::endl;

        if (webserver != nullptr)
        {

            std::string content = "{\"GeneralLog\": \"";
            content.append(logContent);
            content.append("<br>\"}");
            webserver->sendWebSocketMessage(content);
        }
    }
}

void Logger::logger(int threadnumber, const char *name, const char *type, const char *input)
{
    std::string nameStr(name);
    std::string typeStr(type);
    std::string inputStr(input);
    logger(threadnumber, nameStr, typeStr, inputStr);
}

void Logger::logger(int threadnumber, std::string &name, const char *type, const char *input)
{

    std::string typeStr(type);
    std::string inputStr(input);
    logger(threadnumber, name, typeStr, inputStr);
}
void Logger::logger(int threadnumber, const char *name, const std::string &type, const char *input)
{

    std::string nameStr(name);
    std::string inputStr(input);
    logger(threadnumber, nameStr, type, inputStr);
}
void Logger::logger(int threadnumber, const char *name, const char *type, const std::string &input)
{

    std::string nameStr(name);
    std::string typeStr(type);
    // std::string inputStr(input);
    logger(threadnumber, nameStr, typeStr, input);
}

void Logger::logger(int threadnumber, const char *name, const std::string &type, const std::string &input)
{

    std::string nameStr(name);
    // std::string typeStr(type);
    // std::string inputStr(input);
    logger(threadnumber, nameStr, type, input);
}

void Logger::write2generalLog(const std::string &input)
{
    std::lock_guard<std::mutex> lock(logLock);

    if (generalLog.str().empty())
    {
        // Handle initialization if necessary
    }

    if (generalLog.str().length() > 50000000 || generalLogWebFormat.str().length() > 30000000)
    {
        generalLog.str("");
        generalLogWebFormat.str("");
    }

    generalLog << input << "\n";
    generalLogWebFormat << input << "<br>";

    if (webserver != nullptr)
    {

        std::string content = "{\"GeneralLog\": \"";
        content.append(input);
        content.append("<br>\"}");
        webserver->sendWebSocketMessage(content);
    }
}

// Method to read from the general log
std::stringstream &Logger::readGeneralLog(const std::string &input)
{
    std::lock_guard<std::mutex> lock(logLock);

    if (input == "webformat")
    {
        return generalLogWebFormat;
    }
    else
    {
        return generalLog;
    }
}

void Logger::clearLogs()
{
    try
    {
        // Clear logs in maps
        for (auto &entry : autologs)
        {
            std::string emptyString = "";
            autologs.put(entry.first, emptyString);
        }
        for (auto &entry : manuallogs)
        {
            std::string emptyString = "";
            manuallogs.put(entry.first, emptyString);
        }

        // Clear general log
        {
            std::lock_guard<std::mutex> lock(logLock);
            generalLog.clear();
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Logger::sendMessage(const std::string &name, const std::string &discordChannel, const std::string &message)
{
  if (webserver != nullptr) {
    // JSON Format message for the front end (unchanged)
  }

  if (discordBot != nullptr) {
    std::vector<std::string> channels = util::split(discordChannel, ":");
    for (const std::string &channel : channels) {
      try {
        dpp::snowflake channel_id;
        try {
          channel_id = std::stoull(channel);
        } catch (const std::invalid_argument &e) {
          std::cerr << "Error converting channel ID to snowflake: " << e.what() << std::endl;
          return;
        }

        std::shared_lock lock(discordMutex);

        discordBot->message_create(dpp::message(channel_id, message),
                                   [channel_id](const dpp::confirmation_callback_t &event) {
                                     std::cout << "Test" << std::endl;
                                     if (event.is_error()) {
                                       std::cerr << "Error sending message to channel " << channel_id << ": "
                                                 << event.get_error().message << std::endl;
                                     } else {
                                       std::cout << "Message sent successfully to channel " << channel_id << std::endl;
                                     }
                                   });
      } catch (const std::exception &e) {
        std::cerr << "Exception while sending message: " << e.what() << std::endl;
      }
    }
  } else {
    std::cerr << "Discord bot is not initialized." << std::endl;
  }
}

void Logger::startDiscord(std::string &discord_token)
{

    std::unique_lock lock(discordMutex);
   // discordBot = new dpp::cluster(discord_token);
    // discordBot->start(dpp::st_wait);

     discordBot = new dpp::cluster(discord_token, dpp::i_default_intents | dpp::i_message_content);

    discordBot->on_log([](const dpp::log_t &event)
                       { std::cout << "Log: " << event.message << std::endl; });

    discordBot->start(dpp::st_return);
}

void Logger::startSlack(std::string &slack_token)
{

    std::unique_lock lock(slackMutex);

    // code for starting slack goes here
    // discordBot = new dpp::cluster(App::mainConfig.stringConfigs["discord_token"]);
    // discordBot->start(dpp::st_wait);
}

void Logger::startTelegram(std::string &telegram_token)
{

    std::unique_lock lock(telegramMutex);

    // code for starting slack goes here
    // discordBot = new dpp::cluster(App::mainConfig.stringConfigs["discord_token"]);
    // discordBot->start(dpp::st_wait);
}

void Logger::wsConn(WebServer &webserver)
{

    Logger::webserver = &webserver;
}

/*
void Logger::initialize(size_t manualLogsSize, size_t autoLogsSize) {
    Logger::manuallogs = ConcurrentHashMap<int, std::string>(manualLogsSize);
    Logger::autologs = ConcurrentHashMap<int, std::string>(autoLogsSize);
    Logger::manualogsWebFormat = ConcurrentHashMap<int, std::string>(manualLogsSize);
    Logger::autologsWebFormat = ConcurrentHashMap<int, std::string>(autoLogsSize);
}
*/