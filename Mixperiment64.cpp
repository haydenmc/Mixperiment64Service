#include "Mixperiment64.h"
#include <stdexcept>

#pragma region Constructor/Destructor
Mixperiment64::Mixperiment64(std::shared_ptr<Config> config) : 
    config(config)
{
    if (this->config->GetNoMupen() == false)
    {
        this->mupenManager = std::make_unique<MupenManager>(
            config->GetHorizontalResolution(),
            config->GetVerticalResolution());
    }
    if (this->config->GetNoStream() == false)
    {
        this->streamer = std::make_unique<Streamer>(
            config->GetHorizontalResolution(),
            config->GetVerticalResolution(),
            config->GetMixerKey());
    }
    this->nodeManager = std::make_unique<NodeManager>(config->GetNodeAppPath());
    std::vector<Rom> roms = config->GetRoms();
    if (roms.size() <= 0)
    {
        throw std::invalid_argument("No roms defined in configuration.");
    }
    this->currentRom = roms.at(0);
    this->init();
}
#pragma endregion

#pragma region Public methods
void Mixperiment64::Start()
{
    if (this->mupenManager != nullptr)
    {
        this->mupenManagerThread = std::thread(&MupenManager::Start, this->mupenManager.get(), this->currentRom);
    }

    if (this->streamer != nullptr)
    {
        this->streamerThread = std::thread(&Streamer::Start, this->streamer.get());
    }

    if (this->nodeManager != nullptr)
    {
        this->nodeThread = std::thread(&NodeManager::Start, this->nodeManager.get());
    }

    if (this->mupenManager != nullptr)
    {
        this->mupenManagerThread.join();
    }

    if (this->streamer != nullptr)
    {
        this->streamerThread.join();
    }

    if (this->nodeManager != nullptr)
    {
        this->nodeThread.join();
    }
}
#pragma endregion

#pragma region Private methods
void Mixperiment64::init()
{
    
}
#pragma endregion