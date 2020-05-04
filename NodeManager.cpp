#include "NodeManager.h"
#include <iostream>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdexcept>

#pragma region Constructor/Destructor
NodeManager::NodeManager()
{ }
#pragma endregion

#pragma region Public methods
void NodeManager::Start()
{
    // Fork off for node service process
    pid_t originalPid = getpid();
    pid_t nodePid;
    int status;
    nodePid = fork();
    if (nodePid < 0)
    {
        throw std::runtime_error("Error forking to new process for node.");
    }
    else if (nodePid == 0)
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
        // Spin up node
        execlp("node", "node",
            "/home/hayden/Source/Mixer64/build/app.js",
            NULL);
    }
    else
    {
        // Wait for node
        wait(&status);
        std::cout << "Node exited with status " << status << std::endl;
    }
}
#pragma endregion

#pragma region Private methods
void NodeManager::init()
{

}
#pragma endregion