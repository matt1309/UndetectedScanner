// class for setting up chrome
#include "chrome.h"
#include <iostream>
#include <string>
#include "util.h"
#include <map>

chrome::chrome() : ready(false), curl(nullptr)
{

    ready = false;
}

void chrome::init()
{

    if (ready == false)
    {
#ifdef _WIN32

        HMODULE curlLib = LoadLibraryW(L"libcurl.dll");
#else
        void *curlLib = dlopen("lib/libcurl.so", RTLD_NOW | RTLD_GLOBAL);
#endif
        if (curlLib)
        {
            using CurlEasyInit = CURL *(*)();
#ifdef _WIN32
            CurlEasyInit curlEasyInit = (CurlEasyInit)GetProcAddress(curlLib, "curl_easy_init");
#else
            CurlEasyInit curlEasyInit = (CurlEasyInit)dlsym(curlLib, "curl_easy_init");
#endif
            if (curlEasyInit)
            {
                curl = curlEasyInit();
                ready = (curl != nullptr);
            }
        }

        // defaultHeadersLookup = {{"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.75 Safari/537.36", "hello"},
        //};
        // to add all of these. and do same for default headers

        if (curl)
        {

            ready = true;
        }
        else
        {

            std::cout << "Error loading curl" << std::endl;
        }
        // setup curl
    }
    else
    {

        // throw error that it's already setup.
    }
}

chrome::~chrome()
{
    curl_easy_cleanup(curl);
}

bool chrome::isReady()
{

    return ready;
}

