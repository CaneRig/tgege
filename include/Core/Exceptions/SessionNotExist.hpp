#pragma once
#include <exception>
#include <string>

namespace core
{
    namespace excepts
    {
        struct SessionNotExist : public std::exception
        {
            SessionNotExist(){}
            SessionNotExist(const std::string &object_name) : _obj_name(std::string("Getting an unexisting session ") + object_name) {}

            const char *what() const noexcept override { return _obj_name.c_str(); }

        private:
            std::string _obj_name;
        };
    } // namespace exceptions

} // namespace core
