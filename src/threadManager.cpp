#include "threadManager.h"
#include <iostream>
#include <chrono>
#include <algorithm>
// #include "App.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip> // Include this header for std::get_time
#include <sstream>
#include <stdexcept>
#include "siteScraper.h"
#include <functional>

std::list<std::thread> threadManager::manualThreads;
std::list<std::thread> threadManager::autoThreads;
std::vector<std::atomic<bool>> threadManager::manualStopCommand;
std::vector<std::atomic<bool>> threadManager::autoStopCommand;
std::vector<std::chrono::system_clock::time_point> threadManager::startTimes;
std::vector<std::chrono::system_clock::time_point> threadManager::stopTimes;
std::unordered_map<std::string, int> threadManager::vpnIssueTracker;
std::unordered_map<std::string, bool> threadManager::vpnRebooting;
std::vector<std::mutex> threadManager::vpnMutexes;
std::shared_mutex threadManager::threadMutex;
std::vector<std::atomic<bool>> threadManager::manualChangeCommand;
std::vector<std::atomic<bool>> threadManager::autoChangeCommand;
//ConcurrentHashMap<std::string, bool> vpnReboot; 











    //NEED TO RESET LOGS/RECREATE CONCURRENTHASHMAPS ON CHANGES OTHERWISE THERE MIGHT NOT BE ENOUGH MUTEX'S JUST WARN THE USERS THIS WILL OCCUR. IT'LL BE FINE AS IT'LL WRITE TO FILE/CONTINUE STRAIGHT AWAY
    //WILL NEED TO ADD SAFETIES INTO LOGGER TO PREVENT IT TRYING TO WRITE WHEN IT'S BEING UPDATED. AN OVERARCHING MUTEX THAT LIVES ELSE WHERE PROBABLY BEST. 



threadManager::threadManager()
{
    std::cout << "Making threadManager" << std::endl;
}

bool threadManager::vpnIssue(std::string &vpn)
{

    // Safely update the map with the issue count using a specific mutex for this VPN
    size_t hash = std::hash<std::string>{}(vpn);
    std::unique_lock<std::mutex> lock(vpnMutexes[hash % vpnMutexes.size()]);
    vpnIssueTracker[vpn]++;
    return true;
}

bool threadManager::vpnReboot(std::string &vpn)
{

    // Safely update the map with the issue count using a specific mutex for this VPN
    size_t hash = std::hash<std::string>{}(vpn);
    std::unique_lock<std::mutex> lock(vpnMutexes[hash % vpnMutexes.size()]);
    vpnRebooting[vpn] = true;
    return true;
}

