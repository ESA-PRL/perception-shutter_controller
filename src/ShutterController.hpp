#ifndef _SHUTTER_CONTROLLER_HPP_
#define _SHUTTER_CONTROLLER_HPP_

#include <vector>
#include <stdint.h> // for uint8_t

#include "base-logging/Logging.hpp"

#include <shutter_controller/Config.hpp>

#define BASE_LOG_DISABLE

namespace shutter_controller
{

class ShutterController
{
public:
    ShutterController(const Config& config = Config());

    // Computes the mean intensity and checks simple thresholds.
    // If the mean is too bright and there are more bright pixels than allowed,
    // it returns (num too bright pixels) - (threshold for num of too bright pixels).
    // Similarly, it returns a negative value (threshold for num of too dark pixels) - (num too dark pixels)
    // if the image mean is too dark with too many dark pixels.
    // If neither of these two combined conditions holds, this functions returns 0.
    int analyze(const std::vector<uint8_t>& image);

    // Calculate new shutter time based on earlier findings.
    int calcNewShutterTime(const int analysis_result);

private:
    Config config;
    int shutter_time;
    float scaling_factor;
    double weight_old;
    double weight_new;
    uint8_t threshold_max_intensity;
    uint8_t threshold_min_intensity;
    double min_mean;
    double max_mean;
};

}

#endif //_SHUTTER_CONTROLLER_HPP_
