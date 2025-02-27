
#include "siteScraper.h"
#include <algorithm>

// #include firefox.h
#include <ctime>
#include "Browser.h"
// #include "include/rapidjson/document.h"
#include <algorithm>

#include <iostream>
#include <vector>
#include <algorithm>


#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif



// #include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-windows/include/gumbo.h"
//  #include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-windows-static/include/lexbor/html/parser.h"
//  #include "C:/Users/Matt Jayne/Desktop/VS Code Project files/vcpkg/installed/x64-windows-static/include/lexbor/core/core.h"

// #include "ConfigClass.h"
// need to add linux and windows libraries in here
lxb_status_t find_callback(lxb_dom_node_t *node, lxb_css_selector_specificity_t spec,
                           void *ctx)
{
    std::vector<lxb_dom_node_t *> *matchedNodes = reinterpret_cast<std::vector<lxb_dom_node_t *> *>(ctx);

    // Store the matched node in the vector
    matchedNodes->push_back(node);

    // Return LXB_STATUS_OK to continue searching for more matches
    return LXB_STATUS_OK;
}

siteScraper::siteScraper(std::atomic<bool> &atom, std::atomic<bool> &configChangedFlag, std::list<ConfigClass::scraperSettings> &config, std::string type, std::string &moderator_channel, std::string &moderator_channel_telegram, int threadnumber, std::string &VPNrebootCommand) // to add realease date mode as var
{
    std::string message = "Starting scraper: " + std::to_string(threadnumber);
    Logger::logger(threadnumber,"Misc",type,message);
    Logger::logger(0,"general","general","Starting scraper: " + std::to_string(threadnumber));

    std::string t = "no";
    std::string *releaseDateMode = &t;
    // creating objects that transcend all functions



    chrome chrome1; // might need to define seperate Safari but think it uses same code just different cyphers so can maybe use chrome1 for that. i've assumeed that for now
    // Firefox firefox;     // curl editted for it's tls and http2 fingerprint to look like firefox (also have headers stored in here for the various useragents)
    // JSbrowser jsBrowser; // playwrite running in python via cPython where data is passed back in pipes

    Browser *scrapingBrowser;

    reTryableErrors = {"500"};

    // Looping to extract using data before running. (frequency for min sleep time and browser type for setting up curl objects or at least their wrappers being chrome1/firefox.)

    std::list<int> frequencyList;

    Logger::logger(threadnumber,"Misc",type,"Initiailising browsers");

    for (ConfigClass::scraperSettings &setting : config)
    {
        // if Chrome or if doesnt exist default will be chrome1 and if it isnt already setup.
        if (((setting.stringSettings.find("browser") == setting.stringSettings.end()) || (setting.stringSettings.find("browser") != setting.stringSettings.end() && setting.stringSettings["browser"] == "chrome")) && !chrome1.ready)
        {

            chrome1.init();
        }
        /*  else if ((setting.stringSettings.find("browser") != setting.stringSettings.end() && setting.stringSettings["browser"] == "firefox") && !firefox.ready)
          {

              firefox.init();
          }
          else if ((setting.stringSettings.find("browser") != setting.stringSettings.end() && setting.stringSettings["browser"] == "dynamic") && !jsBrowser.ready)
          {

              jsBrowser.init();
          }
          */
        // uncomment once firefox and python browser built

        frequencyList.push_back(setting.intSettings["frequency"] / setting.intSettings["offsetMax"]);
    }
    auto minElement = std::min_element(frequencyList.begin(), frequencyList.end());
    int minWait = 0;
    if (minElement != frequencyList.end())
    {
        minWait = *minElement; // Dereference the iterator to get the value
        Logger::logger(threadnumber,"Misc",type,"Scraper loop sleeptime: " + std::to_string(minWait));
      //  std::cout << "Minimum element in frequencyList: " << minWait << std::endl;
    }
    else
    {
        std::cout << "frequencyList is empty." << std::endl;
        Logger::logger(threadnumber,"Misc",type,"ERROR: frequencyList is empty.");
    }

    std::string emptyString = "";
    std::string defaultScannerType = "GET";

    // int loop_counter=0;

    while (atom.load()) // might swap this to a mutex. Not convinced on the thread safety of this once i start dynamically adjusting number of threads. vector of mutex seems safer.
    {



        // make a copy of config here as it may change. will have another atomic bool to flag if it gets changed.

        int loopCounter = 0;
        int numberOfScans = 0;
        int numberOfScansPython = 0;
        std::string browserDefault = "chrome";

        int total_scans = frequencyList.size();

        auto currentTime = std::chrono::system_clock::now();
        startTime = std::chrono::system_clock::now();

        for (ConfigClass::scraperSettings &site : config)
        {

            // settin up variables make it easier to name throughout code.
            link = &site.stringSettings["link"];
            name = &site.name;

            Logger::logger(threadnumber,"Misc",type,"Site: " + *name);

            channel = &site.stringSettings["channel"];
            channelTelegram = &site.stringSettings["channelTelegram"];

            // set default values here.
            if (util::hasKey(site.stringSettings, "filter"))
            {
                filter = &site.stringSettings["filter"];
            }
            else
            {

                filter = &emptyString;
            }

            if (util::hasKey(site.stringSettings, "requestData"))
            {
                requestData = &site.stringSettings["requestData"];
            }
            else
            {

                requestData = &emptyString;
            }

            if (util::hasKey(site.stringSettings, "replace"))
            {
                replace = &site.stringSettings["replace"];
            }

            if (util::hasKey(site.stringSettings, "useragent"))
            {
                useragent = &site.stringSettings["useragent"];
            }
            else
            {

                useragent = &emptyString;
            }

            if (util::hasKey(site.stringSettings, "proxy"))
            {
                proxy = &site.stringSettings["proxy"];
            }
            else
            {

                proxy = &emptyString;
            }

            if (util::hasKey(site.stringSettings, "requestType"))
            {
                requestType = &site.stringSettings["requestType"];
            }
            else
            {

                requestType = &defaultScannerType;
            }
            if (util::hasKey(site.stringSettings, "pythonScriptName"))
            {
                pythonScriptName = &site.stringSettings["pythonScriptName"];
            }
            else
            {

                pythonScriptName = &emptyString;
            }



            if (util::hasKey(site.stringSettings, "pyFunctionName"))
            {
                pyFunctionName = &site.stringSettings["pyFunctionName"];
            }
            else
            {

                pyFunctionName = &emptyString;
            }




            scrapingBrowser = &chrome1;

            if (util::hasKey(site.stringSettings, "browser"))
            {
                if (site.stringSettings["browser"] == "firefox")
                {

                    scrapingBrowser = &chrome1;
                }
                else if (site.stringSettings["browser"] == "chrome")
                {

                    scrapingBrowser = &chrome1;
                };
            }
            else
            {

                scrapingBrowser = &chrome1;
            }

            if (util::hasKey(site.stringSettings, "sendLink"))
            {
                useragent = &site.stringSettings["sendLink"];
            }
            else
            {

                sendLink = &emptyString;
            }

            if (util::hasKey(site.stringSettings, "headers"))
            {
                headers = &site.stringSettings["headers"];
            }
            else
            {

                headers = &emptyString;
            }

            frequency = &site.intSettings["frequency"];
            offset = &site.intSettings["offset"];
            offsetMax = &site.intSettings["offsetMax"];
            maxOutput = &site.intSettings["maxOutput"];
            timeout = &site.intSettings["timeout"];

            functions = &site.vectorSettings["functions"];

            if (!functions->empty())
            {
                firstFunc = &(*functions)[0];
            }

            // std::string firstFunc = (*functions)[0];
            selectors = &site.vectorSettings["selectors"];

            if (util::hasKey(site.vectorSettings, "prefix"))
            {
                prefix = &site.vectorSettings["prefix"];
            }

            if (util::hasKey(site.vectorSettings, "suffix"))
            {
                suffix = &site.vectorSettings["suffix"];
            }

            if (util::hasKey(site.vectorSettings, "botIdentifiers"))
            {
                botIdentifiers = &site.vectorVectorPair["botIdentifiers"];
            }

            // setting up variables needed for timing
            auto currentTime = std::chrono::system_clock::now();

            // check pointers are not null.
            if (lastTime.find((*name)) != lastTime.end() && (currentTime - lastTime[*name]) < std::chrono::seconds(*frequency * 780 / 1000))
            {

               // std::cout << "Skipping: " << *name << " as not enough time has passed" << std::endl;
                Logger::logger(threadnumber,*name,type,"Skipping: " + *name + " as not enough time has passed");
            }
            else if (loopCounter % (*offsetMax) != (*offset) - 1)
            {

                //std::cout << "Skipping: " << *name << " as it's not it's loop turn yet has passed to scan again" << std::endl;
                Logger::logger(threadnumber,*name,type,"Skipping: " + *name + " as it's not it's loop turn yet has passed to scan again");
            }
            else
            {

                // running scrapers:
                int errorConuter = 0;
                std::pair<std::string, std::string> dataOutput = std::make_pair("", "");
                int botDetectedCounter = 0;
                bool firstRun = true;

                while (errorConuter < 2 && (firstRun || std::find(reTryableErrors.begin(), reTryableErrors.end(), dataOutput.first) != reTryableErrors.end()))
                {
                    // fix this:
                    //code to determine whether pythonmodule, externalscript or built in curl is used. 
                    dataOutput = (*scrapingBrowser).Request(firstRun, (*link), (*requestType), (*headers), (*useragent), (*proxy), (*timeout), moderator_channel, moderator_channel_telegram, (*requestData), threadnumber);
                    firstRun = false;

                    if (dataOutput.first != "Ok")
                    {

                        errorConuter++;


                            if(botDetection(dataOutput.second,botIdentifiers)){

                                botDetectedCounter = 1;
                        

                            }
                        // bot detection function will go here. if true will send vpn reboot and try once more. if that fails will break.
                    }
                    else
                    {
                        break;
                    }
                }

                if (dataOutput.first == "Ok") // add for errors
                {
                    if ((*firstFunc).find("xml") != std::string::npos)
                    {

                        output = extractXML(dataOutput, (*selectors), (*functions), (*prefix), (*suffix));
                    }
                    else if ((*firstFunc).find("json") != std::string::npos)
                    {

                        output = extractJSON(dataOutput, (*selectors), (*functions), (*prefix), (*suffix));
                    }
                    else if ((*firstFunc).find("html") != std::string::npos)
                    {

                        output = extractHTML(dataOutput, (*selectors), (*functions), (*prefix), (*suffix));
                    }
                }
                else
                {
                    std::cout << "Error: " << name << "" << dataOutput.first << std::endl;
                }

                if (output.size() == 0 || (output.size() == 1 && output[0] == ""))
                {
                    std::cout << "Moving onto next as no items founds" << std::endl;

                    std::string filepath = "data/" + type + "/" + (*name) + ".txt";

                    std::ifstream file(filepath);

                    if (!file.good())
                    { // Check if the file doesn't exist or is not accessible
                        std::vector<std::string> temp = {" "};
                        util::write(filepath, temp);
                    }

                    if (oldOutputList.find(*name) == oldOutputList.end() && util::readLines("data/" + type + "/" + (*name) + ".txt")[0] == " " && util::readLines("data/" + type + "/" + (*name) + ".txt").size() == 0)
                    {

                        std::vector<std::string> temp = {" "};
                        oldOutputList[*name] = {" "};
                    }

                    if (oldOutputList.find(*name) == oldOutputList.end() && (util::readLines("data/" + type + "/" + (*name) + ".txt"))[0] != " ")
                    {

                        std::vector<std::string> temp = {" "};
                        oldOutputList[*name] = util::readLines("data/" + type + "/" + (*name) + ".txt");
                    }

                    if (oldOutputList.find(*name) != oldOutputList.end() && oldOutputList[*name].size() > 1)
                    {

                        std::cout << "Bot detected, Connection issue or layout change on " << name << "Error log will be created to help identify the issue" << name << std::endl;

                        // write to error file to write this later. WIll write the whole page source from getData if i can. Need to pass in pointers for that so can access out where quickly swap between.
                    }
                }
                else
                {
                    for (int bb = 0; bb < output.size(); bb++)
                    {
                        if (output[bb].find("&_sid=") != std::string::npos || output[bb].find("?_pos=") != std::string::npos)
                        {
                            if (output[bb].find("&_sid=") != std::string::npos)
                            {
                                try
                                {
                                    output[bb] = output[bb].substr(0, output[bb].find("&_sid="));
                                }
                                catch (const std::exception &ignore)
                                {
                                    std::cout << ignore.what() << std::endl;
                                }
                            }

                            if (output[bb].find("?_pos=") != std::string::npos)
                            {
                                try
                                {
                                    output[bb] = output[bb].substr(0, output[bb].find("?_pos="));
                                }
                                catch (const std::exception &ignore)
                                {
                                    std::cout << ignore.what() << std::endl;
                                }
                            }
                        }
                    }

                    std::string filepath = "data/" + type + "/" + (*name) + ".txt";

                    std::ifstream file(filepath);

                    if (!file.good() || file.peek() == std::ifstream::traits_type::eof())
                    {
                        std::cout << "No files detected, first run for website: " << (*name)
                                  << " using " << type << " setting up files" << std::endl;

                        util::write(filepath, output);
                        oldOutputList[*name] = output;
                    }
                    else
                    {
                        if (oldOutputList.find(*name) != oldOutputList.end())
                        {
                            oldOutput = oldOutputList[*name];
                        }
                        else
                        {

                            oldOutput = util::readLines(filepath);
                            oldOutputList[*name] = oldOutput;
                        }

                        std::vector<std::string> newitems = util::vecCompare(output, oldOutputList[*name]);

                        if (!newitems.empty() && newitems.size() < (*maxOutput) && !(newitems.size() == 1 && newitems[0] == ""))
                        {
                            std::vector<std::string> filter_positive;
                            std::vector<std::string> filter_negative;

                            if (*filter != "")
                            {
                                // std::string filter = data["filter"];
                                std::vector<std::string> filters;

                                size_t pos = 0;
                                while ((pos = (*filter).find(',')) != std::string::npos)
                                {
                                    filters.push_back((*filter).substr(0, pos));
                                    (*filter).erase(0, pos + 1);
                                }
                                filters.push_back((*filter));

                                for (const auto &f : filters)
                                {
                                    if (f[0] == '!')
                                    {
                                        filter_negative.push_back(f.substr(1));
                                    }
                                    else
                                    {
                                        filter_positive.push_back(f);
                                    }
                                }
                            }

                            for (const auto &newitem : newitems)
                            {
                                if (*filter != "")
                                {
                                    //  std::string filter = data["filter"];
                                    bool toSend = checkFilter(newitem, filter_positive, filter_negative);

                                    if (toSend)
                                    {
                                        if (*sendLink != "" && *sendLink == "yes")
                                        {
                                            if ((*firstFunc).find("external") != std::string::npos && (*link).find("http") != std::string::npos)
                                            {
                                                size_t startIndex = (*link).find("https://");
                                                size_t endIndex = (*link).find(' ', startIndex);
                                                if (endIndex == std::string::npos)
                                                {
                                                    endIndex = (*link).length();
                                                }
                                                // messageSender(link.substr(startIndex, endIndex - startIndex) + " " + newitem,
                                                //     message_channelitem, guildID, message_channelitem_telegram);
                                            }
                                            else
                                            {
                                                // messageSender(link + " " + newitem, message_channelitem, guildID, message_channelitem_telegram);
                                            }
                                        }
                                        else
                                        {
                                            // messageSender(newitem, message_channelitem, guildID, message_channelitem_telegram);
                                        }
                                    }
                                }
                                else
                                {

                                    if (*sendLink != "")
                                    {
                                        if (firstFunc->find("external") != std::string::npos && (*link).find("https://") != std::string::npos)
                                        {
                                            int startIndex = (*link).find("https://");
                                            int endIndex = (*link).find(" ", startIndex);

                                            if (endIndex == -1)
                                            {
                                                endIndex = (*link).length();
                                            }

                                            // Logger::messageSender(link.substr(startIndex, endIndex - startIndex) + " " + newitems[a],
                                            //   message_channelitem,
                                            //   guildID, message_channelitem_telegram);
                                        }
                                        else
                                        {
                                            //  Logger::messageSender(link + " " + newitems[a],
                                            //  message_channelitem,
                                            // guildID, message_channelitem_telegram);
                                        }
                                    }
                                    else
                                    {
                                        // Logger::messageSender(newitems[a], message_channelitem,
                                        //    guildID, message_channelitem_telegram);
                                    }
                                }

                                /*
                                 log = log + newitems.size() + "new item(s) found for " + site + "\n";
                                            Logger.logger(threadnumber, type,
                                                    newitems.size() + "new item(s) found for " + site);

                                            // system log to note that the oldOutput data stored in the file name keys[i] is
                                            // being replaced with the new one so next time we run the program we can
                                            // compare to the new output from website
                                            // System.out.println("Replacing output data for next scan");

                                            log = log + "Replacing output data for next  \n\n";
                                            log = log + "-------------------------\n";

                                            Logger.logger(threadnumber, type,
                                                    "Replacing output data for next");
                                            Logger.logger(threadnumber, type,
                                                    "-------------------------");

                                            write(("data/" + type + "/" + site + ".txt"), output);

                                            oldOutputList.put(site, output); // think this is already done regardless
                                */
                                util::write(("data/" + type + "/" + (*name) + ".txt"), output);
                                oldOutputList[*name] = output; // optimise

                            } // this needs to move up. no idea where...
                        }
                        else if (newitems.size() > *maxOutput - 1)
                        {

                            // Logger.messageSender(
                            //                                            "More than " + maxOutput + " items found on page see" + link,
                            //                                          message_channelitem, guildID, message_channelitem_telegram);

                            util::write(("data/" + type + "/" + (*name) + ".txt"), output);

                            oldOutputList[*name] = output; // optimise
                        }
                        else
                        {

                            std::cout << "Nothing new" << std::endl;
                        }
                    }

                    /// add catch here.
                }

                // compare the output to oldOutput

                // add try's throughout all of this.
            }

            // error here.
            if (lastTime.find(*name) == lastTime.end() ||
                (lastTime.at(*name).time_since_epoch().count() == 0 ||
                 ((std::chrono::system_clock::now() - lastTime.at(*name)) > std::chrono::milliseconds(*frequency * 780)) &&
                     ((loopCounter % (*offsetMax)) == *offset - 1)))
            {
                lastTime[*name] = std::chrono::system_clock::now();
            }
        }

        long SleepTime;
        if (releaseDateMode != nullptr && *releaseDateMode == "Yes")
        {

            auto currentTime2 = std::chrono::system_clock::now();

            // Calculate the time difference in milliseconds since startTime //startTime's wrong look into
            auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime2 - startTime);

            // Calculate sleepDouble using std::chrono::duration and arithmetic
            double sleepDouble = (static_cast<double>(minWait) * 1000.0 * (static_cast<double>(numberOfScans) + static_cast<double>(numberOfScansPython))) /
                                     static_cast<double>(total_scans) -
                                 static_cast<double>(timeDifference.count());
            SleepTime = std::max<long>(static_cast<long>(std::round(sleepDouble)), 2000L);
        }
        else
        {
            SleepTime = std::max<long>(0L, (minWait * 1000 - (numberOfScans * 2000)) - (numberOfScansPython * 500));
        }

        std::string runtime[1];
        int totalScans = numberOfScans;
        // log this shit.

        auto currentTime3 = std::chrono::system_clock::now();
        auto timeDifference2 = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime3 - startTime);
        double seconds = timeDifference2.count();

        // Print the duration in seconds
        std::cout << "Duration: " << seconds << " miliseconds" << std::endl;

        std::cout << "Loop done" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));

        loopCounter++;

    } // end of while loop for checking if should still be running

} // end of constructor