void threadManager::startScrapers(ConfigClass &config) //could make this pass by value and use this and the thing that updates
{

    // code to run scheduler.

    std::cout << "ehere" << std::endl;
    // to check the checks in java in this function to make sure i also dont need them in here.
    //  std::map<std::string, std::vector<std::list<scraperSettings>>>
    // std::map<std::string, std::vector<std::list<ConfigClass::scraperSettings>>> splitConfigs = config.splitConfigs;

    for (std::pair<const std::string, std::vector<std::list<ConfigClass::scraperSettings>>> &settings : config.splitConfigs)
    {

        if (settings.first == "manual" && !settings.second.empty())
        {
            //going to swap these to a vector of points to atomic bools so i can handle dynamic adjusting size.
            std::vector<std::atomic<bool>> manualStopCommandLocal(settings.second.size());
            std::vector<std::atomic<bool>> manualChangeCommandLocal(settings.second.size());

            // Move or copy the constructed vector to manualStopCommand
            manualStopCommand = std::move(manualStopCommandLocal);
            manualChangeCommand = std::move(manualChangeCommandLocal);

            //  manualStopCommand = manualStopCommandLocal;
            // manualStopCommand = &manualStopCommandLocal;

            // manualStopCommand.resize(settings.second.size());
            for (int i = 0; i < settings.second.size(); i++)
            {

                manualStopCommand[i].store(true);
                manualChangeCommand[i].store(true);

                // std::atomic<bool> temp(true);
                // manualStopCommand[i].store(true);

                std::string type = settings.first;

                manualThreads.emplace_back([&, i, type]()
                                           { siteScraper siteScraper(manualStopCommand[i], manualChangeCommand[i], settings.second[i], type, config.stringConfigs["moderator_channel"], config.stringConfigs["telegram_moderator_channel"], i,config.stringConfigs["VPNrebootCommand"]); });
            }
        }
        else if (settings.first == "auto" && !settings.second.empty())
        {

            // autoStopCommand.resize(settings.second.size());
//going to swap these to a vector of points to atomic bools so i can handle dynamic adjusting size.
            std::vector<std::atomic<bool>> autoStopCommandLocal(settings.second.size());
            std::vector<std::atomic<bool>> autoChangeCommandLocal(settings.second.size());

            // Move or copy the constructed vector to manualStopCommand
            autoStopCommand = std::move(autoStopCommandLocal);
            autoChangeCommand = std::move(autoChangeCommandLocal);

            for (int i = 0; i < settings.second.size(); i++)
            {
                // autoStopCommand[i].store(true);
                autoStopCommand[i].store(true);
                autoChangeCommand[i].store(true);
                // work out how to change this to pass by reference

                std::string type = settings.first;

                autoThreads.emplace_back([&, i, type]()
                                         { siteScraper siteScraper(autoStopCommand[i],autoChangeCommand[i], settings.second[i], type, config.stringConfigs["moderator_channel"], config.stringConfigs["telegram_moderator_channel"], i, config.stringConfigs["VPNrebootCommand"]); });
            }
        }
    }
}
/*
void threadManager::scheduler(std::vector<std::chrono::system_clock::time_point> &Times, std::string &type)
{
    std::chrono::system_clock::time_point nextTime;

    if (type == "start")
    {
        nextTime = nextime(threadManager::convertStringsToTimePoints(App::mainConfig.stringArrConfigs["schedule_start"]));
        auto now = std::chrono::system_clock::now();
        auto duration = nextTime - now;

        auto task = [&]()
        { threadManager::startScrapers(); }; // Explicitly capture the class name

        std::thread startschedulerThread([duration, task]()
                                         {
            std::this_thread::sleep_for(duration);
            task(); });

        startschedulerThread.detach();
    }
    else if (type == "stop")
    {
        nextTime = nextime(threadManager::convertStringsToTimePoints(App::mainConfig.stringArrConfigs["schedule_start"]));
        auto now = std::chrono::system_clock::now();
        auto duration = nextTime - now;

        auto task = [&]()
        { threadManager::stopScrapers(); }; // Explicitly capture the class name

        std::thread stopschedulerThread([duration, task]()
                                        {
            std::this_thread::sleep_for(duration);
            task(); });

        stopschedulerThread.detach();
    }
}

*/

std::chrono::system_clock::time_point threadManager::nextime(std::vector<std::chrono::system_clock::time_point> &Times)
{

    auto currentTime = std::chrono::system_clock::now();

    // Find the next time point after the current time
    auto nextTimePoint = std::find_if(Times.begin(), Times.end(), [currentTime](const auto &timePoint)
                                      { return timePoint > currentTime; });

    if (nextTimePoint != Times.end())
    {
        // Next time point found

        std::cout << "Next time point: " << std::chrono::system_clock::to_time_t(*nextTimePoint) << std::endl;
        return *nextTimePoint;
    }
    else
    {
        auto tomorrow = std::chrono::system_clock::now() + std::chrono::hours(24);
        auto nextDayTimePoint = std::find_if(Times.begin(), Times.end(), [tomorrow](const auto &timePoint)
                                             { return timePoint > tomorrow; });

        if (nextDayTimePoint != Times.end())
        {
            std::cout << "Next time point on the next day: "
                      << std::chrono::system_clock::to_time_t(*nextDayTimePoint) << std::endl;
            return *nextDayTimePoint;
        }
        else
        {
            std::cout << "No future time points found as no schedule input" << std::endl;
            throw std::runtime_error("No future time points found as no schedule input");
        }
    }
}

std::vector<std::chrono::system_clock::time_point> threadManager::convertStringsToTimePoints(const std::vector<std::string> &timeStrings)
{
    std::vector<std::chrono::system_clock::time_point> timePoints;

    for (const auto &timeString : timeStrings)
    {
        std::tm tmStruct = {};
        std::istringstream ss(timeString);

        // Parse the time string into a std::tm structure
        ss >> std::get_time(&tmStruct, "%H:%M");

        if (ss.fail())
        {
            // Handle parsing error
            throw std::runtime_error("Failed to parse time string: " + timeString);
        }

        // Convert std::tm to std::chrono::system_clock::time_point
        auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tmStruct));
        timePoints.push_back(timePoint);
    }

    return timePoints;
}

