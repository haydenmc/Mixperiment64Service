#pragma once
#include "Config.h"
#include <sys/types.h>

class MupenManager
{
public:
    /* Constructor/Destructor */
    MupenManager(
        unsigned int horizontalResolution,
        unsigned int verticalResolution);
    //~MupenManager();

    /* Public methods */
    void Start(Rom rom);
private:
    /* Private methods */
    void init();

    /* Private members */
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    pid_t mupenPid;
};