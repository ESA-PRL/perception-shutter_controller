#ifndef _SHUTTER_CONTROLLER_CONFIG_HPP_
#define _SHUTTER_CONTROLLER_CONFIG_HPP_

#include <stdint.h> // for uint8_t

namespace shutter_controller
{
    struct Config
    {
        // shutter time
        int initShutterTime;
        int maxShutterTime;
        int minShutterTime;

        // intensity thresholds
        uint8_t maxIntensity;
        double factMaxIntensity; // pixels brighter than <maxIntensity>*<factMaxIntensity> are considered too bright
        double factMinIntensity; // pixels darker   than <maxIntensity>*<factMinIntensity> are considered too dark

        // tolerance for mean intensity
        double factMinMean; // <maxIntensity>*<factMinMean>
        double factMaxMean; // <maxIntensity>*<factMaxMean>

        // compute and apply new shutter time every <shutterCompPeriod>th frame
        int shutterCompPeriod;

        // new shutter time is computed as (x=weightOldShutterTime):
        // newShutterTime = x*oldShutterTime + (1-x)*newGuess
        double weightOldShutterTime;

        Config()
            :
            initShutterTime(1500),
            maxShutterTime(3904),
            minShutterTime(3),
            maxIntensity(255),
            factMaxIntensity(0.65),
            factMinIntensity(0.05),
            factMinMean(0.30),
            factMaxMean(0.40),
            shutterCompPeriod(3),
            weightOldShutterTime(0.995)
        {
        }
    };
}

#endif //_SHUTTER_CONTROLLER_CONFIG_HPP_