int threadManager::timeToMinutes(const std::string &timeStr)
{
    int hours, minutes;
    sscanf(timeStr.c_str(), "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
}

// Function to schedule tasks
void threadManager::scheduleTasks(const std::vector<std::string> &times, const std::string &functionName, ConfigClass &config)
{
    std::map<int, std::function<void()>> tasks;

    // Populate tasks map with time in minutes since midnight and corresponding function
    for (const auto &timeStr : times)
    {
        int timeInMinutes = timeToMinutes(timeStr);
        tasks[timeInMinutes] = [&]()
        {
            // Dynamically call the function using its name
            std::cout << "Running function: " << functionName << std::endl;
            // Assuming the function name is available in the global scope
            if (functionName == "start")
            {
                threadManager::startScrapers(config);
            }
            else if (functionName == "stop")
            {
                // threadManager::stopScrapers(config);
            }
            else
            {
                std::cout << "Function " << functionName << " not found!" << std::endl;
            }
        };
    }

    // Run the scheduler
    while (true)
    {
        // Get current time in minutes since midnight
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::tm *now_tm = std::localtime(&now_time);
        int currentMinutes = now_tm->tm_hour * 60 + now_tm->tm_min;

        // Check if there's a task to run at the current time
        auto it = tasks.upper_bound(currentMinutes);
        if (it != tasks.end())
        {
            // Calculate time difference until next scheduled task
            int timeDifference = it->first - currentMinutes;

            // Sleep for longer if the next task is more than 5 minutes away
            if (timeDifference > 6)
            {
                std::this_thread::sleep_for(std::chrono::minutes(timeDifference - 6));
            }
            // Sleep for 5 minutes if the next task is within 5 minutes
            else
            {
                std::this_thread::sleep_for(std::chrono::minutes(2));
            }

            // Check if it's time to execute the task
            if (currentMinutes >= it->first - 2)
            {
                // Execute the task
                (it->second)();
                // Remove the task as it has been executed
                tasks.erase(it);
            }
        }
        else
        {
            // No tasks remaining, exit loop
            break;
        }
    }
}




//might swap these to mutex/lock

void threadManager::configChangeNotify(std::string type, int threadnumber){


if(type == "auto"){


autoChangeCommand[threadnumber].store(true);


}else if(type == "manual"){


manualChangeCommand[threadnumber].store(false);

}else{

//throw error/log

}









}


// Method to check if any scraper threads are running
    std::map<std::string,std::vector<std::string>> threadManager::status() {

        
                std::shared_lock<std::shared_mutex> slock(threadMutex);

        std::vector<std::string> manualThreadStatus;
        std::vector<std::string> autoThreadStatus;

    int i=0;
    //maybe add in another to check for issues in terms of mismatch size between shutdown command and threads. shouldnt happen but be good to check that's the case. 
        for (auto& thread : manualThreads) {
            if (thread.joinable() && manualStopCommand.size() > i - 1 && manualStopCommand[i].load()) {
                std::string c = "Shutting down";
                manualThreadStatus.push_back(c);
                i++;

            }else if(thread.joinable() && manualStopCommand.size() > i - 1 && !manualStopCommand[i].load()){
                std::string c = "Running";
                manualThreadStatus.push_back(c);
                i++;
                
            }else{
                  std::string c = "Not Running";
                manualThreadStatus.push_back(c);
                i++;
            }
        }
        for (auto& thread : autoThreads) {
             if (thread.joinable() && autoStopCommand.size() > i - 1 && autoStopCommand[i].load()) {
                std::string c = "Shutting down";
                autoThreadStatus.push_back(c);
                i++;

            }else if(thread.joinable() && autoStopCommand.size() > i - 1 && !autoStopCommand[i].load()){
                std::string c = "Running";
                autoThreadStatus.push_back(c);
                i++;
                
            }else{
                  std::string c = "Not Running";
                autoThreadStatus.push_back(c);
                i++;
            }
        }
    
    std::map<std::string, std::vector<std::string>> output;
    output["manual"] = manualThreadStatus;
    output["auto"] = autoThreadStatus;

    return output;


    }


    