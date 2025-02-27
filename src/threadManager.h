#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include "include/rapidjson/document.h"
#include <cstdbool>
#include <atomic>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include "ConfigClass.h"
#include <mutex>
#include <shared_mutex>


#pragma once
class ConfigClass;

#include <mutex>

class threadManager
{
private:



    static std::vector<std::atomic<bool>> manualStopCommand;
    static std::vector<std::atomic<bool>> autoStopCommand;

    static std::vector<std::atomic<bool>> manualChangeCommand;
    static std::vector<std::atomic<bool>> autoChangeCommand;

    static std::vector<std::chrono::system_clock::time_point> startTimes;
    static std::vector<std::chrono::system_clock::time_point> stopTimes;



    static std::unordered_map<std::string, int> vpnIssueTracker;
    static std::unordered_map<std::string, bool> vpnRebooting;
    static std::vector<std::mutex> vpnMutexes;
    static  std::shared_mutex threadMutex; //used for when changing number of threads. 





    //make this a singleTon






    void startScheduler();

    void healthChecker();


    void threadStatusCheck(std::vector<std::thread>);

    void threadAllStopped(std::vector<std::thread>);

    void scheduler(std::vector<std::chrono::system_clock::time_point>& startTimes, std::string& type);

    void configChangeNotify(std::string type, int threadnumner);
    int timeToMinutes(const std::string &timeStr);
    void scheduleTasks(const std::vector<std::string> &times, const std::string &functionName, ConfigClass &config);

    //void sleepUntil(std::chrono::system_clock::time_point timepoint, Callable&& func);

    std::chrono::system_clock::time_point nextime(std::vector<std::chrono::system_clock::time_point>& Times);
    std::vector<std::chrono::system_clock::time_point> convertStringsToTimePoints(const std::vector<std::string>& timeStrings);


public:

    static std::list<std::thread> manualThreads;
    static std::list<std::thread> autoThreads;

    bool vpnIssue(std::string& vpn);
    bool vpnReboot(std::string& vpn);


    static void startScrapers(ConfigClass& config);

    static void stopScrapers(ConfigClass& config);

   static std::map<std::string,std::vector<std::string>> status();



    threadManager();



};