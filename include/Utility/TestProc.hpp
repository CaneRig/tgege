#pragma once 
#include <iostream>
#include <string>
#include <functional>
#include <exception>

namespace testutil{
    using TestFunc = std::function<void()>;


    // runs test
    // if no exception catched test passes
    // otherwise fails
    void TestException(const std::string& testName, TestFunc&& fn){
        try{
            fn();
        }catch(const std::exception& e){
            std::cout << "Test: \"" << testName << "\" \t - \t";
            std::cout << "[FAILED]\n\t\t catched exception: \"" << e.what() <<"\"" << std::endl; 
            return;
        }
        std::cout << "Test: \"" << testName << "\" \t - \t";
        std::cout << "[PASSED]" << std::endl;
    }
}