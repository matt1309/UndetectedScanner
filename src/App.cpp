#include "App.h"

using namespace std;

ConfigClass App::mainConfig; // Definition with initialization
//dpp::cluster App::discordBot;

void App::start()
{

    cout << "App Created\n"
         << endl;

    // maybe move these somewhere else. like wrap around the botobejcts

    cout << "Starting logger" << endl;

    // Mutex's used for alerting for centalised element changes (will also need to add one for config somewhere...)

    Logger::logSetup();

Logger::logger(0,"general","general","Generating config");
   // cout << "Generating config" << endl;

    ConfigClass mainConfig;

  //  cout << "Config Generation Success\n";
    Logger::logger(0,"general","general","Config Generation Success");

  //  if (App::mainConfig.stringArrConfigs["discord_guildIDs"].size() != 1 && App::mainConfig.stringArrConfigs["discord_guildIDs"][0] != "")
    
  //if there a discord_token then strat discord bot
    if(mainConfig.stringConfigs["discord_token"] != "")
    {

        Logger::startDiscord(mainConfig.stringConfigs["discord_token"]);
        //   dpp::cluster discordBot(mainConfig.stringConfigs["discord_token"]);
        // discordBot.start(dpp::st_wait);
        // need to setup bot to regsiter commands here.

       // std::this_thread::sleep_for(std::chrono::seconds(10)); 

       //Send message to notify the D++ conneciton started 
        Logger::sendMessage("Startup Process",mainConfig.stringConfigs["moderator_channel"],"Undetected scanner discord connection complete");
    }

  //  cout << "Starting Webserver" << endl;
    Logger::logger(0,"general","general","Starting Webserver");

    WebServer webserver(mainConfig.stringConfigs["webServer"]);
    

    std::this_thread::sleep_for(std::chrono::seconds(60));

    Logger::wsConn(webserver);

    Logger::sendMessage("Startup Process",mainConfig.stringConfigs["moderator_channel"],"WebServer Connection started");

    threadManager::startScrapers(mainConfig); // need config mutex in here but might just reference centrally.

    // onlyThreadManager.startScrapers(mainConfig);
    cout << "done2" << endl;

    for (auto &thread : threadManager::manualThreads)
    {
        thread.join();
    }
    for (auto &thread : threadManager::autoThreads)
    {
        thread.join();
    }
}