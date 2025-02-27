#include <iostream>
// #include <string>
#include "ConfigClass.h"
#include "threadManager.h"
#include <chrono>
#include <thread>
#include "WebServer.h"
#include "Logger.h"
#include <mutex>

#include "App.h"

using namespace std;



int main()
{

cout << "Starting App\n"<< endl;

App::start();

return 0;





/*
    cout << "Start App\n";

cout << "Starting logger" << endl;

//Mutex's used for alerting for centalised element changes (will also need to add one for config somewhere...)
std::mutex configMutex;
std::mutex discordMutex;
std::mutex telegramMutex;
std::mutex slackMutex;

Logger logger(); //will just need to refernece the centrall held stuff. 



    cout << "Generating config" << endl;

    ConfigClass mainConfig;

    cout << "Config Generation Success\n";

    cout << "Starting Webserver" << endl;

    WebServer webserver(mainConfig.stringConfigs["webServer"]);

    std::this_thread::sleep_for(std::chrono::seconds(60));

    threadManager::startScrapers(mainConfig); //need config mutex in here but might just reference centrally. 

    // onlyThreadManager.startScrapers(mainConfig);
    cout << "done2" << endl;

    for (auto& thread : threadManager::manualThreads) {
        thread.join();
    }
    for (auto& thread : threadManager::autoThreads) {
        thread.join();
    }






    return 0;
    // some codes

    */
}