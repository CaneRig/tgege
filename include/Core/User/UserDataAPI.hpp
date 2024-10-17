#pragma once
#include <string>
#include <iostream>
#include "../Session/SessionAppData.hpp"

namespace core
{
    class UserDataAPI
    {
        SessionAppData _session;

    public:
        UserDataAPI() {}
        UserDataAPI(SessionAppData data) { _setUDSession(data); }
        virtual ~UserDataAPI() {}

    protected:
        // saves @data by associated @appId to user's db
        void SaveApp(const std::string &appId, const std::string &data)
        {
            std::cout << "Saving app " << appId << "\t with data: " << data << std::endl;
        }
        // @return appdata associated with @appId from user's db
        std::string LoadApp(const std::string appId)
        {
            std::cout << "Loading app " << appId << std::endl;

            return "";
        }

        // set session data
        void _setUDSession(SessionAppData session)
        {
            this->_session = session;
        }
    };
} // namespace core
