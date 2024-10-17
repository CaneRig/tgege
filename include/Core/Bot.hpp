#pragma once
#include <tgbot/tgbot.h>
#include <iostream>
#include <string>
#include <functional>
#include "./BotConfig.hpp"
#include "./Session/Session.hpp"
#include "./Session/CreateSession.hpp"
#include "./Session/SessionManager.hpp"
#include "./App/AApp.hpp"
#include "./Exceptions/UninitializedStaticObject.hpp"
#include "./Exceptions/SessionNotExist.hpp"
#include "../Utility/ConfigReader.hpp"
#include "../Utility/DebugLog.hpp" 

namespace core
{
    struct Bot
    {
    private:
        SessionManager _sessionManager;

        std::shared_ptr<TgBot::Bot> _botPtr;

    public:
        std::shared_ptr<BotConfig> config;

        Bot(const std::string &s, const util::config_t &conf);

        // pools requests
        void Poll();

        // returns reference to TgBot
        TgBot::Bot &operator->() { return *_botPtr; }
        TgBot::Bot &bot() { return *_botPtr; }

        // defaultApp -> App(void)
        // set app that loads when session creates or resets
        void SetDefaultApp(appid_t defaultApp) { this->_sessionManager.SetDefaultApp(defaultApp); }

        // returns session of specific 'user' (if there is no creates)
        core::rSession GetSession(TgUser user);

        // set 'session' app to default
        void ResetSession(TgUser user);
    };
    static std::shared_ptr<Bot> ___static_bot_instance = nullptr;

    // bot reference
    using rBot = std::shared_ptr<Bot>;

    // create singleton bot
    static void CreateBot(const std::string &token, const util::config_t &conf)
    {
        ___static_bot_instance = std::make_shared<Bot>(token, conf);
    }
    // loads singleton bot
    static auto GetBot()
    {
        if (___static_bot_instance.get() == nullptr)
            throw core::excepts::UninatializedStaticObject("_static_bot_instance");
        return ___static_bot_instance;
    }
} // namespace core

void core::Bot::Poll()
{
    try
    {
        std::cout << "Bot username: " << bot().getApi().getMe()->username.c_str() << "\n";

        TgBot::TgLongPoll longPoll(this->bot());

        auto lastCleanupTime = std::chrono::system_clock::now();

        while (true)
        {
            auto currentTime = std::chrono::system_clock::now();

            if (std::chrono::duration<float>(currentTime - lastCleanupTime).count() >= this->config->sessions_cleanup_interval)
            {
                util::DebugLog("Session cleanup started at ", util::to_string(currentTime));

                auto s = this->_sessionManager.SessionCount();

                this->_sessionManager.Cleanup(this->config->session_lifetime);

                lastCleanupTime = std::chrono::system_clock::now();

                util::DebugLog("Session cleanup ended, elapsed ", std::chrono::duration<float>(lastCleanupTime - currentTime).count(),
                               ", deleted ", s - this->_sessionManager.SessionCount(), ", now ", this->_sessionManager.SessionCount());
            }

            longPoll.start();
        }
    }
    catch (TgBot::TgException &e)
    {
        std::cerr << "Bot error: " << e.what();
    }
}

core::Bot::Bot(const std::string &s, const util::config_t &conf) : _botPtr(std::make_shared<TgBot::Bot>(s))
{
    this->config = (std::shared_ptr<BotConfig>(new BotConfig(conf)));

    this->bot().getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
                                         {
                                        auto session = this->GetSession({message->chat->id});

                                        session->OnMessage(message->text); });
}
core::rSession core::Bot::GetSession(TgUser user)
{
    core::rSession session;

    try
    {
        session = this->_sessionManager.GetSession(user);
    }
    catch (excepts::SessionNotExist &err)
    {
        util::DebugLog("Session cannot be found, creating one");
        session = core::CreateSession(this->config, user, this->_botPtr);

        this->_sessionManager.AddSession(session);
    }

    return session;
}
void core::Bot::ResetSession(TgUser user)
{
    this->_sessionManager.ResetSession(user);
}