#include "ShutterController.hpp"
#include <iostream> // for cout

namespace shutter_controller
{

ShutterController::ShutterController(const Config& config)
    : mConfig(config), shutterTime(config.initShutterTime)
{
    maxShutterTime = 3900;
    minShutterTime = 0;
}

//int ShutterController::analyze(base::samples::frame::Frame frame)
int ShutterController::analyze(std::vector<uint8_t> image)
{
    //std::vector<uint8_t> image = frame.getImage();
    int pixelCount = image.size()/sizeof(uint8_t);
    uint8_t maxIntensity = 255;

    // use this to scale result to be be within allowed shutter time boundaries
    scalingFactor = (maxShutterTime - minShutterTime)/(float)pixelCount;

    int maxNumTooBright    = (int)(pixelCount * 0.1);
    int maxNumTooDark      = (int)(pixelCount * 0.2);
    uint8_t threshMaxIntensity = (uint8_t)(maxIntensity * 0.65);
    uint8_t threshMinIntensity = (uint8_t)(maxIntensity * 0.05);

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
    if (mean > maxIntensity*0.3 && numTooBright > maxNumTooBright)
    {
        // image is too bright, return positive value
        std::cout << " TOO BRIGHT " << numTooBright << "/" << maxNumTooBright << "/" << pixelCount << std::endl;
        return numTooBright;
    }
    else if (mean < maxIntensity*0.4 && numTooDark > maxNumTooDark)
    {
        // image is too dark, return negative value
        std::cout << " TOO DARK " << numTooDark << "/" << maxNumTooDark << "/" << pixelCount << std::endl;
        return -numTooDark;
    }
    // image is within bounds
    return 0;
}

// TODO put some more thought/experiments into this
int ShutterController::calcNewShutterTime(const int analysisResult)
{
    shutterTime = shutterTime - (int)(analysisResult/2.0*scalingFactor);
    shutterTime = std::max(minShutterTime, shutterTime);
    shutterTime = std::min(shutterTime, maxShutterTime);
    return shutterTime;
}

}
