#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
// #include "include/rapidjson/document.h"
#include <cstdbool>
#include <atomic>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include "Browser.h"
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <dlfcn.h>
#endif

// #include "includeMan/windowsCurl/curl.h"

#ifdef _MSC_VER // Check if compiling with MSVC

#include "include/curl-modified/curl.h"

#else

#include "chrome/curl-modified/curl.h"

#endif

class chrome : public Browser
{
    // include chrome version of curl ie boringssl version
    // need another of these classes for firefox.
private:
    // write the cyphers to file based on chrome version
    std::map<std::string, std::string> cyphers;

    std::string removeEscapes(const std::string &str);

    // this needs to go in cpp location.
    /*{{"chromeVerion":"cipher1, cipher2"},
     {110,"cipher1,cipher2"}}
    */

    CURL *curl;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data);
    std::pair<std::string, std::string> Request(const bool retry, const std::string &url, const std::string &method,
                                                const std::string &headers,
                                                const std::string &useragent,
                                                const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber);


    // map layout,
    std::map<std::string, std::map<std::string, std::string>> defaultOptions;

public:
    chrome();
    ~chrome();

    bool ready;
    void init();
    bool isReady();

    std::string getCypher(std::string &chromeVerion);
};