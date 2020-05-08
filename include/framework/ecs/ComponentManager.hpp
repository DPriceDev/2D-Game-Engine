#ifndef HESTIA_FRAMEWORK_ECS_COMPONENTMANAGER_H_
#define HESTIA_FRAMEWORK_ECS_COMPONENTMANAGER_H_

#include <map>
#include <typeinfo>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "Component.hpp"
#include "SystemManager.hpp"

#include "util/Logger.hpp"

namespace HGE {
    using UID = int;
    
    /**
     * Component Array Interface
     */
    class IComponentArray {
        public:
        virtual ~IComponentArray() = default;
    };

    /**
     * Component Array
     */
    template <class C>
    class ComponentArray : public IComponentArray {
        std::vector<C> mComponents;
        friend class ComponentManager;

        public:
        ComponentArray() : mComponents(std::vector<C>()) { }
        ~ComponentArray() = default;
        ComponentArray& operator= (const ComponentArray &other) = delete;

        const std::vector<C>& getComponents() const { return mComponents; }
    };

    /**
     * Component Manager
     */
    class ComponentManager {
        std::map<std::string, std::unique_ptr<IComponentArray>> mTypedComponentArrays;
        SystemManager* mSystemManager;

        public:
        ComponentManager() : mTypedComponentArrays(std::map<std::string, std::unique_ptr<IComponentArray>>()) { }
        ~ComponentManager() { }
        ComponentManager& operator= (const ComponentManager &other) = delete;

        /* Assign the System manager. */
        void setSystemManager(SystemManager* pSystemManager) { 
            mSystemManager = pSystemManager; 
        }

        /* Create a component vector within the typed component map. returns the vector. */
        template <class C>
        ComponentArray<C>* createComponentArray() {
            auto type = typeid(C).name();
            mTypedComponentArrays[type] = std::make_unique<ComponentArray<C>>();
            auto componentArray = dynamic_cast<ComponentArray<C>*>(mTypedComponentArrays[type].get());
            mSystemManager->createSystem<C>();
            return componentArray;
        }

        /* Creates a component and adds it to the corresponding typed vector. returns the component. */
        /* TODO: Check if need to delete pointer?? */
        template <class C, typename ... Args>
        C* createComponent(Args&& ... args) {
            auto type = typeid(C).name();
            auto it = mTypedComponentArrays.find(type);
            ComponentArray<C>* pArray;

            if(it == mTypedComponentArrays.end()) {
                pArray = createComponentArray<C>();
            } else {
                pArray = dynamic_cast<ComponentArray<C>*>(mTypedComponentArrays[type].get());
            }
            pArray->mComponents.push_back(C(std::forward<Args>(args)...));
            Logger::getInstance()->logDebug("Component Manager", "Component Created!");
            return &pArray->mComponents.back();
        }

        /* Takes the pointer for a component and removes it from the selected component array, if it exists. */
        template <class C>
        void deleteComponent(C* component) {
            auto type = typeid(C).name();
            auto it = mTypedComponentArrays.find(type);

            if(it != mTypedComponentArrays.end()) {
                auto pArray = dynamic_cast<ComponentArray<C>*>(mTypedComponentArrays[type].get());
                auto deleteIt = pArray.mComponents.find_if(pArray.mComponents.begin(), pArray.mComponents.end(), [&] (auto & arrayComponent) { 
                    return component->getOwnerUID() == arrayComponent.getOwnerUID();
                });
                pArray.mComponents.erase(deleteIt);
            }
        }

        /* deletes a component by its owning id, if it exists. */
        template <class C>
        void deleteComponent(UID ownerId) {
            auto type = typeid(C).name();
            auto it = mTypedComponentArrays.find(type);

            if(it != mTypedComponentArrays.end()) {
                auto pArray = dynamic_cast<ComponentArray<C>*>(mTypedComponentArrays[type].get());
                auto deleteIt = pArray.mComponents.find_if(pArray.mComponents.begin(), pArray.mComponents.end(), [&] (auto & arrayComponent) { 
                    return ownerId == arrayComponent.getOwnerUID();
                });
                pArray.mComponents.erase(deleteIt);
            }
        }

        /* Get the component vector for the given type. creates and returns the vector if it doesn't exist. */
        template <class C>
        ComponentArray<C>* getComponentArray() {
            auto type = typeid(C).name();
            auto it = mTypedComponentArrays.find(type);

            if(it == mTypedComponentArrays.end()) {
                return createComponentArray<C>();
            } else {
                return dynamic_cast<ComponentArray<C>*>(mTypedComponentArrays[type].get());
            }
        }
    };
}

#endif