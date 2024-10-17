#pragma once
#include <tgbot/tgbot.h>
#include "../User/TgUser.hpp"

namespace core
{
    struct SessionAppData
    {
        SessionAppData(){}

        std::shared_ptr<TgBot::Bot> bot;
        TgUser tguser;
        std::shared_ptr<BotConfig> config;
    };

} // namespace core
