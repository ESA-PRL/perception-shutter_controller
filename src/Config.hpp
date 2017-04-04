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

        double factNumTooBright; // allow only <pixelCount>*<factNumTooBright> many pixels which are too bright
        double factNumTooDark;

        // tolerance for mean intensity
        double factMinMean; // <maxIntensity>*<factMinMean>
        double factMaxMean; // <maxIntensity>*<factMaxMean>

        // the higher the precision value, the lower the change rate
        double precision;

        // compute and apply new shutter time every <shutterCompPeriod>th frame
        int shutterCompPeriod;

        Config()
            :
            initShutterTime(1500),
            maxShutterTime(3904),
            minShutterTime(3),
            maxIntensity(255),
            factMaxIntensity(0.65),
            factMinIntensity(0.05),
            factNumTooBright(0.10),
            factNumTooDark(0.20),
            factMinMean(0.30),
            factMaxMean(0.40),
            precision(32.0),
            shutterCompPeriod(3)
        {
        }
    };
}

#endif //_SHUTTER_CONTROLLER_CONFIG_HPP_
