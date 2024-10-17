#pragma once
#include <exception>
#include <string>

namespace core
{
    namespace excepts
    {
        struct UninatializedStaticObject : public std::exception
        {

            UninatializedStaticObject(const std::string &object_name = "[no description provided]") : _obj_name(std::string("Using an uninialized object: ") + object_name) {}

            const char *what() const noexcept override { return _obj_name.c_str(); }

        private:
            std::string _obj_name;
        };
    } // namespace exceptions

} // namespace core
