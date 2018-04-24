#include "ShutterController.hpp"
#include <iostream>

namespace shutter_controller
{

    ShutterController::ShutterController(const Config& nConfig)
        : config(nConfig), shutterTime(config.initShutterTime)
    {
        threshMaxIntensity = (uint8_t)(config.maxIntensity * config.factMaxIntensity);
        threshMinIntensity = (uint8_t)(config.maxIntensity * config.factMinIntensity);
        minMean = config.maxIntensity * config.factMinMean;
        maxMean = config.maxIntensity * config.factMaxMean;
        weightOld = config.weightOldShutterTime;
        weightNew = 1.0 - weightOld;
    }

    //int ShutterController::analyze(base::samples::frame::Frame frame)
    int ShutterController::analyze(std::vector<uint8_t> image)
    {
        int pixelCount = image.size()/sizeof(uint8_t);

        // use this to scale result to be be within allowed shutter time boundaries
        scalingFactor = (config.maxShutterTime - config.minShutterTime)/(float)pixelCount;

        int numTooBright = 0;
        int numTooDark   = 0;
        int intensitySum = 0;
        // iterate over image to calculate mean and cound bright and dark pixels
        for (int i = 0; i < pixelCount; i++)
        {
            if (image[i] > threshMaxIntensity)
                numTooBright++;
            if (image[i] < threshMinIntensity)
                numTooDark++;
            intensitySum += image[i];
        }
        double mean = (double)intensitySum / (double)pixelCount;

        // decide whether image is too bright or dark
        if (mean > maxMean)
        {
            // image is too bright, return positive value
            LOG_DEBUG_S << "Mean " << mean << " bigger than max mean " << (int)maxMean;
            return numTooBright;
        }
        else if (mean < minMean)
        {
            // image is too dark, return negative value
            LOG_DEBUG_S << "Mean " << mean << " smaller than min mean " << (int)minMean;
            return -numTooDark;
        }
        // image is within bounds
        return 0;
    }

    int ShutterController::calcNewShutterTime(const int analysisResult)
    {
        shutterTime = weightOld*shutterTime + weightNew*(shutterTime - (scalingFactor * analysisResult));
        shutterTime = std::max(config.minShutterTime, shutterTime);
        shutterTime = std::min(shutterTime, config.maxShutterTime);
        return shutterTime;
    }

}