std::vector<std::string> siteScraper::extractHTML(std::pair<std::string, std::string> &data, std::vector<std::string> &inputSelectors, std::vector<std::string> &functions, std::vector<std::string> &prefix, std::vector<std::string> &suffix)
{

    std::vector<std::string> output;
    std::vector<std::vector<std::string>> selectorOutputs;

    /*
        GumboQueryDocument doc(data.second.c_str());

        int outputSize = -1;
        for (int j = 0; j < selectors.size(); j++)
        {
            std::vector<GumboNode *> elements = doc.find(selectors[j]);

            for (size_t i = 0; i < elements.size(); ++i)
            {

                //if here for htmler2json. pass the below to json funciton
                std::string text = extractTextAndAttributes(elements[i], function);

                if (j == selectorOutputs.size())
                { // should be fine as will never skip one but tihnk about just in case there is scenario where it jumps aehad one.
                    std::vector<std::string> temp = {text};
                    selectorOutputs.push_back(temp);
                }
                else
                {
                    selectorOutputs[j].push_back(text);
                }
            }

            */

    lxb_html_document_t *document;
    lxb_css_parser_t *parser;
    lxb_status_t status;

    document = lxb_html_document_create();
    status = lxb_html_document_parse(document, reinterpret_cast<const lxb_char_t *>(data.second.c_str()), data.second.length());
    if (status != LXB_STATUS_OK)
    {
        // return EXIT_FAILURE;
        std::cout << "error" << std::endl;
    }

    parser = lxb_css_parser_create();
    status = lxb_css_parser_init(parser, NULL);
    if (status != LXB_STATUS_OK)
    {
        // return EXIT_FAILURE;
        std::cout << "error" << std::endl;
    }

    int outputSize = -1;
    for (int j = 0; j < inputSelectors.size(); j++)
    {
        lxb_selectors_t *selectors;
        selectors = lxb_selectors_create();
        status = lxb_selectors_init(selectors);
        if (status != LXB_STATUS_OK)
        {
            // return EXIT_FAILURE;
            std::cout << "error" << std::endl; // swap to return output the empty vect
        }

        // Parse CSS selector
        lxb_css_selector_list_t *list;
        list = lxb_css_selectors_parse(parser, reinterpret_cast<const lxb_char_t *>(inputSelectors[j].c_str()), inputSelectors[j].length());
        if (parser->status != LXB_STATUS_OK)
        {
            // return EXIT_FAILURE;
            std::cout << "error" << std::endl;
        }

        lxb_dom_node_t *body;
        body = lxb_dom_interface_node(document);

        // lxb_dom_collection_t *elements = lxb_dom_collection_make(lxb_dom_interface_node(document)->owner_document, 0);

        std::vector<lxb_dom_node_t *> elements;
        status = lxb_selectors_find(selectors, body, list, find_callback, &elements);
        if (status != LXB_STATUS_OK)
        {
            // return EXIT_FAILURE;
            std::cout << "error" << std::endl;
        }

        // lxb_html_element_t *element = lxb_html_document_query_selector(document, lxb_html_document_document_element(document), selectors);
        std::string text = "";
        for (size_t i = 0; i < elements.size(); ++i)
        {
            lxb_dom_node_t *node = elements[i];

            text = extractContent(node, functions[j]);

            // code to extract the content here
        }

        if (j == selectorOutputs.size())
        {
            selectorOutputs.push_back({text});
        }
        else
        {
            selectorOutputs[j].push_back(text);
        }

        // element = lxb_html_document_query_selector(document, lxb_html_node_next(element), selectors);

        (void)lxb_selectors_destroy(selectors, true);
        // lxb_dom_collection_destroy(elements, true);
        lxb_css_selector_list_destroy_memory(list);

        // lxb_html_document_destroy(document);

        // Clean up resources

        if (outputSize = -1)
        {

            outputSize = selectorOutputs[j].size();
        }
        else
        {

            if (selectorOutputs[j].size() != outputSize)
            {

                // Throw errors as output will be nonsense.
            }
        }
    }

    lxb_html_document_destroy(document);

    (void)lxb_css_parser_destroy(parser, true);

    // check all same size. is done above now on the fly.

    // append the outputs.

    for (int x = 0; x < outputSize; x++)
    {
        std::stringstream lineStream;
        for (int y = 0; y < selectorOutputs.size(); y++)
        {
            if (!prefix[y].empty())
            {
                lineStream << prefix[y] << " "; // Add space after prefix
            }
            lineStream << selectorOutputs[y][x] << " "; // No space after selectorOutputs
            if (!suffix[y].empty())
            {
                lineStream << suffix[y]; // No space after suffix
            }
        }
        output.push_back(lineStream.str());
    }

    std::sort(output.begin(), output.end());
    return output;
}
/*std::string siteScraper::extractTextAndAttributes(GumboNode *node, std::string function)
{

    std::string attributeName;
    std::string actualFunciton;
    // move this shit to outside function. recurssion will be doign it over and over when it's never going to change.
    if (function.find("attrib") != std::string::npos)
    {

        std::istringstream iss(function);
        std::vector<std::string> tokens;
        std::string token;

        // Split the string by ':'
        while (std::getline(iss, token, ':'))
        {
            tokens.push_back(token);
        }

        // need to sort this once i decide on config layout. v rough atm.
        std::string actualFunction = tokens[0];

        if (token.size() > 1)
        {

            actualFunction = tokens[1];
        }
    }

    if (actualFunction == "search")
    {

        if (node->type == GUMBO_NODE_TEXT)
        {
            return std::string(node->v.text.text);
        }
        else if (node->type == GUMBO_NODE_ELEMENT)
        {
            std::string content = "";
            GumboVector *children = &node->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i)
            {
                const std::string childContent = extractTextAndAttributes(static_cast<GumboNode *>(children->data[i]), function);
                content += childContent;
            }
            return content;
        }
    }
    else if (actualFunction == "attrib")
    {

        GumboAttribute *attribute = gumbo_get_attribute(&node->v.element.attributes, attributeName.c_str());
        return attribute->value;
    }
    else
    {

        // return error.
    }
}
*/

