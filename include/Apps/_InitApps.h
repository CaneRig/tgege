#pragma once
#include "EchoApp.hpp"
#include "RusAccent.hpp"
#include "StartApp.hpp"
#include "../Core/App/AppRegistery.hpp"

#define DEFAULT_APP_ID "app.rus.select"

static void INIT_APPS(){
    auto& r = core::AppManager::GetRegistery();

    r.Register<StartApp>(DEFAULT_APP_ID);
    r.Register<StartApp>();
    r.Register<StartApp>("null");

    r.Register<rus::RusAccent>();
    r.Register<EchoApp>();
}