std::pair<std::string, std::string> chrome::Request(const bool firstrun, const std::string &url, const std::string &method,
                                                    const std::string &inputHeaders,
                                                    const std::string &inputUseragent,
                                                    const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber)
{

    // todo make copies of all the input vars and store them in class var.
    // if new var equals old/current var then can skip clearing and loading curl. If not need to clear and reload inputs in.
    // do this at a later date.

    // setting up variables
    std::string cuseragent;
    std::string cCipher;
    bool chttp2;
    std::string chttp2settings;
    long chttp2WindowsUpdate;
    bool ccompressed;
    std::string cech;
    std::string ctlsVersion;
    bool calps;
    bool ctlsPermuteExtensions;
    std::string ccertCompression;
    std::string chttp2PseudoHeaderOrder;
    std::string ccurves;
    std::string csignatureHashes;
    bool cnoTLSSessionTicket;
    std::string cDefaultHeaders;
    bool ctlsGrease;
    std::map<std::string, std::string> headerAsMap;
    std::unordered_set<std::string> defaultHeaderKeys;
    std::unordered_set<std::string> alreadyAdded;

    if (inputUseragent == "")
    {

        cuseragent = "chrome110";
    }

    // setting default headers. will need to have another one of these for inputheaders further down. wrap each of these in a try incase headers or something has dodgy option. that it'll continue.
    // do this once debugging has been done/finalised.

    if (http2.find(cuseragent) != http2.end() && http2[cuseragent])
    {
        chttp2 = http2[cuseragent];

        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    }
    else
    {

        chttp2 = false;
    }

    if (tlsVersion.find(cuseragent) != tlsVersion.end())
    {
        ctlsVersion = tlsVersion[cuseragent];

        if (ctlsVersion == "1.2")
        {
            curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        }
        else if (ctlsVersion == "1.1")
        {
            curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_1);
        }
        else if (ctlsVersion == "1.0")
        {
            curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_0);
        }
        else
        {
            // Handle unsupported TLS version or invalid string
            std::cerr << "Unsupported or invalid TLS version" << std::endl;
        }
    }

    if (ciphers.find(cuseragent) != ciphers.end())
    {
        cCipher = ciphers[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, cCipher.c_str());
    }

    if (curves.find(cuseragent) != curves.end())
    {
        ccurves = curves[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_EC_CURVES, ccurves.c_str());
    }

    if (signatureHashes.find(cuseragent) != signatureHashes.end())
    {
        csignatureHashes = signatureHashes[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_SIG_HASH_ALGS, csignatureHashes.c_str());
    }

    if (npn.find(cuseragent) != npn.end() && npn[cuseragent] && npn[cuseragent])
    {
        // calpn = alpn[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_NPN, 1);
    }
    else
    {

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_NPN, 0);
    }

    if (alpn.find(cuseragent) != alpn.end() && alpn[cuseragent] && alpn[cuseragent])
    {
        // calpn = alpn[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPN, 1);
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPN, 0);
    }

    if (alps.find(cuseragent) != alps.end() && alps[cuseragent])
    {
        calps = alps[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPS, 1);
    }
    else
    {

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_ALPS, 0);
    }

    if (tlsSessionTicket.find(cuseragent) != tlsSessionTicket.end() && tlsSessionTicket[cuseragent])
    {

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_TICKET, 1);
    }
    else
    {

        curl_easy_setopt(curl, CURLOPT_SSL_ENABLE_TICKET, 0);
    }

    if (tlsPermuteExtensions.find(cuseragent) != tlsPermuteExtensions.end() && tlsPermuteExtensions[cuseragent])
    {
        ctlsPermuteExtensions = tlsPermuteExtensions[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_PERMUTE_EXTENSIONS, 1);
    }

    if (certCompression.find(cuseragent) != certCompression.end())
    {
        ccertCompression = certCompression[cuseragent];

        curl_easy_setopt(curl, CURLOPT_SSL_CERT_COMPRESSION, ccertCompression.c_str()); // "br" for Brotli
    }

    // same for this one. maybe put this above user input headers so users can define if they want.
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

    /*
        if (headers.find(cuseragent) != headers.end() || inputHeaders.length() > 0)
        {

            struct curl_slist *headerList = nullptr;
            if (headers.find(cuseragent) != headers.end())
            {
                cDefaultHeaders = headers[cuseragent];

                // Set headers

                size_t startPos = 0;
                while (startPos != std::string::npos && startPos < cDefaultHeaders.size())
                {
                    size_t endPos = cDefaultHeaders.find("\n", startPos);
                    if (endPos == std::string::npos)
                    {
                        endPos = cDefaultHeaders.size(); // Use cDefaultHeaders.size() instead of headers.size()
                    }
                    std::string header = cDefaultHeaders.substr(startPos, endPos - startPos);
                    // need some try catches in here!!!!!!
                    std::vector<std::string> headerVec = util::split(header, ":");
                    std::string headerKey = util::toLowerCase(headerVec[0]);
                    std::string headerContent = util::toLowerCase(headerVec[1]);

                    defaultHeaderKeys.insert(headerKey);
                    headerAsMap[headerKey] = headerContent;
                    // headerList = curl_slist_append(headerList, header.c_str());
                    startPos = endPos + 1; // Move past "\n"
                }
            }

            size_t startPos = 0;
            while (startPos != std::string::npos && startPos < inputHeaders.size())
            {
                size_t endPos = inputHeaders.find("\n", startPos);
                if (endPos == std::string::npos)
                {
                    endPos = inputHeaders.size(); // Use cDefaultHeaders.size() instead of headers.size()
                }
                std::string header = inputHeaders.substr(startPos, endPos - startPos);
                // need some try catches in here!!!!!!
                std::vector<std::string> headerVec = util::split(header, ":");
                std::string headerKey = headerVec[0];
                std::string headerContent = headerVec[1];

                headerAsMap[headerKey] = headerContent;
                alreadyAdded.insert(headerKey);
                defaultHeaderKeys.erase(headerKey);

                headerList = curl_slist_append(headerList, header.c_str());
                startPos = endPos + 1; // Move past "\n"
            }




            for (const std::string &remainingHeaders : defaultHeaderKeys)
            {

                headerList = curl_slist_append(headerList, remainingHeaders.c_str());
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        }

    */

    if ((headers.find(cuseragent) != headers.end()) && inputHeaders == "")
    {
        cDefaultHeaders = headers[cuseragent];

        // Set headers
        struct curl_slist *headerList = nullptr;
        size_t startPos = 0;
        while (startPos != std::string::npos && startPos < cDefaultHeaders.size())
        {
            size_t endPos = cDefaultHeaders.find("\n", startPos);
            if (endPos == std::string::npos)
            {
                endPos = cDefaultHeaders.size(); // Use cDefaultHeaders.size() instead of headers.size()
            }
            std::string header = cDefaultHeaders.substr(startPos, endPos - startPos);
            headerList = curl_slist_append(headerList, header.c_str());
            startPos = endPos + 1; // Move past "\n"
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    else
    {

        // Set headers
        struct curl_slist *headerList = nullptr;
        size_t startPos = 0;
        while (startPos != std::string::npos && startPos < inputHeaders.size())
        {
            size_t endPos = inputHeaders.find("\n", startPos);
            if (endPos == std::string::npos)
            {
                endPos = inputHeaders.size(); // Use cDefaultHeaders.size() instead of headers.size()
            }
            std::string header = inputHeaders.substr(startPos, endPos - startPos);
            headerList = curl_slist_append(headerList, header.c_str());
            startPos = endPos + 1; // Move past "\n"
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }

    if (http2PseudoHeaderOrder.find(cuseragent) != http2PseudoHeaderOrder.end())
    {
        chttp2PseudoHeaderOrder = http2PseudoHeaderOrder[cuseragent];

        curl_easy_setopt(curl, CURLOPT_HTTP2_PSEUDO_HEADERS_ORDER, chttp2settings.c_str());
    }

    if (http2Settings.find(cuseragent) != http2Settings.end())
    {
        chttp2settings = http2Settings[cuseragent];

        curl_easy_setopt(curl, CURLOPT_HTTP2_SETTINGS, chttp2settings.c_str());
    }

    if (http2WindowsUpdate.find(cuseragent) != http2WindowsUpdate.end())
    {
        chttp2WindowsUpdate = http2WindowsUpdate[cuseragent];

        curl_easy_setopt(curl, CURLOPT_HTTP2_WINDOW_UPDATE, chttp2WindowsUpdate);
    }

    if (ech.find(cuseragent) != ech.end())
    {
        cech = ech[cuseragent];

        curl_easy_setopt(curl, CURLOPT_ECH, cech.c_str());
    }

    if (tlsGrease.find(cuseragent) != tlsGrease.end() && tlsGrease[cuseragent])
    {
        ctlsGrease = tlsGrease[cuseragent];

        curl_easy_setopt(curl, CURLOPT_TLS_GREASE, ctlsGrease);
    }
    else
    {

        curl_easy_setopt(curl, CURLOPT_TLS_GREASE, false);
    }



//think this is fine as proxy will either have a value or be ""

        if(proxy != ""){
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
        }else{
curl_easy_setopt(curl, CURLOPT_PROXY, "");

        }
   

    // ATtm all browsers are null for CURLOPT_TLS_EXTENSION_ORDER opton
    curl_easy_setopt(curl, CURLOPT_TLS_EXTENSION_ORDER, NULL);

    std::string responseData;
    std::string error;
    // just in case something happened between now and last scan. Will restart curl object if not there.
    if (!curl)
    {

        curl = curl_easy_init();
    }

    if (curl)
    {

        if (firstrun)
        {

            // Set URL
            std::cout << url << std::endl;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Set method (GET or POST)
            if (method == "POST")
            {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);

                // std::string tempst = removeEscapes(requestData);
                // requestData = "{\"context\":{\"apiKeys\":[\"klevu-166850699917615799\"]},\"recordQueries\":[{\"id\":\"productList\",\"typeOfRequest\":\"SEARCH\",\"settings\":{\"query\":{\"term\":\"springbank\"},\"typeOfRecords\":[\"KLEVU_PRODUCT\"],\"limit\":\"12\",\"searchPrefs\":[\"searchCompoundsAsAndQuery\"],\"fallbackQueryId\":\"productListFallback\"},\"filters\":{\"filtersToReturn\":{\"enabled\":true,\"rangeFilterSettings\":[{\"key\":\"klevu_price\",\"minMax\":\"true\"}]}}},{\"id\":\"productListFallback\",\"typeOfRequest\":\"SEARCH\",\"isFallbackQuery\":\"true\",\"settings\":{\"query\":{\"term\":\"*\"},\"typeOfRecords\":[\"KLEVU_PRODUCT\"],\"limit\":\"12\",\"searchPrefs\":[\"excludeDescription\",\"searchCompoundsAsAndQuery\"],\"sort\":\"RELEVANCE\"}},{\"id\":\"noResultsFoundPopularProductList\",\"typeOfRequest\":\"SEARCH\",\"settings\":{\"query\":{\"term\":\"*\"},\"typeOfRecords\":[\"KLEVU_PRODUCT\"],\"limit\":4,\"searchPrefs\":[\"ignoreManualBoosting\"]}}]}";

                if (requestData != "")
                {
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
                }
            }
            else if (method != "GET")
            {
                error = "Invalid method. Only GET and POST supported.";
                return std::make_pair(responseData, error);
            }
        }

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // set default headers to reset then

        // Set data callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            if (res == CURLE_OPERATION_TIMEDOUT)
            {
                error = "Timeout error";
            }
            else
            {
                error = curl_easy_strerror(res);
            }
        }
        else
        {
            error = "Ok";
        }

        // Cleanup needs to be moved else where as we want it to stay open if possible
        //  curl_easy_cleanup(curl);
    }
    else
    {
        error = "Failed to initialize libcurl.";
    }

    std::cout << error << std::endl;
    std::cout << responseData << std::endl;

    return std::make_pair(error, responseData);
}

size_t chrome::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data)
{
    size_t totalSize = size * nmemb;
    data->append((char *)contents, totalSize);
    return totalSize;
}

std::string chrome::removeEscapes(const std::string &str)
{
    std::string unescapedStr;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == '\\' && i + 1 < str.size() && str[i + 1] == '\"')
        {
            unescapedStr += '"';
            ++i; // Skip the escape character
        }
        else
        {
            unescapedStr += str[i];
        }
    }
    return unescapedStr;
}