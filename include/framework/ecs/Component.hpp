#ifndef HESTIA_FRAMEWORK_ECS_COMPONENT_HPP_
#define HESTIA_FRAMEWORK_ECS_COMPONENT_HPP_

#include <string>
#include <memory>

namespace HGE {
    using UID = int;

    /**
     * Component Interface
     */
    class Component {

        UID mOwnerUID;

        public:
        Component(UID ownerId) : mOwnerUID(ownerId) { }
        virtual ~Component() = default;
        
        UID getOwnerUID() const { 
            return mOwnerUID; 
        }
    };

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
        std::vector<std::unique_ptr<C>> mComponents;
        friend class ComponentManager;

        public:
        ComponentArray() : mComponents(std::vector<std::unique_ptr<C>>()) { }
        ~ComponentArray() = default;
        ComponentArray& operator= (const ComponentArray &other) = delete;

        std::vector<std::unique_ptr<C>>& getComponents() { return mComponents; }
    };
}

#endif