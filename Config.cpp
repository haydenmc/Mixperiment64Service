#include "Config.h"
#include <filesystem>
#include <iostream>
#include <tinyxml2.h>
#include <string.h>

using namespace tinyxml2;

#pragma region Constructor/Destructor
Config::Config()
{ }
#pragma endregion

#pragma region Public methods
void Config::Load()
{
    this->loadConfigFile("config.default.xml");
    this->loadConfigFile("config.xml");
}

void Config::ParseArgs(unsigned int argc, char* argv[])
{
    for(unsigned int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--no-mupen") == 0)
        {
            std::cout << "CONFIG: Mupen disabled." << std::endl;
            this->noMupen = true;
        }
        else if (strcmp(argv[i], "--no-stream") == 0)
        {
            std::cout << "CONFIG: Streaming disabled." << std::endl;
            this->noStream = true;
        }
    }
}

const std::vector<Rom> Config::GetRoms()
{
    return this->roms;
}

const unsigned int Config::GetHorizontalResolution()
{
    return this->horizontalResolution;
}

const unsigned int Config::GetVerticalResolution()
{
    return this->verticalResolution;
}

const std::string Config::GetMixerKey()
{
    return this->mixerKey;
}

const std::string Config::GetNodeAppPath()
{
    return this->nodeAppPath;
}

const bool Config::GetNoStream()
{
    return this->noStream;
}

const bool Config::GetNoMupen()
{
    return this->noMupen;
}
#pragma endregion

#pragma region Private methods
void Config::loadConfigFile(std::string fileName)
{
    if (std::filesystem::exists(fileName) == false)
    {
        std::cout << "CONFIG: `" << fileName <<"` does not exist.";
        return;
    }
    std::cout << "CONFIG: Loading config from `" << fileName << "`" << std::endl;
    XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    const XMLElement* rootElement = doc.FirstChildElement("config");
    if (rootElement == nullptr)
    {
        std::cout << "CONFIG: No root config element found in `" << fileName << "`" << std::endl;
        return;
    }

    // Horizontal resolution
    if (const XMLElement* el = rootElement->FirstChildElement("horizontal-resolution"))
    {
        this->horizontalResolution = el->UnsignedText(0);
        std::cout << 
            "CONFIG: Horizontal resolution `" <<
            this->horizontalResolution <<
            "`" <<
            std::endl;
    }

    // Vertical resolution
    if (const XMLElement* el = rootElement->FirstChildElement("vertical-resolution"))
    {
        this->verticalResolution = el->UnsignedText(0);
        std::cout << 
            "CONFIG: Vertical resolution `" <<
            this->verticalResolution <<
            "`" <<
            std::endl;
    }

    // Node app path
    if (const XMLElement* el = rootElement->FirstChildElement("node-app-path"))
    {
        const char* nodeAppPathText = el->GetText();
        if (nodeAppPathText != nullptr)
        {
            this->nodeAppPath = std::string(nodeAppPathText);
            std::cout << "CONFIG: Node app path: `" << this->nodeAppPath << "`" << std::endl;
        }
    }

    // Mixer key
    if (const XMLElement* el = rootElement->FirstChildElement("mixer-key"))
    {
        const char* mixerKeyText = el->GetText();
        if (mixerKeyText != nullptr)
        {
            this->mixerKey = std::string(mixerKeyText);
            std::cout << "CONFIG: Mixer Key: `REDACTED`" << std::endl;
        }
    }

    // Load ROMs
    if (const XMLElement* el = rootElement->FirstChildElement("roms"))
    {
        const XMLElement* currentRomElement = el->FirstChildElement();
        while (currentRomElement != nullptr)
        {
            Rom rom = Rom{};
            const char* nameText = currentRomElement->Attribute("name");
            const char* filePathText = currentRomElement->Attribute("file-path");
            if ((nameText == nullptr) || (filePathText == nullptr))
            {
                std::cerr << "CONFIG: Rom entry missing name or file-path attribute.";
                currentRomElement = currentRomElement->NextSiblingElement();
                continue;
            }
            rom.Name = std::string(nameText);
            rom.FilePath = std::string(filePathText);
            this->roms.push_back(rom);
            std::cout << 
                "CONFIG: Added rom `" << 
                rom.Name <<
                "` @ `" <<
                rom.FilePath <<
                "`" <<
                std::endl;
            currentRomElement = currentRomElement->NextSiblingElement();
        }
    }
    else
    {
        std::cout << "CONFIG: No `roms` section found in `" << fileName << "`" << std::endl;
    }
}
#pragma endregion