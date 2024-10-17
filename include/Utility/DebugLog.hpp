// Header for console debugging
#pragma once


#include <iostream>
#include <string>

// Prints current function, line and file
#ifndef NDEBUG
#define DEB_POS()                                                                                          \
    {                                                                                                      \
        std::cout << "DEBUG:: at " << __FUNCTION__ << " in " << __FILE__ << ": " << __LINE__ << std::endl; \
    }
#else
#define DEB_POS()
#endif

namespace util
{
    // Prints to console line break and flushes cout's buffer
    static void DebugLog()
    {
        std::cout << std::endl;
    }
    // Prints to console 'arg' then line break + flush 
    template <class T>
    static void DebugLog(T arg)
    {
        std::cout << arg << std::endl;
    }
    // Prints to console 'arg' then recursively other arguments at the end puts line break + flush 
    template <class T, class... Args>
    static void DebugLog(T arg, Args... args)
    {
        std::cout << arg << ' ';
        DebugLog(args...);
    }

    // Prints to error console line break and flushes cout's buffer
    static void ErrorLog()
    {
        std::cerr << std::endl;
    }
    // Prints to error console 'arg' then line break + flush 
    template <class T>
    static void ErrorLog(T arg)
    {
        std::cerr << arg << std::endl;
    }
    // Prints to error console 'arg' then recursively other arguments at the end puts line break + flush 
    template <class T, class... Args>
    static void ErrorLog(T arg, Args... args)
    {
        std::cerr << arg << ' ';
        ErrorLog(args...);
    }
} // namespace util