std::string siteScraper::extractContent(lxb_dom_node_t *node, const std::string &function)
{
    if (function.find("search") != std::string::npos)
    {
        // If function contains "search", return the text content of the node
        // Assuming the node is an element node

        // Get the text content of the element
        // Note: You may need to handle text content differently based on your exact requirements
        size_t text_length;
        const lxb_char_t *text_content = lxb_dom_node_text_content(node, &text_length);
        if (text_content != nullptr)
        {
            return std::string(reinterpret_cast<const char *>(text_content));
        }
    }
    else if (function.find("attrib:") != std::string::npos)
    {
        // If function contains "attrib:", extract the attribute specified
        // Split function using ":"
        size_t pos = function.find(":");
        if (pos != std::string::npos && pos + 1 < function.size())
        {
            // Get the attribute name
            std::string attribute = util::split(function, ":")[2];

            // function.substr(pos + 1);

            // Get the attribute value from the node
            size_t attribute_value_length;
            // Get the attribute value from the node
            const lxb_char_t *attribute_value = lxb_dom_element_get_attribute(lxb_dom_interface_element(node), reinterpret_cast<const lxb_char_t *>(attribute.c_str()), attribute.length(), &attribute_value_length);
            if (attribute_value != nullptr)
            {
                return std::string(reinterpret_cast<const char *>(attribute_value), attribute_value_length);
            }
        }
    }
    // Return empty string if function does not match any case
    return "";
}

