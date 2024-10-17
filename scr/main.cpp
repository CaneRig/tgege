#include "../include/Core/Bot.hpp"
#include <iostream>
#include <sstream>
#include "../include/Utility/ConfigReader.hpp"
#include "../include/Utility/DebugLog.hpp"
#include "../include/Core/Session/CreateSession.hpp"

#include "../include/Core/App/AppRegistery.hpp"

#include "../include/Apps/_InitApps.h"

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
            throw std::invalid_argument("required config file path");

        util::DebugLog("Reading config");
        auto config = util::ReadConfig(argv[1]);

        util::DebugLog("Config had been loaded, readed", config.size(), "lines");

        INIT_APPS();

        auto token = config["bot_token"];
        config.erase("bot_token"); // для безопасности

        util::DebugLog("Token dropped");

        core::CreateBot(token, config);

        auto bot = core::GetBot();

        bot->SetDefaultApp(DEFAULT_APP_ID);

        bot->bot()
            .getEvents()
            .onCommand("start", [&bot](TgBot::Message::Ptr message)
                       { 
                                        std::stringstream text;

                                        text << "Hello " << message->chat->firstName << '\n';

                                        (bot->bot()).getApi().sendMessage(message->chat->id, text.str()); });

        bot->bot()
            .getEvents()
            .onCommand("menu", [&bot](TgBot::Message::Ptr message)
                       { bot->ResetSession({message->chat->id}); });

        bot->Poll();
    }
    catch (std::exception &ex)
    {
        util::ErrorLog("Critical ERROR: \"", ex.what(), "\"");

        return -1;
    }
}

int main_(int argc, char *argv[])
{
    if (argc < 2)
        throw std::invalid_argument("required config file path");

    util::DebugLog("Reading config");
    auto config = util::ReadConfig(argv[1]);

    util::DebugLog("Config had been loaded, read", config.size(), "lines");

    auto token = config["bot_token"];
    config.erase("bot_token"); // для безопасности

    util::DebugLog("Token dropped");

    util::DebugLog(token);

    TgBot::Bot bot(token);

    // Thanks Pietro Falessi for code
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
    TgBot::InlineKeyboardButton::Ptr checkButton(new TgBot::InlineKeyboardButton);
    checkButton->text = "check";
    checkButton->callbackData = "check";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);

    bot.getEvents().onCommand("start", [&bot, &keyboard](TgBot::Message::Ptr message)
                              { bot.getApi().sendMessage(message->chat->id, "Hi!", false, 0, keyboard); });
    bot.getEvents().onCommand("check", [&bot, &keyboard](TgBot::Message::Ptr message)
                              {
        std::string response = "ok";
        bot.getApi().sendMessage(message->chat->id, response, false, 0, keyboard, "Markdown"); });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](TgBot::CallbackQuery::Ptr query)
                                    {
        if (StringTools::startsWith(query->data, "check")) {
            std::string response = "ok";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        } });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message)
                                 {
        auto m = bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        printf("User wrote %s\n", message->text.c_str()); });

    // bot.getApi().setChatMenuButton(message->chat->id, "[{command: \"start\", description: \"Hello world\"}]");

    try
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true)
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (std::exception &e)
    {
        printf("error: %s\n", e.what());
    }

    return 0;
}