#pragma once

#include "../Core/App/AApp.hpp"
#include "RusAccent.hpp"

class EchoApp : public core::AApp
{
public:
    EchoApp() : AApp(AApp::NullArg{}) {}
    EchoApp(core::SessionAppData data) : AApp(AApp::NullArg{}) { this->Load(data); }
    virtual ~EchoApp() {}

    virtual void OnMessage(const std::string &message);
    virtual core::appid_t AppId() { return StaticAppId(); }

    static core::appid_t StaticAppId() { return "app.test.echoapp"; }

protected:
    virtual void
    OnLoad()
    {
    }
    void OnClose() override {}
};

void EchoApp::OnMessage(const std::string &message)
{
    std :: cout << "ECHO SAID: ";
    std :: cout << message << std :: endl ;
    this->SendMessage(std::string("You said: ") + message);
}