/*std::string siteScraper::site_scraper_extract_text_and_attributes(const std::string &function, lexbor_dom_node_t *node)
{
    if (node == nullptr)
    {
        // Handle invalid input
        return "";
    }

    const char *function_cstr = function.c_str();
    const char *actual_function = function_cstr;
    const char *attribute_name = nullptr;

    // Check if the function contains 'attrib'
    if (lexbor_str_data_cmp(function_cstr, strlen(function_cstr), "attrib", 6) == 0)
    {
        lexbor_str_t *tokens = lexbor_array_obj_make();
        if (tokens == nullptr)
        {
            // Handle memory allocation error
            return "";
        }

        lexbor_str_t *token = nullptr;
        lexbor_str_split(&token, function_cstr, (size_t)-1, (const lxb_char_t *)":", 1);
        if (token == nullptr)
        {
            lexbor_array_obj_destroy(tokens, true);
            // Handle memory allocation error
            return "";
        }

        lexbor_array_obj_push(tokens, (void *)token);
        actual_function = lexbor_str_data(token);

        // Extract attribute name
        token = lexbor_array_obj_get(tokens, 1);
        if (token != nullptr)
        {
            attribute_name = lexbor_str_data(token);
        }
    }

    // Check actual function
    if (lexbor_str_data_cmp(actual_function, strlen(actual_function), "search", 6) == 0)
    {
        if (node->type == LXB_DOM_NODE_TYPE_TEXT)
        {
            return std::string((const char *)node->text.data, node->text.length);
        }
        else if (node->type == LXB_DOM_NODE_TYPE_ELEMENT)
        {
            // Implementation for "search" function
        }
    }
    else if (lexbor_str_data_cmp(actual_function, strlen(actual_function), "attrib", 6) == 0 && attribute_name != nullptr)
    {
        lexbor_dom_attr_t *attr = lexbor_dom_attr_by_name(&node->element.attributes, attribute_name, strlen(attribute_name));
        if (attr != nullptr)
        {
            return std::string((const char *)attr->value.data, attr->value.length);
        }
    }
    else
    {
        // Return error.
        return "";
    }

    // Placeholder return value
    return "";
}
*/
std::vector<std::string> siteScraper::extractJSON(std::pair<std::string, std::string> &data, std::vector<std::string> &selectors, std::vector<std::string> &functions, std::vector<std::string> &prefix, std::vector<std::string> &suffix)
{

    std::vector<std::string> output;
    std::vector<std::vector<std::string>> selectorOutputs;

    std::string jsonString = data.second;
    Document document;
    document.Parse(jsonString.c_str());

    if (document.IsObject())
    {
        int outputSize = -1;

        std::string jsonArrayKey;
        std::string jsonAttribute;

        std::vector<std::string> jsonArrayKeysOld;
        Value *resultsArray = nullptr;

        for (int j = 0; j < selectors.size(); j++)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(selectors[j]);
            while (std::getline(tokenStream, token, ':'))
            {
                tokens.push_back(token);
            }

            jsonArrayKey = tokens[0];
            jsonAttribute = tokens[1];

            std::vector<std::string> jsonArrayKeys;
            std::vector<std::string> jsonAttributeKeys;

            size_t pos = jsonArrayKey.find('.');
            if (pos != std::string::npos)
            {
                std::string token;
                size_t start = 0;
                while ((pos = jsonArrayKey.find('.', start)) != std::string::npos)
                {
                    token = jsonArrayKey.substr(start, pos - start);
                    jsonArrayKeys.push_back(token);
                    start = pos + 1;
                }
                jsonArrayKeys.push_back(jsonArrayKey.substr(start));
            }
            else
            {
                jsonArrayKeys.push_back(jsonArrayKey);
            }

            pos = jsonAttribute.find('.');
            if (pos != std::string::npos)
            {
                std::string token;
                size_t start = 0;
                while ((pos = jsonAttribute.find('.', start)) != std::string::npos)
                {
                    token = jsonAttribute.substr(start, pos - start);
                    jsonAttributeKeys.push_back(token);
                    start = pos + 1;
                }
                jsonAttributeKeys.push_back(jsonAttribute.substr(start));
            }
            else
            {
                jsonAttributeKeys.push_back(jsonAttribute);
            }

            if (jsonArrayKeysOld != jsonArrayKeys)
            {
                resultsArray = siteScraper::jsonKeyExtractor(jsonArrayKeys, document);
            }

            std::string debug = toJsonString(*resultsArray);
            // std::cout << debug << std::endl;

            Value products(kArrayType);

            if (jsonArrayKeys.size() == 1 && jsonArrayKeys[0].empty())
            {
                products.PushBack(*resultsArray, document.GetAllocator());

                debug = toJsonString(products);
                // std::cout << debug << std::endl;
            }
            else if (resultsArray != nullptr && !resultsArray->IsNull())
            {
                if (resultsArray->IsArray())
                {
                    products.CopyFrom(*resultsArray, document.GetAllocator());
                    debug = toJsonString(products);
                    //  std::cout << debug << std::endl;
                }
                else if (resultsArray->IsObject())
                {
                    if (resultsArray->HasMember(jsonArrayKeys.back().c_str()) && (*resultsArray)[jsonArrayKeys.back().c_str()].IsArray())
                    {
                        products.CopyFrom((*resultsArray)[jsonArrayKeys.back().c_str()], document.GetAllocator());
                        debug = toJsonString(products);
                        //  std::cout << debug << std::endl;
                    }
                }
            }

            for (SizeType z = 0; z < products.Size(); z++)
            {
                std::vector<std::string> outputTemp;
                Value *content;
                content = siteScraper::jsonKeyExtractor(jsonAttributeKeys, products[z]);
                debug = toJsonString(*content);
                if (content != nullptr && !content->IsNull())
                {

                    std::string value = toJsonString(*content);

                    if (content->IsString())
                    {
                        if (!value.empty() && value.front() == '"' && value.back() == '"')
                        {
                            // If it starts and ends with ", remove them
                            outputTemp.push_back(value.substr(1, value.length() - 2));
                        }
                        else
                        {
                            // If it doesn't start and end with ", keep it as it is
                            outputTemp.push_back(value);
                        }
                    }
                    else
                    {
                        // If it's a long, no need to remove double quotes
                        outputTemp.push_back(value);
                    }

                    if (j == selectorOutputs.size())
                    {
                        selectorOutputs.push_back(outputTemp);
                    }
                    else
                    {
                        // removing "" at start and end
                        if (content->IsString())
                        {

                            selectorOutputs[j].push_back(value.substr(1, value.length() - 2));
                        }
                        else
                        {

                            selectorOutputs[j].push_back(value);
                        }
                    }
                }
            }

            if (outputSize == -1)
            {
                outputSize = selectorOutputs[j].size();
            }
            else
            {
                if (selectorOutputs[j].size() != outputSize)
                {
                    // Handle error: output will be inconsistent as each selector isn't getting the same number of outputs.
                }
            }

            jsonArrayKeysOld = jsonArrayKeys;
        }

        for (int x = 0; x < outputSize; x++)
        {
            std::stringstream lineStream;
            for (int y = 0; y < selectorOutputs.size(); y++)
            {
                if (!prefix[y].empty())
                {
                    lineStream << prefix[y] << " "; // Add space after prefix
                }
                lineStream << selectorOutputs[y][x] << " "; // No space after selectorOutputs
                if (!suffix[y].empty())
                {
                    lineStream << suffix[y]; // No space after suffix
                }
            }
            // std::cout << "Here" << std::endl;
            output.push_back(lineStream.str());
        }
    }
    else
    {

        std::cout << "error with jsonobect" << std::endl;

        // try running botdetection at this point as well, as it will be the case. Given sometimes no error returned.
    }

    std::sort(output.begin(), output.end());
    return output;
}

