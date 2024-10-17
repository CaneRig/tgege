#pragma once
#include <tgbot/tgbot.h>
#include <iostream>
#include <string>
#include <ios>

namespace core
{
    // bot user id (telegram chat id)
    struct TgUser
    {
        TgUser() : chatId(0) {}

        uint64_t chatId;

        TgUser(uint64_t id) : chatId(id) {}
        TgUser(int64_t id) : chatId(id) {}

        bool operator<(const TgUser &u) const
        {
            return chatId < u.chatId;
        }
        bool operator>(const TgUser &u) const { return chatId > u.chatId; }
        bool operator==(const TgUser &u) const { return chatId == u.chatId; }
        bool operator!=(const TgUser &u) const { return chatId != u.chatId; }
    };

    static std::ostream &operator<<(std::ostream &stream, const TgUser &user)
    {
        return stream << "User{" << user.chatId << "}";
    }
} // namespace core
