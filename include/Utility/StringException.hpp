#pragma once
#include <string>
#include <exception>

namespace util
{
    // exception that contains std::string messsage
    struct string_exception : std::exception
    {
        std::string _message;

    private:
        string_exception(const std::string &msg) : _message(msg) {}
        virtual ~string_exception() {}

        virtual char const *what() { return _message.c_str(); }
    }
}