Value *siteScraper::jsonKeyExtractor(const std::vector<std::string> &jsonArrayKey, Value &jsonObject)
{

    // need to add some mroe elses to manage the cases where members are missing to give more meaningful outputs
    if (jsonArrayKey.size() == 1)
    {
        const std::string &key = jsonArrayKey[0]; // make this more efficient maybe reversing vector order and just removing end
        if (jsonObject.HasMember(key.c_str()))
        {

            return &jsonObject[key.c_str()];
        }
        else if (key.find('[') != std::string::npos && jsonObject.HasMember(key.substr(0, key.find('[')).c_str()))
        {
            const std::string &arrayKey = key.substr(0, key.find('['));
            if (jsonObject[arrayKey.c_str()].IsArray())
            {
                const std::string &indexStr = key.substr(key.find('[') + 1, key.find(']') - key.find('[') - 1);
                int index = std::stoi(indexStr);
                Value &nestedArray = jsonObject[arrayKey.c_str()];
                if (index >= 0 && index < nestedArray.Size())
                {
                    return &nestedArray[index];
                }
            }
        }

        else
        {
            return nullptr;
        }
    }
    else
    {
        const std::string &key = jsonArrayKey[0];
        if (jsonObject.HasMember(key.c_str()))
        {
            Value &nestedObject = jsonObject[key.c_str()];
            std::vector<std::string> elementDown(jsonArrayKey.begin() + 1, jsonArrayKey.end()); // make this more efficient maybe reversing vector order and just removing end
            return jsonKeyExtractor(elementDown, nestedObject);
        }
        else if (key.find('[') != std::string::npos && jsonObject.HasMember(key.substr(0, key.find('[')).c_str()))
        {
            const std::string &arrayKey = key.substr(0, key.find('['));
            if (jsonObject[arrayKey.c_str()].IsArray())
            {
                const std::string &indexStr = key.substr(key.find('[') + 1, key.find(']') - key.find('[') - 1);
                int index = std::stoi(indexStr);
                Value &nestedArray = jsonObject[arrayKey.c_str()];
                if (index >= 0 && index < nestedArray.Size())
                {
                    Value &nestedObject = nestedArray[index];
                    std::vector<std::string> elementDown(jsonArrayKey.begin() + 1, jsonArrayKey.end());
                    return jsonKeyExtractor(elementDown, nestedObject);
                }
            }
        }
        return nullptr;
    }
    return nullptr;
}

