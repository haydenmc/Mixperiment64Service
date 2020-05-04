#include "Config.h"
#include "Mixperiment64.h"
#include <memory>

int main(int argc, char* argv[])
{
    std::shared_ptr<Config> config = std::make_shared<Config>();
    config->Load();
    config->ParseArgs(argc, argv);
    std::unique_ptr<Mixperiment64> service = std::make_unique<Mixperiment64>(config);
    service->Start();
}