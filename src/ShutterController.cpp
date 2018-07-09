#include "ShutterController.hpp"

using namespace shutter_controller;

ShutterController::ShutterController(const Config& new_config)
    : config(new_config), shutter_time(config.init_shutter_time)
{
    threshold_min_intensity = static_cast<uint8_t>(config.max_intensity * config.fact_min_intensity);
    threshold_max_intensity = static_cast<uint8_t>(config.max_intensity * config.fact_max_intensity);
    min_mean = config.max_intensity * config.fact_min_mean;
    max_mean = config.max_intensity * config.fact_max_mean;
    weight_old = config.weight_old_shutter_time;
    weight_new = 1.0 - weight_old;
}

int ShutterController::analyze(const std::vector<uint8_t>& image)
{
    int pixel_count = image.size();

    // use this to scale result to be within allowed shutter time boundaries
    scaling_factor = (config.max_shutter_time - config.min_shutter_time)/static_cast<float>(pixel_count);

    int num_too_bright = 0;
    int num_too_dark   = 0;
    int intensity_sum  = 0;

    // iterate over image to calculate mean and count bright and dark pixels
    for (int i = 0; i < pixel_count; i++)
    {
        if (image[i] > threshold_max_intensity)
            num_too_bright++;
        if (image[i] < threshold_min_intensity)
            num_too_dark++;
        intensity_sum += image[i];
    }
    double mean = static_cast<double>(intensity_sum) / static_cast<double>(pixel_count);

    // decide whether image is too bright or dark
    if (mean > max_mean)
    {
        // image is too bright, return positive value
        LOG_DEBUG_S << "Mean " << mean << " bigger than max mean " << max_mean;
        return num_too_bright;
    }
    else if (mean < min_mean)
    {
        // image is too dark, return negative value
        LOG_DEBUG_S << "Mean " << mean << " smaller than min mean " << min_mean;
        return -num_too_dark;
    }
    // image is within bounds
    return 0;
}

int ShutterController::calcNewShutterTime(const int analysis_result)
{
    shutter_time = weight_old*shutter_time + weight_new*(shutter_time - (scaling_factor * analysis_result));
    shutter_time = std::max(config.min_shutter_time, shutter_time);
    shutter_time = std::min(shutter_time, config.max_shutter_time);
    return shutter_time;
}
