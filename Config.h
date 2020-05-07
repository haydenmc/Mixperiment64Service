#pragma once
#include <string>
#include <vector>

struct Rom
{
    std::string FilePath;
    std::string Name;
};

class Config
{
public:
    /* Constructor/Destructor */
    Config();
    // ~Config();

    /* Public methods */
    void Load();
    void ParseArgs(unsigned int argc, char* argv[]);
    
    /* Getters */
    const std::vector<Rom> GetRoms();
    const unsigned int GetHorizontalResolution();
    const unsigned int GetVerticalResolution();
    const std::string GetMixerKey();
    const std::string GetNodeAppPath();
    const std::string GetNodeHttpsKeyPath();
    const std::string GetNodeHttpsCertPath();
    const bool GetNoStream();
    const bool GetNoMupen();
    const bool GetNoNode();

private:
    /* Private methods */
    void loadConfigFile(std::string fileName);

    /* Private members */
    std::vector<Rom> roms;
    unsigned int horizontalResolution = 1280;
    unsigned int verticalResolution = 720;
    std::string mixerKey;
    std::string nodeAppPath;
    std::string nodeHttpsKeyPath;
    std::string nodeHttpsCertPath;
    bool noStream = false;
    bool noMupen = false;
    bool noNode = false;
};