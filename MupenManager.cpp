#include "MupenManager.h"
#include <iostream>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <stdexcept>
#include <unistd.h>
#include <string>
#include <sstream>

#pragma region Constructor/Destructor
MupenManager::MupenManager(
    unsigned int horizontalResolution,
    unsigned int verticalResolution
) : 
    horizontalResolution(horizontalResolution),
    verticalResolution(verticalResolution)
{
    this->init();
}
#pragma endregion

#pragma region Public methods
void MupenManager::Start(Rom rom)
{
    // Fork off for mupen64 program
    pid_t originalPid = getpid();
    pid_t mupenPid;
    int status;
    mupenPid = fork();
    if (mupenPid < 0)
    {
        throw new std::runtime_error("Error forking to new process for mupen.");
    }
    else if (mupenPid == 0)
    {
        // Die if parent dies
        int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
        if (r < 0)
        {
            throw std::runtime_error("Error setting process properties");
        }
        if (getppid() != originalPid)
        {
            throw std::runtime_error("Parent process died while child was starting.");
        }
        // Spin up mupen
        std::stringstream resolutionStr;
        resolutionStr << 
            std::to_string(this->horizontalResolution) << 
            "x" << 
            std::to_string(this->verticalResolution);
        execlp("mupen64plus", "mupen64plus",
            "--resolution", resolutionStr.str().c_str(),
            "--input", "/home/hayden/Source/mupen64plus-input-udp/build/libmupen64plus-input-udp.so",
            "--gfx", "mupen64plus-video-rice",
            rom.FilePath.c_str(),
            NULL);
    }
    else
    {
        // Store pid
        this->mupenPid = mupenPid;
        std::cout << "Mupen started - PID `" << this->mupenPid << "`" << std::endl;
        // Wait for mupen
        waitpid(this->mupenPid, &status, 0);
        std::cout << "Mupen exited with status " << std::to_string(status) << std::endl;
    }
}
#pragma endregion

#pragma region Private methods
void MupenManager::init()
{

}
#pragma endregion