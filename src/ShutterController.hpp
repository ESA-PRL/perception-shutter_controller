#ifndef _SHUTTER_CONTROLLER_HPP_
#define _SHUTTER_CONTROLLER_HPP_

#include <shutter_controller/Config.hpp>
#include <vector>
#include <stdint.h> // for uint8_t

namespace shutter_controller
{

class ShutterController
{
public:
    ShutterController(const Config& config = Config());

    /** Computes the mean intensity and checks simple thresholds.
     * If the mean is too bright and there are more bright pixels than allowed, 
     * it returns (num too bright pixels) - (threshold for num of too bright pixels).
     * Similarly, it returns a negative value (threshold for num of too dark pixels) - (num too dark pixels)
     * if the image mean is too dark with too many dark pixels.
     * If neither of these two combined conditions holds, this functions returns 0.
     */
    //int analyze(base::samples::frame::Frame frame);
    int analyze(std::vector<uint8_t> image);

    /**
     * Calculate new shutter time based on earlier findings.
     */
    int calcNewShutterTime(const int analysisResult);

private:
    /**
     * Contains the configuration of the shutter controller.
     * Properties are initial shutter time, thresholds (TODO), ...
     */
    Config config;
    int shutterTime;
    float scalingFactor;
    uint8_t threshMaxIntensity;
    uint8_t threshMinIntensity;
    uint8_t minMean;
    uint8_t maxMean;
};

}

#endif //_SHUTTER_CONTROLLER_HPP_
