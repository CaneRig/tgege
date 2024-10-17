#pragma once
#include <tgbot/tgbot.h>
#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include "./SessionAppData.hpp"
#include "../User/TgUser.hpp"
#include "../App/AApp.hpp"
#include "../BotConfig.hpp"
#include "../Exceptions/NotImplemented.hpp"
#include "../App/AppRegistery.hpp"

namespace core
{
    class Session
    {
        App _currentApp;
        SessionAppData _data;

        std::chrono::system_clock::time_point _timeUsed;

    public:
        Session() = delete;
        Session(SessionAppData udata) : _data(udata)
        {
            SetApp<EmptyApp>();
        }
        Session(const Session &) = delete;
        ~Session() { std::cout << "Session deleted" << std::endl; }

        // opens app & closes current
        // @AppT - app
        // @args - AppT(args...)
        template <AppInstance AppT, class... Args>
        void SetApp(Args... args);

        // opens app @app & closes current
        void SetRApp(App app);

        // @return current opened app
        App GetApp() const { return _currentApp; }

        // calls when user had recieved a message
        void OnMessage(const std::string &message);

        // closes app
        void Close();

        // @return user id
        TgUser UID() const { return _data.tguser; }

        // @return time, when last OnMessage called
        auto LastMessageTime() const { return _timeUsed; }
    };

    using rSession = std::shared_ptr<Session>;
} // namespace core

void core::Session::Close()
{
    if (this->_currentApp.get())
        this->_currentApp->Close();

    this->_currentApp.reset();
    this->_data = core::SessionAppData{};
}
template <core::AppInstance AppT, class... Args>
void core::Session::SetApp(Args... args)
{
    auto curr = this->_currentApp;

    try{
        this->_currentApp = std::make_shared<AppT>(this->_data, args...);
        try{
         if (curr.get())
             curr->Close();
         } catch(std::exception& e){
             util::ErrorLog("SetAppError.close", e.what());
        }
    }catch(std::exception& e){
        util::ErrorLog("SetAppError", e.what());
    }
}
void core::Session::SetRApp(App app)
{
    auto curr = this->_currentApp;

    try{
        this->_currentApp = app;
        this->_currentApp->Load(this->_data);
        try{
            if (curr)
                curr->Close();
        } catch(std::exception& e){
            util::ErrorLog("SetAppError.close", e.what());
        }
    }catch(std::exception& e){
        util::ErrorLog("SetAppError", e.what());
    }
}
void core::Session::OnMessage(const std::string &message)
{
    this->_timeUsed = std::chrono::system_clock::now();
    this->_currentApp->ReceiveMessage(message);

    if (this->_currentApp->GetSwitchContext().Ready())
    {
        try
        {
            this->SetRApp(core::AppManager::GetRegistery().Instantiate(
                this->_currentApp->GetSwitchContext().GetAppId(),
                this->_currentApp->GetSwitchContext().Data()));
        }
        catch (std::exception &ex)
        {
            this->_currentApp->GetSwitchContext().fail();

            util::ErrorLog("Context switch failed: \n$start message$ \n", ex.what(), "\n$end message$\n\n");
        }
    }
}