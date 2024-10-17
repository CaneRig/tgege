#pragma once
#include <string>
#include "../Utility/ConfigReader.hpp"
#include <cstdlib>

namespace core
{
    // class that stores data from config file
    struct BotConfig
    {
        util::config_t raw_config;

        BotConfig(const util::config_t &conf);
        ~BotConfig() {}

        // path to data folder
        std::string data_path;

        // time in seconds between expired sessions cleanup
        float sessions_cleanup_interval;

        // time in millis
        long long session_lifetime;

        // @return found value associated to key
        const std::string &operator[](const std::string &key) const;
    };

} // namespace core

const std::string &core::BotConfig::operator[](const std::string &key) const
{
    return raw_config.at(key);
}
core::BotConfig::BotConfig(const util::config_t &conf)
{
    raw_config = conf;

    data_path = util::get_conf_val(conf, "data_path");
    sessions_cleanup_interval = std::stof(util::get_conf_val(conf, "sessions_cleanup_interval"));
    session_lifetime = std::stoll(util::get_conf_val(conf, "session_lifetime"));
}