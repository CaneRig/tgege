#pragma once
#include "./Session.hpp"
#include "./SessionAppData.hpp"
#include "../User/TgUser.hpp"
#include "../App/AApp.hpp"

namespace core
{
    // @return session reference
    rSession CreateSession(std::shared_ptr<BotConfig> conf, TgUser user, std::shared_ptr<TgBot::Bot> bot){
        SessionAppData sdata;

        sdata.bot = bot;
        sdata.tguser = user;
        sdata.config = conf;

        rSession session = std::make_shared<Session>(sdata);
        session->SetApp<EmptyApp>();

        return session;
    }
    // InitApp - startup app
    // @return session reference
    template<AppInstance InitApp>
    rSession CreateSession(std::shared_ptr<BotConfig> conf, TgUser user, std::shared_ptr<TgBot::Bot> bot){
        SessionAppData sdata;

        sdata.bot = bot;
        sdata.tguser = user;
        sdata.config = conf;

        rSession session = std::make_shared<Session>(sdata);
        session->SetApp<InitApp>();

        return session;
    }
} // namespace core
 