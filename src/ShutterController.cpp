#include "ShutterController.hpp"
#include <iostream> // for cout

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
    //std::vector<uint8_t> image = frame.getImage();
    int pixelCount = image.size()/sizeof(uint8_t);

    // use this to scale result to be be within allowed shutter time boundaries
    scalingFactor = (config.maxShutterTime - config.minShutterTime)/(float)pixelCount;

    int maxNumTooBright = (int)(pixelCount * config.factNumTooBright);
    int maxNumTooDark   = (int)(pixelCount * config.factNumTooDark);

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

    // debug only
    //if (mean > maxMean)
    //    std::cout << mean << " BIGGER THAN MAX MEAN " << (int)maxMean << std::endl;
    //if (mean < minMean)
    //    std::cout << mean << " SMALLER THAN MIN MEAN " << (int)minMean << std::endl;
    //if (numTooBright > maxNumTooBright)
    //    std::cout << numTooBright << " TOO MANY TOO BRIGHT " << maxNumTooBright << std::endl;
    //if (numTooDark > maxNumTooDark)
    //    std::cout << numTooDark << " TOO MANY TOO DARK " << maxNumTooDark << std::endl;

    // decide whether image is too bright or dark
    if (mean > maxMean && numTooBright > maxNumTooBright)
    {
        // image is too bright, return positive value
        //std::cout << "Too bright: " << shutterTime << std::endl;
        return numTooBright;
    }
    else if (mean < minMean && numTooDark > maxNumTooDark)
    {
        // image is too dark, return negative value
        //std::cout << "Too dark: " << shutterTime << std::endl;
        return -numTooDark;
    }
    // image is within bounds
    return 0;
}

// TODO put some more thought/experiments into this
int ShutterController::calcNewShutterTime(const int analysisResult)
{
    shutterTime = 0.9*shutterTime + 0.1*(shutterTime - (scalingFactor * analysisResult));
    shutterTime = std::max(config.minShutterTime, shutterTime);
    shutterTime = std::min(shutterTime, config.maxShutterTime);
    return shutterTime;
}

}
