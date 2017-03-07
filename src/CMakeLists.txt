rock_library(shutter_controller
    SOURCES ShutterController.cpp
    HEADERS ShutterController.hpp Config.hpp
    DEPS_PKGCONFIG base-types)

#rock_executable(shutter_controller_bin Main.cpp
#    DEPS shutter_controller)
