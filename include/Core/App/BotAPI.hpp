#pragma once
#include <tgbot/tgbot.h>
#include <iostream>
#include <string>
#include "../Session/SessionAppData.hpp"
#include "../../Utility/StringOperations.hpp"

namespace core
{
    class BotAPI
    {
        SessionAppData _session;

    public:
        BotAPI() {}
        BotAPI(SessionAppData data) { _setBotSession(data); }
        virtual ~BotAPI() {}

        // @message - message to be sent
        // @usedMarkdown - using markdownv2 in message, see https://core.telegram.org/bots/api#formatting-options
        void SendMessage(const std::string &message, bool useMarkdown = false);

        // sets commands panel
        // @commands - {command, description}
        void SetCommands(const std::vector<std::pair<std::string, std::string>> &commands);

    protected:
        void _setBotSession(SessionAppData session) { this->_session = session; }
    };
} // namespace core

void core::BotAPI::SetCommands(const std::vector<std::pair<std::string, std::string>> &commands)
{
    std::vector<TgBot::BotCommand::Ptr> cmds(commands.size());

    for (size_t i = 0; i < cmds.size(); i++)
    {
        cmds[i] = TgBot::BotCommand::Ptr(new TgBot::BotCommand);

        cmds[i]->command = commands[i].first;
        cmds[i]->command = commands[i].second;
    }

    auto scope = new TgBot::BotCommandScopeChat;
    scope->chatId = this->_session.tguser.chatId;

    auto scopePtr = TgBot::BotCommandScope::Ptr(scope);

    this->_session.bot->getApi().setMyCommands(cmds, scopePtr);
}
void core::BotAPI::SendMessage(const std::string &message, bool useMarkdown)
{
    if (util::strip_alpha(message).size())
        this->_session.bot->getApi().sendMessage(this->_session.tguser.chatId, message, false, 0, nullptr, (useMarkdown? "MarkdownV2" : ""));
}