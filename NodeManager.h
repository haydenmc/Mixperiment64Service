#pragma once
#include <string>
#include <sys/types.h>

class NodeManager
{
public:
    /* Constructor/Destructor */
    NodeManager(std::string nodeAppPath,
        std::string nodeHttpsKeyPath,
        std::string nodeHttpsCertPath);
    //~NodeManager();

    /* Public methods */
    void Start();

private:
    /* Private methods */
    void init();

    /* Private members */
    std::string nodeAppPath;
    std::string nodeHttpsKeyPath;
    std::string nodeHttpsCertPath;
    pid_t nodePid;
};