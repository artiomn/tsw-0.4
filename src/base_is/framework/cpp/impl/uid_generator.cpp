/**
  * @file uid_generator.cpp
  * @author Artiom N.(cl)2018
  * @brief UIDGenerator class implementation.
  *
  */

#include <tsw/error.h>
#include <tsw/types.h>
#include <tsw/uid_generator.h>

#include "mac_address_utility/mac_address_utility.h"


namespace tsw
{

UID UIDGenerator::generate_uid()
{
    using namespace std::chrono;
    auto t = duration_cast<nanoseconds>(steady_clock::now().time_since_epoch());
    auto mac_hash = MACAddressUtility::get_mac_address(nullptr);

    if (mac_hash < 0) TSW_THROW(Exception, String("MACAddressUtility::get_mac_address returned ") +
                                std::to_string(mac_hash) +  "!");

    return t.count() + mac_hash;
}

} // namespace tsw
