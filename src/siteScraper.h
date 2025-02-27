#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include "/rapidjson/document.h"
#include "/rapidjson/writer.h"
#include "/rapidjson/stringbuffer.h"
#include <cstdbool>
#include <atomic>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include "ConfigClass.h"
#include <fstream>
#include "util.h"
#include "Logger.h"
//#include "lexbor/core/core.h"
//#include "lexbor/html/base.h"

//#include "lexbor/html/parser.h"
//#include "lexbor/html/interfaces/document.h"
//#include "lexbor/selectors/selectors.h"
//#include "lexbor/css/css.h"
//#include "lexbor/html/html.h"
//#include "lexbor/dom/collection.h"

//#include "lexbor/dom/interfaces/element.h"

#include <sstream>
#include <chrono>
#include "chrome.h"

//#ifdef _MSC_VER // Check if compiling with MSVC

#include "include/lexbor/core/core.h"
#include "include/lexbor/html/parser.h"
#include "include/lexbor/html/base.h"
#include "include/lexbor/html/interfaces/document.h"
#include "include/lexbor/selectors/selectors.h"
#include "include/lexbor/css/css.h"
#include "include/lexbor/html/html.h"
#include "include/lexbor/dom/collection.h"
#include "include/lexbor/dom/interfaces/element.h"
#include "include/lexbor/dom/interfaces/node.h"
#include "include/lexbor/dom/interfaces/document.h"
#include "include/lexbor/dom/dom.h"
#include "include/lexbor/css/parser.h"

#include "include/pugixml.hpp"
//#include "include/pugixml.hpp"

/*
#else

#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/core/core.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/html/parser.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/html/base.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/html/interfaces/document.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/selectors/selectors.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/css/css.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/html/html.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/dom/collection.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/dom/interfaces/element.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/dom/interfaces/node.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/dom/interfaces/document.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/dom/dom.h"
#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/lexbor/css/parser.h"

#include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-mingw-static/include/pugixml.hpp"
#endif
*/

using namespace rapidjson;

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class siteScraper
{
private:

    //std::pair<std::string,std::string> dataOutput;


    std::vector<std::string> reTryableErrors;

    std::unordered_map<std::string, std::vector<std::string>> oldOutputList;
    std::vector<std::string> oldOutput;
    std::vector<std::string> output;

    std::unordered_map<std::string, TimePoint> lastTime;
    std::chrono::time_point<std::chrono::system_clock> startTime;




    std::string* link;
    std::string* site;
    std::string* name;
    std::string* channel;
    std::string* channelTelegram;
    std::string* useragent;
    std::string* proxy;
    std::string* sendLink;
    std::string* requestData;

    std::string* filter;
    std::string* replace;
    std::string* browser;
    //std::string* scannerType;
    std::string* requestType;
    std::string* pythonScriptName;
    std::string* pyFunctionName;


    int* frequency;
    int* offset;
    int* offsetMax;
    int* maxOutput;
    int* timeout;

    std::vector<std::string>* functions;
    std::string* firstFunc;
    std::vector<std::string>* selectors;
    std::vector<std::string>* prefix;
    std::vector<std::string>* suffix;
    std::string* headers;
    std::vector<std::pair<std::string, std::string>>* botIdentifiers;


    bool checkFilter(const std::string& newitem, const std::vector<std::string>& positive, const std::vector<std::string>& negative);








    std::unordered_map<std::string, std::string> getData(bool& firstRun, std::string& browser, std::string& type, std::string moderator_channel, std::string& moderator_channel_telegram, int threadnumber, std::string link, int timeout, std::vector<std::string> selectors);


    std::vector<std::string> extractJSON(std::pair<std::string, std::string>& data, std::vector<std::string>& selectors, std::vector<std::string>& functions, std::vector<std::string>& prefix, std::vector<std::string>& suffix);
    std::vector<std::string> extractXML(std::pair<std::string, std::string>& data, std::vector<std::string>& selectors, std::vector<std::string>& functions, std::vector<std::string>& prefix, std::vector<std::string>& suffix);
    std::vector<std::string> extractHTML(std::pair<std::string, std::string>& data, std::vector<std::string>& selectors, std::vector<std::string>& functions, std::vector<std::string>& prefix, std::vector<std::string>& suffix);


    std::string toJsonString(const Value& value);

    bool botDetection(std::string& dataOutput, std::vector<std::pair<std::string, std::string>>*& botIdentifiers);


    bool vpnRebooter(std::string& vpn, std::string& command, std::string &expectedResult, int timeoutSeconds);

    static bool pythonInitialized = false;


public:

    siteScraper(std::atomic<bool>& atom,std::atomic<bool>& configChangedFlag, std::list<ConfigClass::scraperSettings>& config, std::string type, std::string& moderator_channel, std::string& moderator_channel_telegram, int threadnumber, std::string &VPNrebootCommand);
    //std::string extractTextAndAttributes(GumboNode *node, std::string function);
    //std::string site_scraper_extract_text_and_attributes(const std::string& function, lexbor_dom_node_t *node);
    Value* jsonKeyExtractor(const std::vector<std::string>& jsonArrayKey, Value& jsonObject);
    //static lxb_status_t find_callback(lxb_dom_node_t *node, lxb_css_selector_specificity_t spec, void *ctx);
    std::string extractContent(lxb_dom_node_t* node, const std::string& function);

    std::pair<std::string, std::string> pythonScriptData(const std::string pythonScriptName,const std::string pyFunctionName, const bool retry, const std::string &url, const std::string &method,
        const std::string &headers,
        const std::string &useragent,
        const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber);


        std::pair<std::string, std::string> externalScriptData(const std::string pythonScriptName,const std::string pyFunctionName, const bool retry, const std::string &url, const std::string &method,
            const std::string &headers,
            const std::string &useragent,
            const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber);
    
    




};