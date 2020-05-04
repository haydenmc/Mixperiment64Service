#pragma once

class MupenManager
{
public:
    /* Constructor/Destructor */
    MupenManager(
        unsigned int horizontalResolution,
        unsigned int verticalResolution);
    //~MupenManager();

    /* Public methods */
    void Start();
private:
    /* Private methods */
    void init();

    /* Private members */
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
};