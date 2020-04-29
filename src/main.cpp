/**
 * Copyright D. price 2020.
 */

#include <iostream>

#include "framework/Engine.hpp"
#include "framework/GameEnvironment.hpp"

#include "../src/graphics/OpenGlModule.hpp"

/* Function Declarations */
GLFWwindow* OpenGlInit(const char * title, int windowX, int windowY);
HGE::Engine *HGE::Engine::mEngine;

/**
 * Main Entry Point.
 */
int main(void) 
{
    HGE::Engine* engine = HGE::Engine::getInstance();
    if(!engine->Init(new HGE::OpenGlModule())) {
        return -1;
    }

    HGE::GameEnvironment* gameEnvironment = new HGE::GameEnvironment();
    gameEnvironment->Init();
    // TODO: Load inital config and get initial level?
    // TODO: Load inital level and init the gameEnvironment
    
    /* Main Loop whilst window is open. */
    while(engine->getInstance()->getGraphicsModule()->isWindowOpen())
    {
        gameEnvironment->GameLoop();
    }

    engine->terminate();
    return 0;
}