#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace util
{
    inline std::string to_string(const char *v)
    {
        return std::string(v);
    }
    inline std::string to_string(char c)
    {
        char chr[2] = {0, 0};
        chr[0] = c;

        return std::string(chr);
    }
    inline std::string to_string(const std::string &s)
    {
        return s;
    }
    inline std::string to_string(std::chrono::system_clock::time_point tp)
    {
        auto tt = tp.time_since_epoch();
        const time_t durS = std::chrono::duration_cast<std::chrono::seconds>(tt).count();
        std::ostringstream ss;
        if (const std::tm *tm = std::gmtime(&durS))
        {
            ss << std::put_time(tm, "%Z %Y-%m-%d %H:%M:%S.");

            const long long durMs = std::chrono::duration_cast<std::chrono::milliseconds>(tt).count();
            ss << std::setw(3) << std::setfill('0') << int(durMs - durS * 1000);
        }
        // gmtime/localtime() returned null
        else
        {
            ss << "%convertion error%";
        }
        return ss.str();
    }
} // namespace util
