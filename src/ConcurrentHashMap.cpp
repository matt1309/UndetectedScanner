
#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <functional>
#include <sstream>
#include <iomanip>
// #include "Logger.h"

template <typename Key, typename Value>
class ConcurrentHashMap
{

public:
    struct Element
    {
        Value value;
        std::mutex mtx;
       
       Element() : value(Value()) {}
        Element(const Value &val) : value(val) {}

        // Delete copy constructor and copy assignment operator
        Element(const Element &) = delete;
        Element &operator=(const Element &) = delete;

        // Allow move constructor and move assignment operator
        Element(Element &&) = default;
        Element &operator=(Element &&) = default;


    };

private:
    std::unordered_map<Key, std::mutex> elementMutexes;
    std::shared_mutex globalMutex;
    std::unordered_map<Key, Element> elementMap;

public:
    std::atomic<int> mapSize{0};
    // Get the mutex associated with a key
    std::mutex &getMutexForKey(const Key &key)
    {
        return elementMap[key].mtx;
    }

    // Constructor //change this to 0 and get it added over time.
    explicit ConcurrentHashMap(size_t num_buckets = 0) {}

    // Insert or update the value associated with the given key
    void put(const Key &key, const Value &value)
    {

        if (contains(key))
        {
            std::lock_guard<std::mutex> lock(getMutexForKey(key));
            elementMap[key].value = value;
        }
        else
        {

            // code to add element needs unique lock. and to adjust mapSize element.
            // there are no protections in here for if the event someone forgets to make a mutex for each element.
            std::unique_lock<std::shared_mutex> uLock(globalMutex);
            //    Element elem{value};
            //  elementMap.emplace(key,ConcurrentHashMap::Element{value});
            // elementMap.emplace(std::make_pair(key, Element{value}));

            // elementMap.emplace(key, Element(value));
             
             elementMap.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(key),
                                 std::forward_as_tuple(value));

            //Element newEntry(value);

            // elementMap.insert_or_assign(key, Element{value});
           // elementMap[key] = newEntry;

            mapSize.fetch_add(1);
        }
    }

    // Get the value associated with the given key
    // Returns a default-constructed value if key not found
    Value get(const Key &key)
    {

        if (contains(key))
        {
            //wonder if this lock is even needed/should/could it be shared if it is. 
            std::lock_guard<std::mutex> lock(getMutexForKey(key));
            auto it = elementMap.find(key);
            return it->second.value;
        }
        else
        {
            throw std::out_of_range("Key not found");
        }
    }

    // not thread safe so to remove as it would need locking outside of this object.
    Value &getRef(const Key &key)
    {

        if (contains(key))
        {
            auto it = elementMap.find(key);
        }
        else
        {
            throw std::out_of_range("Key not found");
        }
    }

    // Check if the map contains the given key
    bool contains(const Key &key)
    {
        std::shared_lock<std::shared_mutex> slock(globalMutex);
        // std::lock_guard<std::mutex> lock(getMutexForKey(key));
        return elementMap.find(key) != elementMap.end();
    }

    // Remove the entry with the given key
    void remove(const Key &key)
    {

        if (contains(key))
        {
            std::unique_lock<std::shared_mutex> uLock(globalMutex);
            elementMap.erase(key);
            elementMutexes.erase(key);
            mapSize.fetch_add(-1);
        }
    }

    // Return the size of the map
    size_t size()
    {
        size_t sizeValue = static_cast<size_t>(mapSize.load());
        return sizeValue;
    }

    // this isn't thread safe but think it's only called on shutdown of Logger which will only be called once... I think.. not the best but not sure of an ideal solution
    //  Iterator support
    auto begin()
    {
        std::shared_lock<std::shared_mutex> slock(globalMutex);
        return elementMap.begin();
    }

    auto end()
    {
        std::shared_lock<std::shared_mutex> slock(globalMutex);
        return elementMap.end();
    }

    // the below functions are specific to my use case for logging I think the above however is just a concurrent hashmap optimised for existing element edits.

    void edit(int &key, std::string &value, int maxLength, bool web, std::string type)
    {

        if (contains(key))
        {
            std::lock_guard<std::mutex> lock(getMutexForKey(key));
            if (elementMap[key].value.length() > maxLength)
            {

                logToFile(type, key, value);

                if (web)
                {
                    elementMap[key].value = "To view older log content please view log files <br>" + value;
                }
                else
                {
                    elementMap[key].value = "To view older log content please view log files\n" + value;
                }
            }
            else
            {

                if (web)
                {
                    elementMap[key].value.append("<br>" + value);
                }
                else
                {
                    elementMap[key].value.append("\n" + value);
                }
            }
        }
        else
        {

            throw std::out_of_range("Key not found");
        }
    }

    void edit(std::string &key, std::string &value, int maxLength, bool web, std::string type)
    {

        if (contains(key))
        {
            std::lock_guard<std::mutex> lock(getMutexForKey(key));
            if (elementMap[key].value.length() > maxLength)
            {

                logToFile(type, key, value);

                if (web)
                {
                    elementMap[key].value = "To view older log content please view log files <br>" + value;
                }
                else
                {
                    elementMap[key].value = "To view older log content please view log files\n" + value;
                }
            }
            else
            {

                if (web)
                {
                    elementMap[key].value.append("<br>" + value);
                }
                else
                {
                    elementMap[key].value.append("\n" + value);
                }
            }
        }
        else
        {

            throw std::out_of_range("Key not found");
        }
    }

    void logToFile(std::string &type, int &key, std::string &contents)
    {
        std::string formattedDateTime = getCurrentTime();
        std::string filePath = "logs/";

        if (type == "manual")
        {
            filePath += "manual/manualLog-" + std::to_string(key) + "-" + formattedDateTime + ".txt";
            writeToFile(contents, filePath);
        }
        else if (type == "auto")
        {
            filePath += "auto/autoLog-" + std::to_string(key) + "-" + formattedDateTime + ".txt";
            writeToFile(contents, filePath);
        }
        else if (type == "general")
        {
            filePath += "generalLog-" + formattedDateTime + ".txt";
            writeToFile(contents, filePath);
        }

        std::cout << "Finished writing logs to file: " << type << " " << key << std::endl;
    }

    void logToFile(std::string &type, std::string &key, std::string &contents)
    {
        std::string formattedDateTime = getCurrentTime();
        std::string filePath = "logs/";

        if (type == "manual")
        {
            filePath += "manual/manualLog-" + key + "-" + formattedDateTime + ".txt";
            writeToFile(contents, filePath);
        }
        else if (type == "auto")
        {
            filePath += "auto/autoLog-" + key + "-" + formattedDateTime + ".txt";
            writeToFile(contents, filePath);
        }
        else if (type == "general")
        {
            filePath += "generalLog-" + formattedDateTime + ".txt";
            writeToFile(contents, filePath);
        }

        std::cout << "Finished writing logs to file: " << type << " " << key << std::endl;
    }

    void writeToFile(const std::string &content, const std::string &filePath)
    {
        std::ofstream outputFile(filePath);
        if (outputFile.is_open())
        {
            outputFile << content;
            outputFile.close();
            std::cout << "Writing String to File: " << filePath << std::endl;
        }
        else
        {
            std::cerr << "Error opening file: " << filePath << std::endl;
        }
    }

    std::string getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm timeinfo;
        localtime_s(&timeinfo, &in_time_t); // Use localtime_s instead of localtime

        std::stringstream ss;
        ss << std::put_time(&timeinfo, "%d-%m-%Y %H.%M.%S"); // Use std::put_time with tm object
        return ss.str();
    }
};
