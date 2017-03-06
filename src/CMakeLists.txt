rock_library(shutter_controller
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(shutter_controller_bin Main.cpp
    DEPS shutter_controller)

