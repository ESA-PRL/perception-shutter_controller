#include <boost/test/unit_test.hpp>
#include <shutter_controller/Dummy.hpp>

using namespace shutter_controller;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    shutter_controller::DummyClass dummy;
    dummy.welcome();
}
