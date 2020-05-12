#ifndef HESTIA_FRAMEWORK_CAMERA_SYSTEM_H
#define HESTIA_FRAMEWORK_CAMERA_SYSTEM_H

#include "framework/Engine.h"
#include "framework/ecs/IComponent.h"
#include "framework/ecs/System.h"
#include "camera/CameraManager.h"

#include "util/Logger.h"

namespace HGE {

    /**
     * Camera IComponent
     */
    struct CameraComponent : public IComponent {
        CameraView mCameraView;
        bool mIsActiveCamera, mIsRequestingActive;

        CameraComponent(UID ownerId, CameraView cameraView, bool requestCamera = false)
            : IComponent(ownerId),
            mCameraView(cameraView),
            mIsActiveCamera(false),
            mIsRequestingActive(requestCamera) { }
        ~CameraComponent() override = default;
    };

    /**
     * Camera System
     */
    template <>
    class System<CameraComponent> : public ISystem {
        ComponentArray<CameraComponent>* mCameraArray;

        public:
        explicit System(ComponentArray<CameraComponent>* cameraArray) : mCameraArray(cameraArray) { }
        ~System() override = default;

        void run() override {

            auto requestIt = find_if(mCameraArray->getComponents().begin(), mCameraArray->getComponents().end(),
                    [] (auto const & pComponent) { return pComponent->mIsRequestingActive == true; });

            if( requestIt != mCameraArray->getComponents().end()) {

                for(auto & component : mCameraArray->getComponents()) {
                    component->mIsActiveCamera = false;
                }

                requestIt->get()->mIsActiveCamera = true;
                requestIt->get()->mIsRequestingActive = false;
                Engine::cameraManager()->setCameraView(requestIt->get()->mCameraView);
            } else {
                auto activeIt = find_if(mCameraArray->getComponents().begin(), mCameraArray->getComponents().end(),
                                  [] (auto const & pComponent) { return pComponent->mIsActiveCamera == true; });
                if(activeIt !=  mCameraArray->getComponents().end()) {
                    Engine::cameraManager()->setCameraView(activeIt->get()->mCameraView);
                }
            }
        }
    };
}

#endif