std::vector<std::string> siteScraper::extractXML(std::pair<std::string, std::string> &data, std::vector<std::string> &selectors, std::vector<std::string> &functions, std::vector<std::string> &prefix, std::vector<std::string> &suffix)
{

    std::vector<std::string> output;
    std::vector<std::vector<std::string>> selectorOutputs;

    pugi::xml_document doc;
    doc.load_string(data.second.c_str());

    int outputSize = -1;
    for (int j = 0; j < selectors.size(); j++)
    {
        pugi::xpath_node_set elements = doc.select_nodes(selectors[j].c_str());

        for (size_t i = 0; i < elements.size(); ++i)
        {
            std::string text = elements[i].node().text().get();

            if (j == selectorOutputs.size())
            { // should be fine as will never skip one but tihnk about just in case there is scenario where it jumps aehad one.
                std::vector<std::string> temp = {text};
                selectorOutputs.push_back(temp);
            }
            else
            {
                selectorOutputs[j].push_back(text);
            }
        }

        if (outputSize = -1)
        {

            outputSize = elements.size();
        }
        else
        {

            if (elements.size() != outputSize)
            {

                // Throw errors as output will be nonsense.
            }
        }

        if (outputSize = -1)
        {

            outputSize = selectorOutputs[j].size();
        }
        else
        {

            if (selectorOutputs[j].size() != outputSize)
            {

                // Throw errors as output will be nonsense.
            }
        }
    }

    for (int x = 0; x < outputSize; x++)
    {
        std::stringstream lineStream;
        for (int y = 0; y < selectorOutputs.size(); y++)
        {
            if (!prefix[y].empty())
            {
                lineStream << prefix[y] << " "; // Add space after prefix
            }
            lineStream << selectorOutputs[y][x] << " "; // No space after selectorOutputs
            if (!suffix[y].empty())
            {
                lineStream << suffix[y]; // No space after suffix
            }
        }
        output.push_back(lineStream.str());
    }

    std::sort(output.begin(), output.end());
    return output;
}

