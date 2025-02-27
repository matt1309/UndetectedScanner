#include "WebServer.h" // To build, run: cc main.c mongoose.c
#include <thread>
#include <unordered_set>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
// #include "Logger.cpp"

struct mg_connection *WebServer::webSocketLogs;

bool isAllowedFile(const std::string &filename)
{

    bool output = false;
    // Define a whitelist of allowed filenames (modify as needed)
    static const std::unordered_set<std::string> allowedFiles = {
        ".html",
        ".css",
        ".js",
        ".ico",
        ".json"
        // Add other allowed filenames here
    };

    for (const std::string &element : allowedFiles)
    {

        if (filename.find(element) != std::string::npos)
        {

            return true;
        }
    }

    return allowedFiles.count(filename) > 0;
}

std::string sanitizePath(const std::string &path)
{
    std::string sanitizedPath = path;
    // Remove leading slash if present
    if (sanitizedPath.front() == '/')
    {
        sanitizedPath.erase(0, 1);
    }

    // Remove '..' from the path to prevent directory traversal attacks
    size_t found = sanitizedPath.find("..");
    while (found != std::string::npos)
    {
        size_t slashPos = sanitizedPath.rfind('/', found - 1);
        if (slashPos != std::string::npos)
        {
            sanitizedPath.erase(slashPos, found + 2 - slashPos);
        }
        found = sanitizedPath.find("..", found + 1);
    }
    return sanitizedPath;
}

// HTTP server event handler function
void http_handler(struct mg_connection *c, int ev, void *ev_data)
{
    if (ev == MG_EV_HTTP_MSG)
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        // Sanitize requested path to prevent directory traversal attacks
        std::string sanitizedPath = sanitizePath(std::string(hm->uri.ptr, hm->uri.len));

        // Default to index.html if the path is empty
        if (sanitizedPath.empty())
        {
            sanitizedPath = "index.html";
        }

        // Prepend the root directory to the sanitized path
        std::string root_dir = "./website/";

        std::string fullPath = "";

        if ((sanitizedPath.find("websocket") != std::string::npos) || (sanitizedPath.find("log") != std::string::npos))
        {
            fullPath = sanitizedPath;
        }
        else
        {
            fullPath = root_dir + sanitizedPath;
        }

        // Debugging output to check the paths
        std::cout << "Requested URI: " << std::string(hm->uri.ptr, hm->uri.len) << std::endl;
        std::cout << "Sanitized Path: " << sanitizedPath << std::endl;
        std::cout << "Full Path: " << fullPath << std::endl;

        if (mg_http_match_uri(hm, "/log")) // New endpoint for retrieving logs
        {
            // Create JSON object
            rapidjson::Document logsData;
            logsData.SetObject();

            // Create JSON array for thread counts
            rapidjson::Value threadCount(rapidjson::kArrayType);
            const size_t manualLogsSize = Logger::manuallogs.size();
            const size_t autoLogsSize = Logger::autologs.size();
            threadCount.PushBack(manualLogsSize, logsData.GetAllocator());
            threadCount.PushBack(autoLogsSize, logsData.GetAllocator());
            logsData.AddMember("threadCount", threadCount, logsData.GetAllocator());

            // Read general log
            const std::string toAdd = Logger::readGeneralLog("webformat").str();
            rapidjson::Value webformatValue(rapidjson::kStringType);
            webformatValue.SetString(toAdd.c_str(), toAdd.length(), logsData.GetAllocator());

            // Add webformat value directly to logsData under "general"
            logsData.AddMember("GeneralLog", webformatValue, logsData.GetAllocator());

            // Add other log data
            // logsData.AddMember("singlePage", Logger::singlePagelogsWebFormat, logsData.GetAllocator());
            // logsData.AddMember("multiPage", Logger::multiPagelogsWebFormat, logsData.GetAllocator());
            // logsData.AddMember("multiPageVPN", Logger::multiPagelogsWebFormat, logsData.GetAllocator());

            // rapidjson::Value statusShutdown(rapidjson::kStringType);

            // lock here using threadManager's thread lock to get status and shutDown status.
            // build JSON in for format below

            rapidjson::Value autoStatusArray(rapidjson::kArrayType);
            rapidjson::Value manualStatusArray(rapidjson::kArrayType);

            for (auto &type : threadManager::status())
            {

                rapidjson::Value temp(rapidjson::kArrayType);
                for (auto &status : type.second)
                {
                    temp.PushBack(status, logsData.GetAllocator());
                }

                if (type.first == "auto")
                {

                    autoStatusArray = temp;
                }
                else
                {

                    manualStatusArray = temp;
                }
            }

            logsData.AddMember("autoStatus", autoStatusArray, logsData.GetAllocator());
            logsData.AddMember("manualStatus", manualStatusArray, logsData.GetAllocator());

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            logsData.Accept(writer);

            // Send JSON response

            // mg_http_reply(c, 200, "Content-Type: application/json", "%s", buffer.GetString());

            mg_http_reply(c, 200, "Content-Type: application/json\r\nConnection: close\r\n", "%s", buffer.GetString());
            std::cout << buffer.GetString() << std::endl;
        }
        else if (mg_http_match_uri(hm, "/getConfig"))
        {
            std::string configPath = "./config.json";
            std::ifstream configFile(configPath);
            if (configFile.is_open())
            {
                std::string configContent((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
                configFile.close();
                mg_http_reply(c, 200, "Content-Type: application/json\r\nConnection: close\r\n", "%s", configContent.c_str());
            }
            else
            {
                mg_http_reply(c, 500, "", "Unable to read config file");
            }
        }
        else if (mg_http_match_uri(hm, "/websocket"))
        {
            mg_ws_upgrade(c, hm, NULL);
            WebServer::webSocketLogs = c;

            std::cout << "WebSocket connection upgraded" << std::endl;

            struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;

            mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
            std::cout << "Message: " << std::string(wm->data.ptr, wm->data.len) << std::endl;
        }
        else
        {
            // Check if the sanitized path is an allowed file
            if (isAllowedFile(sanitizedPath))
            {
                struct mg_http_serve_opts opts = {.root_dir = root_dir.c_str()};
                mg_http_serve_file(c, hm, fullPath.c_str(), &opts);
            }
            else
            {
                std::cout << "Forbidden: " << fullPath << std::endl;
                mg_http_reply(c, 403, "", "Forbidden"); // Send 403 for disallowed files
            }
        }
    }
}

WebServer::WebServer(const std::string &address)
{
    if (address != "")
    {
        std::thread server_thread([this, address]()
                                  { this->startServer(address); });
        server_thread.detach();
    }
}

void WebServer::startServer(const std::string &address)
{
    std::cout << "WebServer starting" << std::endl;

    // Declare event manager
    mg_mgr_init(&mgr);                                         // Initialize event manager
    mg_http_listen(&mgr, address.c_str(), http_handler, NULL); // Setup listener
    std::cout << "WebServer started" << std::endl;

    for (;;)
    { // Run an infinite event loop
        mg_mgr_poll(&mgr, 1000);
    }
}

void WebServer::sendWebSocketMessage(const std::string &message)
{
    if (webSocketLogs != nullptr)
    {
        mg_ws_send(webSocketLogs, message.c_str(), message.length(), WEBSOCKET_OP_TEXT);
        std::cout << "Websocket message sent: " << message << std::endl;
    }
    else
    {
        std::cerr << "No websocket connection established." << std::endl;
    }
}
