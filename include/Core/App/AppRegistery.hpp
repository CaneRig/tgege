#pragma once
#include "AApp.hpp"
#include <map>
#include <functional>
#include <string>
#include <memory>
#include "../../Utility/StringOperations.hpp"

namespace core
{
    namespace app
    {
        // app instancer base
        class BAppInstantiator
        {
        protected:
            const appid_t _appId;

        public:
            BAppInstantiator(const appid_t appId) : _appId(appId) {}
            virtual ~BAppInstantiator() {}

            auto AppId() const { return _appId; }
            virtual App Instance(const std::string &data = "") const = 0;
        };

        // instances app with Instance_t type with core::CreateApp<Instance_t>()
        template <AppInstance Instance_t>
        class TAppInstantiator : public BAppInstantiator
        {
        public:
            TAppInstantiator() : BAppInstantiator(Instance_t::StaticAppId()) {}
            TAppInstantiator(const appid_t &id) : BAppInstantiator(id) {}

            virtual App Instance(const std::string &_data = "") const
            {
                auto app = core::CreateApp<Instance_t>();
                app->SetData(_data);
                return app;
            }
        };
        // instances app by calling delegate passed to contructor
        class FAppInstantiator : public BAppInstantiator
        {
        public:
            using Delegate = std::function<App(void)>;

            FAppInstantiator(const appid_t &id, const Delegate fabric) : _fabric(fabric), BAppInstantiator(id) {}
            virtual ~FAppInstantiator() {}

            virtual App Instance(const std::string &_data = "") const
            {
                auto app = _fabric();
                app->SetData(_data);
                return app;
            }

        private:
            const Delegate _fabric;
        };

        // registery list
        // creates app with same app id
        // usage:
        //  - Register app: Register<Instance_t>()
        //                  Stores registery with given Instance_t
        //  - Instantiate(app_id) -> App
        //                  Loads registered AppInstantiator with the given app_id
        //                  @return instantiated app
        class AppRegistery
        {
        public:
            AppRegistery() {}

            // register app by type
            template <AppInstance Instance_t>
            void Register(const appid_t &id = "", bool replace = false);

            // register app by fabric function
            void Register(const appid_t &id, const FAppInstantiator::Delegate fabric, bool replace = false);

            // instance app by id
            App Instantiate(const appid_t &id, const std::string &data = "") const;

        private:
            bool _exist(const appid_t &id) const { return _instantiators.find(id) != _instantiators.end(); }
            void _setRegistery(const appid_t &id, std::shared_ptr<BAppInstantiator> p) { _instantiators[id] = p; }

            std::map<appid_t, std::shared_ptr<BAppInstantiator>> _instantiators;
        };
    } // namespace app

    using app::AppRegistery;

    namespace AppManager
    {
        static AppRegistery _registery;

        static void SetRegistery(AppRegistery &reg) { _registery = reg; }
        static auto &GetRegistery() { return _registery; }
    };

} // namespace core

// register app by type
template <core::AppInstance Instance_t>
void core::app::AppRegistery::Register(const core::appid_t &id, bool replace)
{
    using namespace core::app;
    using namespace core;

    if (id.empty())
    {
        if (!this->_exist(core::GetAppId<Instance_t>()) || replace)
            this->_setRegistery(GetAppId<Instance_t>(), std::shared_ptr<BAppInstantiator>(new TAppInstantiator<Instance_t>()));
    }
    else
    {
        if (!this->_exist(id) || replace)
            this->_setRegistery(id, std::shared_ptr<BAppInstantiator>(new TAppInstantiator<Instance_t>(id)));
    }
}
void core::app::AppRegistery::Register(const core::appid_t &id, const core::app::FAppInstantiator::Delegate fabric, bool replace)
{
    if (!this->_exist(id) || replace)
        this->_setRegistery(id, std::shared_ptr<BAppInstantiator>(new FAppInstantiator(id, fabric)));
}
// instance app
core::App core::app::AppRegistery::Instantiate(const core::appid_t &id, const std::string &data) const
{
    if (!this->_exist(id))
        throw std::invalid_argument(util::concatenate_params("cannot Instantiate app: no id registered: \"", id, "\""));
    return this->_instantiators.at(id)->Instance(data);
}