/**
  * @file tester.cpp
  * @author Artiom N.(cl)2017
  * @brief Test runner executable entry point function.
  *
  */

#include <gtest/gtest.h>

#include <tsw/error.h>


int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  try
  {
    return RUN_ALL_TESTS();
  }
  catch (const boost::exception &e)
  {
      std::cerr << boost::diagnostic_information(e) << std::endl;
      throw;
  }
}
