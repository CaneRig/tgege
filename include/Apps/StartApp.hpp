#pragma once
#include "Core/Bot.hpp"
#include "Core/App/AApp.hpp"
#include "RusAccent.hpp"
#include "Utility/StringOperations.hpp"
#include "Utility/CommandParser.hpp"
#include "AppAliases.h"
#include <iostream>

class StartApp : public core::AApp
{
private:
    util::CommandParser _commands;
public:
    StartApp() : AApp(AApp::NullArg{}) {}
    StartApp(core::SessionAppData data) : AApp(AApp::NullArg{}) { this->Load(data); }
    virtual ~StartApp() {}

    virtual void OnMessage(const std::string &message);
    virtual core::appid_t AppId() { return StaticAppId(); }

    static core::appid_t StaticAppId() { return "app.startapp"; }

protected:
    virtual void OnLoad();
    virtual void OnClose() {
        std::cout << "Start app closed" << std::endl;
    }
};

void StartApp::OnMessage(const std::string &message)
{
    if(message[0] == '!') {
        this->_commands.Run(message);
    } else {
        this->SendMessage("Неизвестное приложение, используйте \"!помощь\"");
    }
}
void StartApp::OnLoad(){
    using namespace std;
    
    this->_commands.Bind("помощь", [this](const std::string& s){
        std::string message = "Команды: \n"
                            "\t!запуск имя  \t- запуск приложения о имени\n"
                            "\t!список      \t- список доступных приложений\n"
                            "\t!помощь      \t- вывод помощи\n";

        this->SendMessage(message);
    });
    this->_commands.Bind("список", [this](const std::string& s){
        std::string message = "Приложения:\n";

        for(auto &[rus, id]: AppAliases){
            message += "\"";
            message += rus;
            message += "\" \t ";
        }

        this->SendMessage(message);
    });
    this->_commands.Bind("запуск", [this](const std::string& s){
        std::cout << "called: \"" << s << "\"" << std::endl;
        auto strip = util::strip(s);


        if(AppAliases.contains(strip)){
            this->SendMessage("Запуск приложения...");
            this->SetContext(AApp::AppSwitchContext(AppAliases.get(strip)));
        }else{
            this->SendMessage("Неизвестное приложение");
        }
    });

    this->_commands.SetDefaultDelegate([this](const std::string& s){
        this->SendMessage("Неизвестная команда, для вывода помощи напиши !помощь");
    });
    
    this->SendMessage("Здравствуй, выбери с помощью команды !запуск [_имя_] приложение, которое хочешь запустить.");
}