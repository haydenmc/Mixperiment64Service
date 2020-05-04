#include "Streamer.h"
#include <iostream>

#pragma region Constructor/Destructor
Streamer::Streamer(
    unsigned int horizontalResolution,
    unsigned int verticalResolution,
    std::string mixerKey
) : 
    horizontalResolution(horizontalResolution),
    verticalResolution(verticalResolution),
    mixerKey(mixerKey)
{
    this->init();
}
#pragma endregion

#pragma region Public methods
void Streamer::Start()
{
    // Play!
    GstStateChangeReturn ret = gst_element_set_state (this->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        gst_object_unref (this->pipeline);
        throw std::runtime_error("Unable to set the pipeline to the playing state.");
    }
    this->bus = gst_element_get_bus (this->pipeline);

    // Listen to gst message bus
    gboolean terminate = FALSE;
    GstMessage *msg;
    do
    {
        msg = gst_bus_timed_pop_filtered(bus,
            GST_CLOCK_TIME_NONE,
            static_cast<GstMessageType>(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
        
        if (msg != NULL)
        {
            GError* err;
            gchar* debug_info;

            switch (GST_MESSAGE_TYPE(msg))
            {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error (msg, &err, &debug_info);
                std::cerr << "GST: Error received from element " <<
                    GST_OBJECT_NAME (msg->src) <<
                    ": " <<
                    err->message <<
                    std::endl;
                std::cerr << "GST: Debugging information: " << 
                    (debug_info ? debug_info : "none") << 
                    std::endl;
                g_clear_error (&err);
                g_free (debug_info);
                terminate = TRUE;
                break;
            case GST_MESSAGE_EOS:
                std::cout << "GST: End-Of-Stream reached." << std::endl;
                terminate = TRUE;
                break;
            case GST_MESSAGE_STATE_CHANGED:
                if (GST_MESSAGE_SRC(msg) == GST_OBJECT(this->pipeline))
                {
                    GstState old_state;
                    GstState new_state;
                    GstState pending_state;
                    gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                    std::cout << "GST: Pipeline state changed from " << 
                        gst_element_state_get_name(old_state) << 
                        " to " << 
                        gst_element_state_get_name(new_state) << 
                        std::endl;
                }
                break;
            default:
                std::cout << "GST: Unexpected message received." << std::endl;
                break;
            }
            gst_message_unref(msg);
        }
    }
    while (!terminate);

    std::cout << "GST: Terminated." << std::endl;
}
#pragma endregion

#pragma region Private methods
void Streamer::init()
{
    // Init gstreamer
    gst_init(NULL, NULL);

    // Create elements
    std::cout << "GST: Creating elements..." << std::endl;
    this->videoSource     = gst_element_factory_make("ximagesrc",    "source");
    this->videoConverter  = gst_element_factory_make("videoconvert", "videoConverter");
    this->videoCapsFilter = gst_element_factory_make("capsfilter",   "videoCapsFilter");
    this->videoEncoder    = gst_element_factory_make("omxh264enc",   "videoEncoder");
    this->videoQueue      = gst_element_factory_make("queue",        "videoQueue");
    this->audioSource     = gst_element_factory_make("pulsesrc",     "audioSource");
    this->audioConverter  = gst_element_factory_make("audioconvert", "audioConverter");
    this->audioEncoder    = gst_element_factory_make("opusenc",      "audioEncoder");
    this->ftl             = gst_element_factory_make("ftlsink",      "ftl");

    // Configure source element
    std::cout << "GST: Configuring video source..." << std::endl;
    g_object_set(this->videoSource,
        "startx",       0,
        "starty",       0,
        "endx",         (this->horizontalResolution - 1), // Not sure why we need to subtract 1...
        "endy",         (this->verticalResolution - 1),
        "use-damage",   FALSE,
        "show-pointer", 0,
        NULL);

    // Configure video caps
    std::cout << "GST: Configuring video caps..." << std::endl;
    GstCaps *caps = gst_caps_new_simple ("video/x-raw",
        "format", G_TYPE_STRING, "I420",
        "framerate", GST_TYPE_FRACTION, 30, 1,
        "width", G_TYPE_INT, this->horizontalResolution,
        "height", G_TYPE_INT, this->verticalResolution,
        NULL);
    g_object_set(this->videoCapsFilter,
        "caps", caps,
        NULL);
    gst_caps_unref(caps);

    // Configure encoder
    std::cout << "GST: Configuring video encoder..." << std::endl;
    g_object_set(this->videoEncoder,
        "preset-level", 0,
        "num-B-Frames", 0,
        "control-rate", 2,
        "iframeinterval", 30,
        "profile", 2,
        "bitrate", 2800000,
        NULL);

    // Configure audio source
    std::cout << "GST: Configuring audio source..." << std::endl;
    g_object_set(this->audioSource,
        "device", "alsa_output.platform-sound.analog-stereo.monitor",
        NULL);

    // Configure FTL
    std::cout << "GST: Configuring FTL..." << std::endl;
    g_object_set(this->ftl,
        "stream-key", this->mixerKey.c_str(),
        NULL);

    // Pipeline
    this->pipeline = gst_pipeline_new("pipeline");

    // Check for creation errors
    if (!this->videoSource     ||
        !this->videoConverter  ||
        !this->videoCapsFilter ||
        !this->videoEncoder    ||
        !this->videoQueue      ||
        !this->audioSource     ||
        !this->audioConverter  ||
        !this->audioEncoder    ||
        !this->ftl)
    {
        throw std::runtime_error("Could not create gstreamer elements!");
    }

    // Set up elements in pipeline
    std::cout << "GST: Adding elements..." << std::endl;
    gst_bin_add_many (GST_BIN(this->pipeline),
        this->videoSource,
        this->videoConverter,
        this->videoCapsFilter,
        this->videoEncoder,
        this->videoQueue,
        this->audioSource,
        this->audioConverter,
        this->audioEncoder,
        this->ftl,
        NULL);

    bool linkResult = true;
    // Link up video
    std::cout << "GST: Linking video elements..." << std::endl;
    linkResult &= gst_element_link_many(
        this->videoSource,
        this->videoConverter,
        this->videoCapsFilter,
        this->videoEncoder,
        this->videoQueue,
        NULL);
    linkResult &= gst_element_link_pads(this->videoQueue, "src", this->ftl, "videosink");

    // Link up audio
    std::cout << "GST: Linking audio elements..." << std::endl;
    linkResult &= gst_element_link_many(
        this->audioSource,
        this->audioConverter,
        this->audioEncoder,
        NULL);
    linkResult &= gst_element_link_pads(this->audioEncoder, "src", this->ftl, "audiosink");

    if (!linkResult)
    {
        gst_object_unref (this->pipeline);
        throw std::runtime_error("Could not link gstreamer elements!");
    }
}
#pragma endregion