std::string siteScraper::toJsonString(const Value &value)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    value.Accept(writer);
    return buffer.GetString();
}

bool siteScraper::botDetection(std::string &dataOutput, std::vector<std::pair<std::string, std::string>> *&botIdentifiers)
{

    if (botIdentifiers == nullptr)
    {

        return false;
    }

    lxb_html_document_t *document;
    lxb_css_parser_t *parser;
    lxb_status_t status;

    document = lxb_html_document_create();
    status = lxb_html_document_parse(document, reinterpret_cast<const lxb_char_t *>(dataOutput.c_str()), dataOutput.length());
    if (status != LXB_STATUS_OK)
    {
        // return EXIT_FAILURE;
        std::cout << "error" << std::endl;
    }

    parser = lxb_css_parser_create();
    status = lxb_css_parser_init(parser, NULL);
    if (status != LXB_STATUS_OK)
    {
        // return EXIT_FAILURE;
        std::cout << "error" << std::endl;
    }

    int outputSize = -1;
    for (int j = 0; j < botIdentifiers->size(); j++)
    {
        lxb_selectors_t *selectors;
        selectors = lxb_selectors_create();
        status = lxb_selectors_init(selectors);
        if (status != LXB_STATUS_OK)
        {
            // return EXIT_FAILURE;
            std::cout << "error" << std::endl; // swap to return output the empty vect
        }

        // Parse CSS selector
        lxb_css_selector_list_t *list;
        list = lxb_css_selectors_parse(parser, reinterpret_cast<const lxb_char_t *>((*botIdentifiers)[j].first.c_str()), (*botIdentifiers)[j].first.length());
        if (parser->status != LXB_STATUS_OK)
        {
            // return EXIT_FAILURE;
            std::cout << "error" << std::endl;
        }

        lxb_dom_node_t *body;
        body = lxb_dom_interface_node(document);

        // lxb_dom_collection_t *elements = lxb_dom_collection_make(lxb_dom_interface_node(document)->owner_document, 0);

        std::vector<lxb_dom_node_t *> elements;
        status = lxb_selectors_find(selectors, body, list, find_callback, &elements);
        if (status != LXB_STATUS_OK)
        {
            // return EXIT_FAILURE;
            std::cout << "error" << std::endl;
        }

        // lxb_html_element_t *element = lxb_html_document_query_selector(document, lxb_html_document_document_element(document), selectors);
        std::string text = "";
        for (size_t i = 0; i < elements.size(); ++i)
        {
            lxb_dom_node_t *node = elements[i];

            text = extractContent(node, "search");

            if (text.find((*botIdentifiers)[j].second) != std::string::npos)
            {

                return true;
            }

            // code to extract the content here
        }
    }

    return false;
}

bool siteScraper::checkFilter(const std::string &newitem, const std::vector<std::string> &positive, const std::vector<std::string> &negative)
{
    bool output = true;

    if ((negative.empty() || negative.size() == 0) && (positive.empty() || positive.size() == 0))
    {
        return true;
    }
    else if ((negative.empty() || negative.size() == 0) && !positive.empty())
    {
        for (const auto &pos : positive)
        {
            if (newitem.find(pos) != std::string::npos)
            {
                return true;
            }
        }
    }
    else if (!negative.empty() && (positive.empty() || positive.size() == 0))
    {
        for (const auto &neg : negative)
        {
            if (newitem.find(neg) != std::string::npos)
            {
                return false;
            }
        }
    }
    else
    {
        for (const auto &neg : negative)
        {
            if (newitem.find(neg) != std::string::npos)
            {
                return false;
            }
        }
        for (const auto &pos : positive)
        {
            if (newitem.find(pos) != std::string::npos)
            {
                return true;
            }
        }
    }

    return output;
}


