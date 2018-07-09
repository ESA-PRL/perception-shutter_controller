#ifndef _SHUTTER_CONTROLLER_CONFIG_HPP_
#define _SHUTTER_CONTROLLER_CONFIG_HPP_

#include <stdint.h> // for uint8_t

namespace shutter_controller
{
    struct Config
    {
        // shutter time
        int init_shutter_time;
        int max_shutter_time;
        int min_shutter_time;

        // intensity thresholds
        uint8_t max_intensity;
        double fact_max_intensity; // pixels brighter than maxIntensity*factMaxIntensity are considered too bright
        double fact_min_intensity; // pixels darker   than maxIntensity*factMinIntensity are considered too dark

        // tolerance for mean intensity
        double fact_min_mean; // maxIntensity*factMinMean
        double fact_max_mean; // maxIntensity*factMaxMean

        // compute and apply new shutter time every shutter_computation_period-th frame
        int shutter_computation_period;

        // new shutter time is computed as (x=weightOldShutterTime):
        // newShutterTime = x*oldShutterTime + (1-x)*newGuess
        double weight_old_shutter_time;

        Config()
            :
            init_shutter_time(1500),
            max_shutter_time(3904),
            min_shutter_time(3),
            max_intensity(255),
            fact_max_intensity(0.65),
            fact_min_intensity(0.05),
            fact_min_mean(0.30),
            fact_max_mean(0.40),
            shutter_computation_period(3),
            weight_old_shutter_time(0.995)
        {
        }
    };
}

#endif //_SHUTTER_CONTROLLER_CONFIG_HPP_
