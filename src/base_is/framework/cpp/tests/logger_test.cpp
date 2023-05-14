/**
  * @file logger_test.cpp
  * @author Artiom N.(cl)2017
  * @brief Logger tests.
  *
  */

#include "tests_common.h"

#include <tsw/logger.h>


TEST(Logger, Log)
{
    LOG(tsw::LogLevel::Notice, "test");
    //tsw::Logger::get_logger("default").flush();
}
