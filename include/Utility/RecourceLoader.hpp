#pragma once
#include <string>
#include <map>
#include <type_traits>
#include <concepts>
#include <memory>
#include "DebugLog.hpp"

namespace util{
    // abstract class for recource
    class ARecource{
        public:
        virtual ~ARecource(){}

        // calls when recource creates
        virtual void Load() = 0;
    };

    // T has to be derived from ARecource
    template<class T>
    concept RecourceConcept = std::is_base_of_v<ARecource, T>;

    // class that contains recources
    class RecourceManager{
        std::map<std::string, std::shared_ptr<ARecource>> _res;

        template<RecourceConcept R>
        std::string _get_hash() const{
            return typeid(R).name();
        }
        template<RecourceConcept R>
        const R& __get(){
            auto hash = _get_hash<R>();
            return *dynamic_cast<R*>(_res[hash].get());
        }
        public:

        // @R - type of recource
        // @return ref to recource instance
        // if not contains recource adds it
        template<RecourceConcept R>
        const R& Get(){
            auto hash = _get_hash<R>();

            if(_res.contains(hash))
                return __get<R>();
            
            Add<R>();

            return __get<R>();
        }

        // Adding recource with type @R
        // Calls R::Load()
        template<RecourceConcept R>
        void Add(){
            auto hash = _get_hash<R>();

            DebugLog("RecourceManager: adding recource: \"", hash, "\"");
            
            auto add = std::shared_ptr<ARecource>(new R());
            add->Load();

            _res.insert({hash, add});
        }

        // Removes recource @R
        template<RecourceConcept R>
        void Remove(){
            auto hash = _get_hash<R>();

            DebugLog("RecourceManager: removing recource: \"", hash, "\"");
            
            _res.erase(hash);
        }

        // Removes recource @R and adds it again
        // On adding calls R::Load()
        template<RecourceConcept R>
        void Reload(){
            Remove<R>();
            Add<R>();
        }
    };

    static RecourceManager Recources;
}