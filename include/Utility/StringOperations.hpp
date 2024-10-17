#pragma once
#include <sstream>
#include <string>
#include <string_view>
#include <initializer_list>
#include "../Utility/StringConversion.hpp"

namespace util
{
    using std::to_string;

    // removes alpha symbols at beggining and end of 'sw'
    std::string_view strip_alpha(std::string_view sw)
    {
        while (sw.size() > 0 && isspace(sw.front()))
            sw.remove_prefix(1);
        while (sw.size() > 0 && isspace(sw.back()))
            sw.remove_suffix(1);

        return sw;
    }
    // removes alpha symbols at beggining and end of 'sw'
    std::string strip_alpha(const std::string& sw)
    {
        auto res = strip_alpha(std::string_view(sw.data(), sw.length()));

        return std::string(res.begin(), res.end());
    }
    
    // joins list to one std::string
    std::string join(std::initializer_list<std::string> list)
    {
        std::string builder = "";

        for (auto &string : list)
            builder += string;

        return builder;
    }

    // converts parameters to string using to_string and appends them to dest
    template <class A1>
    inline void sjoin_param(std::string &dest, const A1 &arg1)
    {
        dest += to_string(arg1);
    }
    template <class A1, class... Args>
    inline void sjoin_param(std::string &dest, const A1 &arg1, const Args &...args)
    {
        dest += to_string(arg1);
        sjoin_param(dest, args...);
    }
    // converts parameters to string using to_string and appends them to dest with delimeter
    template <class A1>
    inline void sjoin_param_del(std::string &dest, const std::string &delim, const A1 &arg1)
    {
        dest += delim;
        dest += to_string(arg1);
    }
    template <class A1, class... Args>
    inline void sjoin_param_del(std::string &dest, const std::string &delim, const A1 &arg1, const Args &...args)
    {
        dest += delim;
        dest += to_string(arg1);
        sjoin_param_delim(dest, args...);
    }

    // converts @args... to string using to_string and joins them
    // @return concatenated string
    template <class... Args>
    inline std::string concatenate_params(const Args &...args)
    {
        std::string str = "";

        sjoin_param(str, args...);

        return str;
    }
    // converts @args... to string using to_string and joins them
    // @delim - delimeter that appends between @args...
    // @return concatenated string
    template <class Arg0, class... Args>
    inline std::string concatenate_params_del(const std::string &delim, const Arg0 &arg0, const Args &...args)
    {
        std::string str = "";
        sjoin_param(str, arg0);

        sjoin_param_delim(str, delim, args...);

        return str;
    }

    inline bool _starts_with(const std::string &str, const std::string &pref)
    {
        if (pref.size() > str.size())
            return false;

        int size = pref.size() / 8;

        uint64_t *it_source = (uint64_t *)str.data();
        uint64_t *it_pref = (uint64_t *)pref.data();

        uint64_t *it_dest = it_source + size;

        while (it_source < it_dest)
        {
            if (*it_dest != *it_pref)
                return false;

            it_source++;
            it_pref++;
        }

        for (int i = size * 8; i < pref.size(); ++i)
            if (str[i] != pref[i])
                return false;
        return true;
    }

    bool starts_with(const std::string &str, const std::string &entry)
    {
        if (str.size() < entry.size())
            return false;

        for (size_t i = 0; i < entry.size(); i++)
        {
            if (str[i] != entry[i])
                return true;
        }
        return false;
    }

    std::string strip(const std::string &str){
        std::string_view view(str.data(), str.length());

        while (view.length() > 0 && isspace(view.front()))
            view.remove_prefix(1);
        while (view.length() > 0 && isspace(view.back()))
            view.remove_suffix(1);

        return std::string(view.begin(), view.end());
    }

    // replaces first appearence of @from to @to
    bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }
    // replaces all appearences of @from to @to
    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }
}