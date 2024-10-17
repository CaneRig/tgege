#include <tgbot/tgbot.h>
#include <string>
#include <vector>
#include <algorithm>

namespace core
{
    namespace extentions
    {
        class TgBotMenuButtons
        {
            // {name, description}
            std::vector<std::pair<std::string, std::string>> _buttons;

            void _push(const std::pair<std::string, std::string> &value);

        public:
            TgBotMenuButtons() {}

            // push buttons {name, description}
            TgBotMenuButtons &operator<<(const std::pair<std::string, std::string> &value);

            // set buttons vector {name, description}
            // repeated buttons are destroyed
            void SetButtons(const std::vector<std::pair<std::string, std::string>> &buttons);
            [[nodiscard]] std::vector<std::pair<std::string, std::string>> GetButtons() const;

            void Clear() { this->_buttons.clear(); }

            [[nodiscard]] std::string GetJson() const;
        };
    } // namespace extentions

} // namespace core

core::extentions::TgBotMenuButtons &core::extentions::TgBotMenuButtons::operator<<(const std::pair<std::string, std::string> &value)
{
    this->_push(value);

    return *this;
}
void core::extentions::TgBotMenuButtons::SetButtons(const std::vector<std::pair<std::string, std::string>> &buttons)
{
    this->_buttons.resize(buttons.size());

    std::unique_copy(buttons.begin(), buttons.end(), this->_buttons.begin(), [](const auto &a, const auto &b)
                     { return a.first == b.first; });
}
std::vector<std::pair<std::string, std::string>> core::extentions::TgBotMenuButtons::GetButtons() const
{
    return this->_buttons;
}
std::string GetJson()
{
    std::string json = "";

    return json;
}

void core::extentions::TgBotMenuButtons::_push(const std::pair<std::string, std::string> &value)
{
    if (auto x = std::find_if(this->_buttons.begin(), this->_buttons.end(), [&](const auto &v)
                              { return v.first == value.first; });
        x != this->_buttons.end())
        x->second = value.second;
    else
        this->_buttons.push_back(value);
}