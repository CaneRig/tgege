#pragma once
#include <exception>
#include <string>

namespace core
{
    namespace excepts
    {
        struct NotImplemented : public std::exception
        {
            NotImplemented(){}

            const char *what() const noexcept override { return "Not implemented"; }

        };
    } // namespace exceptions

} // namespace core
