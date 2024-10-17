#pragma once
#include <tgbot/tgbot.h>
#include <iostream>
#include <string>
#include <memory>
#include "./BotAPI.hpp"
#include "../User/UserDataAPI.hpp"
#include "../../Utility/StringOperations.hpp"
#include "../../Utility/DebugLog.hpp"
#include <variant>
#include <concepts>

namespace core
{
    using appid_t = std::string;

    // abstract class of app
    // when inheriting, the implementation of the static method “StaticAppId” is required
    // sample: static appid_t StaticAppId() { return "id.app"; }
    class AApp : public BotAPI, public UserDataAPI
    {
        bool _loaded = false;
        bool _closed = false;

    public:
        // need to switch App in session after OnMessage called
        struct AppSwitchContext
        {
            AppSwitchContext() : _ready(false), _appid(""), _data("") {}
            // Create Switch Context object
            AppSwitchContext(const appid_t &id, const std::string &data = "") : _appid(id), _data(data)
            {
                _ready = true;
            }

            [[nodiscard]] appid_t GetAppId() const { return _appid; }
            [[nodiscard]] const std::string &Data() const { return _data; }
            // @return true if app hasBeen created;
            bool Ready() const { return _ready; }

            // @return if true switching failed
            bool Failed() const { return _fail; }

            // when called sets 'failed' flag to true
            void fail() { _fail = true; }

        private:
            appid_t _appid;
            std::string _data;
            bool _ready = false;
            bool _fail = false;
        };

    protected:
        struct NullArg
        {
        };
        AApp(NullArg) { }
    public:

        AApp() = delete;
        AApp(const AApp&) = delete;
        AApp(AApp&&) = delete;

        // free resources
        void Close();

        virtual ~AApp(){}

        // calls when message is recieved
        void ReceiveMessage(const std::string &message);

        // virtual method that handles message responses
        virtual void OnMessage(const std::string &message) = 0;

        // @return - uniq app id, ex "me.cars.fastcar"
        virtual appid_t AppId() = 0;

        // bind app with session by @data
        void Load(SessionAppData data);

        // @return current context (may be deactive)
        AppSwitchContext &GetSwitchContext();

        // erases switch context
        void ResetSwitchContext();

        // use this method to transfer data when switching context
        // WARNING: THIS METHOD CAN BE CALLED BEFORE SESSION BINDING
        virtual void SetData(const std::string &data) {}

    protected:
        // calls when app closes
        virtual void OnClose(){};
        // calls when app loads
        virtual void OnLoad(){};

        // trigger to switch App in session after OnMessage called from Session
        // example this->SetContext(AApp::AppSwitchContext("app.myapp"));
        void SetContext(AppSwitchContext);
        
        // same as SetContext(AppSwitchContext), but constructs AppSwitchContext in place
        void SetContext(const appid_t& id, const std::string& data="");

    private:
        AppSwitchContext _context;
    };

    // empty app that does nothing
    class EmptyApp : public AApp
    {
    public:
        EmptyApp(SessionAppData data) : AApp(AApp::NullArg{}) { this->Load(data); }

        virtual void OnMessage(const std::string &message) { util::DebugLog("Empty app on message \n", message); }
        virtual appid_t AppId() { return StaticAppId(); }

        static appid_t StaticAppId() { return "app.null.emptyapp"; }

    protected:
        virtual void OnClose() {}
        virtual void OnLoad() {}
    };

    // App reference
    using App = std::shared_ptr<AApp>;

    // App concept
    template <class Tp>
    concept AppInstance = requires(Tp &a) {
        (AApp &)a;
        Tp::StaticAppId();
    } && std::same_as<decltype(Tp::StaticAppId()), appid_t>;

    // @return AppId of T
    template <AppInstance T>
    constexpr inline appid_t GetAppId() { return T::StaticAppId(); }
    // @return AppId of instance via calling virtual AppId()
    template <AppInstance T>
    inline appid_t GetAppId(const T &instance) { return instance.AppId(); }
    // @return AppId of instance via calling virtual AppId()
    inline appid_t GetAppId(App app) { return app->AppId(); }

    // Creating app of type T
    // @args... arguments that passed to T constructor
    // @return created app
    template <AppInstance T, class... Args>
    App CreateApp(Args... args) { return std::make_shared<T>(args...); }
} // namespace core

core::AApp::AppSwitchContext &core::AApp::GetSwitchContext()
{
    return this->_context;
}
void core::AApp::ResetSwitchContext()
{
    this->_context = core::AApp::AppSwitchContext();
}
void core::AApp::SetContext(AppSwitchContext ctx)
{
    this->_context = ctx;
}
void core::AApp::SetContext(const appid_t& id, const std::string& data){
    this->SetContext(AppSwitchContext(id, data));
}


void core::AApp::Close()
{
    if (this->_closed)
        throw std::runtime_error(util::join({"App ", this->AppId(), " is already closed"}));

    try{
        //if(this->_loaded)
        //    this->OnClose();
    } catch(std::exception const& e){
        util::ErrorLog("core::AApp::Load exception catched: ", e.what());
    }
    this->_closed = true; // ?
}
void core::AApp::Load(core::SessionAppData data)
{
    if (this->_loaded)
        throw std::invalid_argument("loading loaded app");

    this->_setUDSession(data);
    this->_setBotSession(data);

    try{
        this->OnLoad();
    } catch(std::exception const& e){
        util::ErrorLog("core::AApp::Load exception catched: ", e.what());

        this->SetContext(AApp::AppSwitchContext("null"));
        
        this->SendMessage("_ERROR LOADING APP, RESETTING_");
    }
    this->_loaded = true; // ?
}
void core::AApp::ReceiveMessage(const std::string &message){
    if(this->_loaded){
        try{
            this->OnMessage(message);
        } catch(std::exception const& e){
            util::ErrorLog("core::AApp::ReceiveMessage exception catched: ", e.what());
        }
    }
}