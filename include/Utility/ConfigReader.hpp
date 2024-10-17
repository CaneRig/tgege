#pragma once
#include <map>
#include <string>
#include <regex>
#include <fstream>
#include <exception>
#include <sstream>
#include <string_view>
#include "StringOperations.hpp"
#include "DebugLog.hpp"

namespace util
{
    using config_t = std::map<std::string, std::string>;

    // This method reads pairs key-value from file and stores them in config_t
    //  config file structure: key=value \n...
    // @return map object of readed key-value pairs
    config_t ReadConfig(const std::string &path)
    {
        std::ifstream fin(path);

        if (!fin)
            throw std::invalid_argument(std::string("Config file cant be opened: ") + path);
        config_t config;

        for (std::string line; std::getline(fin, line);)
        {
            size_t eqpos = line.find('=');
            if (eqpos == std::string::npos)
                continue;

            std::string_view name = line;
            std::string_view value = line;

            name.remove_suffix(line.size() - eqpos);
            value.remove_prefix(eqpos + 1);

            name = strip_alpha(name);
            value = strip_alpha(value);
            
            config[(std::string)name] = (std::string)value;
        }

        return config;
    }

    // @return value in conf assiciated with key
    // throw if key is not found
    std::string &get_conf_val(config_t &conf, const std::string &key)
    {
        try
        {
            auto &val = conf.at(key);

            return val;
        }
        catch (std::exception &except)
        {
            ErrorLog("Error at get_conf_val", except.what(), "key", std::string("\"") + key + std::string("\""));

            throw except;
        }
    }
    const std::string &get_conf_val(const config_t &conf, const std::string &key)
    {
        try
        {
            const auto &val = conf.at(key);

            return val;
        }
        catch (std::exception &except)
        {
            ErrorLog("Error at get_conf_val", except.what(), "key", std::string("\"") + key + std::string("\""));

            throw except;
        }
    }
}