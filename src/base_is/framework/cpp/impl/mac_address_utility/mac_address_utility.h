/**
  * @file mac_address_utility.h
  * @author Artiom N.(cl)2018
  * @brief MAC address getter class definition.
  *
  */

#ifndef _TSW_MACADDRESS_UTILITY_H
#define _TSW_MACADDRESS_UTILITY_H


namespace tsw
{

class MACAddressUtility
{
public:
    static long get_mac_address(unsigned char *result);
private:
#if defined(WIN32) || defined(UNDER_CE)
    static long get_mac_address_msw(unsigned char *result);
#elif defined(__APPLE__)
    static long get_mac_address_mac(unsigned char *result);
#elif defined(LINUX) || defined(linux)
    static long get_mac_address_linux(unsigned char *result);
#endif
};

} // namespace tsw

#endif // _TSW_MACADDRESS_UTILITY_H
