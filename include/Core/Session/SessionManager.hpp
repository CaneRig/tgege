#pragma once
#include <map>
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include "./Session.hpp"
#include "../Exceptions/SessionNotExist.hpp"
#include "../App/AppRegistery.hpp"
#include "../User/TgUser.hpp"

namespace core
{
    class SessionManager
    {
        std::map<TgUser, rSession> _users;
        appid_t _defaultApp;

    public:
        SessionManager()
        {
            _defaultApp = "null";
        }

        // @init - set default app
        rSession AddSession(rSession session, bool init = true);

        auto GetSession(TgUser user);

        void RemoveSession(TgUser user);

        // resets session's app to default
        void ResetSession(TgUser user);

        // @return number of loaded sessions
        auto SessionCount() const { return _users.size(); }

        // closes expired sessions
        // @sessionLifetime - lifetime in seconds
        void Cleanup(long long sessoinLifetime);

        // @defaultApp(void) -> App
        // return app instance that will be opened when session get resets
        void SetDefaultApp(const appid_t &defaultApp) { this->_defaultApp = defaultApp; }
    };
} // namespace core

auto core::SessionManager::GetSession(TgUser user)
{
    auto p = this->_users.find(user);

    if (p == this->_users.end())
        throw excepts::SessionNotExist("core::SessionManager::GetSession");

    return p->second;
}
core::rSession core::SessionManager::AddSession(rSession session, bool init)
{
    TgUser user = session->UID();

    if (init)
        session->SetRApp(core::AppManager::GetRegistery().Instantiate(this->_defaultApp));

    std::cout << user << " is being to be added" << std::endl;

    return _users[user] = session;
}
void core::SessionManager::RemoveSession(TgUser user)
{
    this->_users.erase(user);
}
void core::SessionManager::ResetSession(TgUser user)
{
    auto session = this->GetSession(user);

    session->SetRApp(core::AppManager::GetRegistery().Instantiate(this->_defaultApp));
}
void core::SessionManager::Cleanup(long long sessoinLifetime)
{
    std::vector<std::pair<core::TgUser, core::rSession>> sessions(this->_users.begin(), this->_users.end());

    auto cmp = [](const auto &lhs, const auto &rhs)
    {
        return lhs.second->LastMessageTime() > rhs.second->LastMessageTime();
    };

    std::sort(sessions.begin(), sessions.end(), cmp);

    auto now = std::chrono::system_clock::now() - std::chrono::milliseconds(sessoinLifetime);

    auto bound_cmp = [](const auto &value, const auto &elem)
    {
        return value < elem.second->LastMessageTime();
    };

    auto p = std::upper_bound(sessions.begin(), sessions.end(), now, bound_cmp);

    this->_users.clear();
    this->_users.insert(p, sessions.end());

    for(auto it = sessions.begin(); it!=p;++it)
        it->second->Close();
}