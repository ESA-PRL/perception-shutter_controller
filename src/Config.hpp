#ifndef _SHUTTER_CONTROLLER_CONFIG_HPP_
#define _SHUTTER_CONTROLLER_CONFIG_HPP_

namespace shutter_controller
{
    struct Config
    {
        int initShutterTime;
        Config()
            : initShutterTime(300)
        {
        }
    };
}

#endif //_SHUTTER_CONTROLLER_CONFIG_HPP_