std::pair<std::string, std::string> siteScraper::pythonScriptData(const std::string pythonScriptName, const std::string pyFunctionName, const bool retry, const std::string &url, const std::string &method,
    const std::string &headers,
    const std::string &useragent,
    const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber){


//need to move this to it's own thread so i can enforce a timeout in c++ and not just in python. 


        if (!pythonInitialized) {
            Py_Initialize();
            if (!Py_IsInitialized()) {
                std::cerr << "Failed to initialize Python interpreter!" << std::endl;
                return;
            }
            PyRun_SimpleString("import sys\nsys.path.append(\".\")");  // Ensure Python finds scripts
            pythonInitialized = true;
        }

        PyObject *pName = PyUnicode_DecodeFSDefault(pythonScriptName.c_str());
        PyObject *pModule = PyImport_Import(pName);
        Py_XDECREF(pName);
    
        if (!pModule) {
            std::cerr << "Failed to load Python module: " << pythonScriptName << std::endl;
            PyErr_Print();
            finalizePython();  // Restart Python on failure
            return {"Module load failed", ""};
        }
    
        PyObject *pFunc = PyObject_GetAttrString(pModule, pyFunctionName.c_str());
        if (!pFunc || !PyCallable_Check(pFunc)) {
            std::cerr << "Python function not found: " << pyFunctionName << std::endl;
            PyErr_Print();
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            finalizePython();  // Restart Python
            return {"Function not found", ""};
        }
    
        PyObject *pArgs = PyTuple_Pack(8, 
            PyBool_FromLong(retry),
            PyUnicode_FromString(url.c_str()),
            PyUnicode_FromString(method.c_str()),
            PyUnicode_FromString(headers.c_str()),
            PyUnicode_FromString(useragent.c_str()),
            PyUnicode_FromString(proxy.c_str()),
            PyLong_FromLong(timeout),
            PyUnicode_FromString(requestData.c_str())
        );
    
        PyObject *pResult = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
    
        std::pair<std::string, std::string> result = {"", ""};
    
        if (!pResult) {
            std::cerr << "Error calling Python function!" << std::endl;
            PyErr_Print();
            result.first = "Error during function call";
            if (pythonInitialized) {
                Py_Finalize();
                pythonInitialized = false;
            } // Restart Python if error occurs
        } else {
            PyObject *pStatus = PyDict_GetItemString(pResult, "status");
            PyObject *pData = PyDict_GetItemString(pResult, "data");
    
            if (pStatus && PyUnicode_Check(pStatus)) {
                result.first = PyUnicode_AsUTF8(pStatus);
            }
            if (pData && PyUnicode_Check(pData)) {
                result.second = PyUnicode_AsUTF8(pData);
            }
    
            Py_DECREF(pResult);
        }
    
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
    
        return result;

    }


    std::pair<std::string, std::string> siteScraper::externalScriptData(const std::string pythonScriptName, const std::string pyFunctionName, const bool retry, const std::string &url, const std::string &method,
        const std::string &headers,
        const std::string &useragent,
        const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber){
    







// Construct the command string with arguments
std::string command = ScriptName;

// Add arguments systematically
command += " -a " + url;
command += " -b " + method;
command += " -c " + headers;
command += " -d " + useragent;
command += " -e " + proxy;
command += " -f " + std::to_string(timeout);
command += " -g " + requestData;
command += " -h " + std::to_string(threadnumber);

// Optionally handle retries
if (retry) {
    command += " --retry";  // Add any specific retry flag or logic as needed
}

// Create a pipe to capture the output of the script
FILE* pipe = popen(command.c_str(), "r");
if (!pipe) {
    throw std::runtime_error("Error executing script: " + ScriptName);
}

// Read the output from the script
char buffer[128];
std::string result;
while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    result += buffer;
}

// Close the pipe
fclose(pipe);

// Now, parse the result as a JSON object
rapidjson::Document doc;
doc.Parse(result.c_str());

// Check if parsing was successful
if (doc.HasParseError()) {
    throw std::runtime_error("Error parsing JSON result: " + result);
}

// Ensure the expected JSON structure is present
if (!doc.IsObject() || !doc.HasMember("output") || !doc["output"].IsObject()) {
    throw std::runtime_error("Unexpected JSON format: " + result);
}

const rapidjson::Value& pair = doc["output"];

// Ensure the "first" and "second" members exist and are strings
if (!pair.HasMember("status") || !pair["status"].IsString()) {
    throw std::runtime_error("Missing or invalid 'first' in JSON result.");
}
if (!pair.HasMember("data") || !pair["data"].IsString()) {
    throw std::runtime_error("Missing or invalid 'second' in JSON result.");
}

// Extract the "first" and "second" values from the JSON object
std::string first = pair["status"].GetString();
std::string second = pair["data"].GetString();

// Return the pair
return { first, second };

        }    



bool siteScraper::vpnRebooter(std::string& vpn, std::string& command, std::string &expectedResult, int timeoutSeconds) {



    



    std::string fullCommand = command + " " + vpn;
    std::string result;
    
    auto future = std::async(std::launch::async, [&]() -> std::string {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(fullCommand.c_str(), "r"), pclose);

        if (!pipe) {
            std::cerr << "popen() failed!" << std::endl;
            return "";
        }

        // Read the output of the command in chunks of 128 characters
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    });

    if (future.wait_for(std::chrono::seconds(timeoutSeconds)) == std::future_status::timeout) {
        std::cerr << "Command timed out!" << std::endl;
        return false;
    }

    result = future.get();

    // Determine the expected result based on your criteria
   // std::string expectedResult = "expected result"; // Replace with the actual expected result

    // Check if the result contains the expected result
    return result.find(expectedResult) != std::string::npos;
}