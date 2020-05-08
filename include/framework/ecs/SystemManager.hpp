#ifndef HESTIA_FRAMEWORK_ECS_SYSTEMMANAGER_H_
#define HESTIA_FRAMEWORK_ECS_SYSTEMMANAGER_H_

#include <map>
#include <memory>
#include <functional>
#include <typeinfo>

#include "System.hpp"
#include "util/Logger.hpp"

namespace HGE {
    class ComponentManager;

    class SystemManager {
        std::map<std::string, std::unique_ptr<ISystem>> mTypedSystems;

        public:
        SystemManager() : mTypedSystems(std::map<std::string, std::unique_ptr<ISystem>>()){ }
        ~SystemManager() = default;
        SystemManager& operator=(const SystemManager &sm) = delete;

        /* create a system for a given component. return the system if it already exists. */
        template <class C>
        System<C>* createSystem() {
            auto type = typeid(System<C>).name();
            auto it = mTypedSystems.find(type);

            if(it == mTypedSystems.end()) {
                mTypedSystems[type] = std::make_unique<System<C>>();
            }
            return dynamic_cast<System<C>*>(mTypedSystems[type].get());
        }

        /* */
        template <class C>
        void deleteSystem() {
            
        }

        /* */
        template <class C>
        System<C>* getSystem() {

        }

        /* */
        void run() {
            for(auto const & system : mTypedSystems) {
                system.second->run();
            }
        }
    };
}

#endif