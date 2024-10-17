#pragma once
#include <map>
#include <string>

class _AppAliases{
    std::map<std::string, std::string> _aliases;
public:
    _AppAliases(){
        _aliases = std::map<std::string, std::string>{
          {"главная", "app.startapp"},
          {"ударения", "app.rus.accent"},
          {"TEST", "app.test.echoapp"}
        };
    }

    [[nodiscard]] auto begin() const {return _aliases.begin();}
    [[nodiscard]] auto end() const {return _aliases.end();}

    bool contains(const std::string& str) const{
        return _aliases.contains(str);
    }
    auto get(const std::string& str) const{
        return _aliases.at(str);
    }
} AppAliases;