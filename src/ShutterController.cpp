#include "ShutterController.hpp"
#include <iostream>

namespace shutter_controller
{

ShutterController::ShutterController(const Config& nConfig)
    : config(nConfig), shutterTime(config.initShutterTime)
{
    threshMaxIntensity = (uint8_t)(config.maxIntensity * config.factMaxIntensity);
    threshMinIntensity = (uint8_t)(config.maxIntensity * config.factMinIntensity);
    minMean = (uint8_t)(config.maxIntensity * config.factMinMean);
    maxMean = (uint8_t)(config.maxIntensity * config.factMaxMean);
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
        // std::cout << mean << " BIGGER THAN MAX MEAN " << (int)maxMean << std::endl;
        return numTooBright;
    }
    else if (mean < minMean)
    {
        // image is too dark, return negative value
        // std::cout << mean << " SMALLER THAN MIN MEAN " << (int)minMean << std::endl;
        return -numTooDark;
    }
    // image is within bounds
    return 0;
}

int ShutterController::calcNewShutterTime(const int analysisResult)
{
    shutterTime = 0.9*shutterTime + 0.1*(shutterTime - (scalingFactor * analysisResult));
    shutterTime = std::max(config.minShutterTime, shutterTime);
    shutterTime = std::min(shutterTime, config.maxShutterTime);
    return shutterTime;
}

}
