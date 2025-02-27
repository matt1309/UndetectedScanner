#ifndef WebServer_H
#define WebServer_H


#pragma once

#include <mutex>
#include <iostream>
#include <string> // Include string header for std::string
#include <unordered_set>
#include "Logger.h"
#include "threadManager.h"



#include "include/mongoose.h"
class Logger;

class WebServer
{
private:
    // General setting memory objects

    // Add any lock-related declarations here

    // string rawConfig; // This seems to be a member variable, not a function
public:
    static char http_port[32]; // Allocate space for port string
                               // static struct mg_serve_http_opts;
    static struct mg_serve_http_opts http_server_opts;

    WebServer(const std::string &address); // Constructor declaration
    void startServer(const std::string &address);

     struct mg_mgr mgr;



    void sendWebSocketMessage(const std::string& message);
    static struct mg_connection *webSocketLogs;
};


#endif // WEBSERVER_H