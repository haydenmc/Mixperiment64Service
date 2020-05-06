#pragma once
#include <memory>
#include <string>
#include <thread>
#include "Config.h"
#include "MupenManager.h"
#include "Streamer.h"
#include "NodeManager.h"

class Mixperiment64
{
public:
    /* Constructor/Destructor */
    Mixperiment64(std::shared_ptr<Config> config);
    //~Mixperiment64();

    /* Public methods */
    void Start();
private:
    /* Private methods */
    void init();

    /* Private members */
    std::shared_ptr<Config> config;
    Rom currentRom;

    /* Helper threads */
    std::thread mupenManagerThread;
    std::thread streamerThread;
    std::thread nodeThread;

    /* Helper instances */
    std::unique_ptr<MupenManager> mupenManager;
    std::unique_ptr<Streamer> streamer;
    std::unique_ptr<NodeManager> nodeManager;
};