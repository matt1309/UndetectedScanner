#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "ConcurrentHashMap.cpp"
#include "util.h"
#include "include/dpp/dpp.h"
#include "WebServer.h"

//template <typename Key, typename Value> class ConcurrentHashMap;

class WebServer;

class Logger
{
    
private:
    static bool ready;
    static std::vector<std::mutex> mutexes;
    static std::stringstream generalLog;
    static std::stringstream generalLogWebFormat;
    static std::queue<std::pair<std::string, std::string>> queue;

    static void shutdownHook();
    static void logToFile(std::string &type, int key, std::string& contents);
    static void writeToFile(const std::string &content, const std::string &filePath);
    static std::string getCurrentTime();

public:
    static std::mutex logLock;
    static ConcurrentHashMap<std::string, std::string> status;
    static ConcurrentHashMap<std::string, std::string> logsByName;
    static ConcurrentHashMap<int, std::string> manuallogs;
    static ConcurrentHashMap<int, std::string> autologs;
    static ConcurrentHashMap<int, std::string> manualogsWebFormat;
    static ConcurrentHashMap<int, std::string> autologsWebFormat;

    static void logSetup();
    static bool isReady();
    static void nightlyLogReset();
    static void logger(int threadnumber, std::string &name, const std::string &type, const std::string &input);
    static void logger(int threadnumber, const char* name, const char* type, const char* input);
     static void logger(int threadnumber, std::string &name, const char* type, const char* input);
     static void logger(int threadnumber, const char* name, const std::string &type, const char* input);
     static void logger(int threadnumber, const char* name, const char* type, const std::string &input);
     static void logger(int threadnumber, const char* name, const std::string &type, const std::string &input);




    void static write2generalLog(const std::string &input);
    static std::stringstream &readGeneralLog(const std::string &input);
    static void clearLogs();
    static void sendMessage(const std::string& name, const std::string& discordChannel, const std::string& message);
static void startDiscord(std::string& discord_token);
static void startSlack(std::string& slack_tokens);
static void startTelegram(std::string& telegram_tokens);

static void wsConn(WebServer& webserver);


static WebServer* webserver;
    static dpp::cluster* discordBot;

     static std::shared_mutex discordMutex;
    static std::shared_mutex telegramMutex;
    static std::shared_mutex slackMutex;

    static void initialize(size_t manualLogsSize, size_t autoLogsSize);

};

#endif /* LOGGER_H */
