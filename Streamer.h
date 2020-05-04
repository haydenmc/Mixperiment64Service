#pragma once
#include <gst/gst.h>
#include <string>

class Streamer
{
public:
    /* Constructor/Destructor */
    Streamer(
        unsigned int horizontalResolution,
        unsigned int verticalResolution,
        std::string mixerKey);
    //~Streamer();

    /* Public methods */
    void Start();
private:
    /* Private methods */
    void init();

    /* Private members */
    /* Configuration values */
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    std::string mixerKey;

    /* Gstreamer Pipeline stuff */
    GstBus*     bus;
    GstElement* pipeline;
    GstElement* videoSource;
    GstElement* videoConverter;
    GstElement* videoCapsFilter;
    GstElement* videoEncoder;
    GstElement* videoQueue;
    GstElement* audioSource;
    GstElement* audioConverter;
    GstElement* audioEncoder;
    GstElement* ftl;

};