/**
  * @file base64_test.cpp
  * @author Artiom N.(cl)2017
  * @brief BASE64 encoder/decoder tests.
  *
  */

#include "tests_common.h"

#include <algorithm>

#include <tsw/base64.h>
#include <fstream>


static const tsw::String short_string = "ss";
static const tsw::String long_string = "looooooooooooooooooooo nggggggggggggggg GGGGGGGGGGGG data";

static const tsw::String base64_short_string = "c3M=";
static const tsw::String base64_long_string = "bG9vb29vb29vb29vb29vb29vb29vbyBuZ2dnZ2dnZ2dnZ2dnZ2dnIEdHR0dHR0dHR0dHRyBkYXRh";
static const tsw::String base64_long_string_lb = "bG9vb29vb29vb29vb29vb29vb29vbyBuZ2dnZ2dnZ2dnZ2dnZ2dnIEdHR0dHR0dHR0dHRyBk\nYXRh";

static const tsw::String base64_long_no_lb = "b29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vbw==";

static const tsw::BinData::value_type not_base64[] =
{
    0x14, 0x13, 0x12, 0x11, 0x10, 0x09, 0x65, 0x73,
    0x74, 0x00, 0x05, 0x00, 0x00, 0x00, 0x74, 0x65,
    0x73, 0x74, 0x00, 0x00
};

static const tsw::BinData::value_type base64_from_prev[] =
{
    // Prefix:
    0x42, 0x41, 0x53, 0x45, 0x36, 0x34, 0x3A, 0x0A,
    // Body:
    0x46, 0x42, 0x4D, 0x53, 0x45, 0x52, 0x41, 0x4A,
    0x5A, 0x58, 0x4E, 0x30, 0x41, 0x41, 0x55, 0x41,
    0x41, 0x41, 0x42, 0x30, 0x5A, 0x58, 0x4E, 0x30,
    0x41, 0x41, 0x41, 0x3D
};

static const tsw::String base64_as_string = "FBMSERAJZXN0AAUAAAB0ZXN0AAA=";

static const tsw::BinData::value_type very_long_data[] =
{
    0xD0, 0x9F, 0xD1, 0x83, 0xD1, 0x81, 0xD1, 0x82,
    0xD1, 0x8C, 0x20, 0xD1, 0x8F, 0xD1, 0x80, 0xD0,
    0xBE, 0xD1, 0x81, 0xD1, 0x82, 0xD1, 0x8C, 0x20,
    0xD0, 0xB1, 0xD0, 0xBB, 0xD0, 0xB0, 0xD0, 0xB3,
    0xD0, 0xBE, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB4,
    0xD0, 0xBD, 0xD0, 0xB0, 0xD1, 0x8F, 0x0A, 0xD0,
    0x92, 0xD1, 0x81, 0xD0, 0xBA, 0xD0, 0xB8, 0xD0,
    0xBF, 0xD0, 0xB0, 0xD0, 0xB5, 0xD1, 0x82, 0x2C,
    0x20, 0xD0, 0xBA, 0xD0, 0xB0, 0xD0, 0xBA, 0x20,
    0xD0, 0xB2, 0xD0, 0xBE, 0xD0, 0xBB, 0xD0, 0xBD,
    0xD0, 0xB0, 0x2C, 0x20, 0xE2, 0x80, 0x94, 0x20,
    0x0A, 0xD0, 0x98, 0xD0, 0xB4, 0xD1, 0x91, 0xD1,
    0x82, 0x20, 0xD0, 0xB2, 0xD0, 0xBE, 0xD0, 0xB9,
    0xD0, 0xBD, 0xD0, 0xB0, 0x20, 0xD0, 0xBD, 0xD0,
    0xB0, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB4, 0xD0,
    0xBD, 0xD0, 0xB0, 0xD1, 0x8F, 0x2C, 0x0A, 0xD0,
    0xA1, 0xD0, 0xB2, 0xD1, 0x8F, 0xD1, 0x89, 0xD0,
    0xB5, 0xD0, 0xBD, 0xD0, 0xBD, 0xD0, 0xB0, 0xD1,
    0x8F, 0x20, 0xD0, 0xB2, 0xD0, 0xBE, 0xD0, 0xB9,
    0xD0, 0xBD, 0xD0, 0xB0, 0x21, 0x0A, 0x0A, 0xD0,
    0x9A, 0xD0, 0xB0, 0xD0, 0xBA, 0x20, 0xD0, 0xB4,
    0xD0, 0xB2, 0xD0, 0xB0, 0x20, 0xD1, 0x80, 0xD0,
    0xB0, 0xD0, 0xB7, 0xD0, 0xBB, 0xD0, 0xB8, 0xD1,
    0x87, 0xD0, 0xBD, 0xD1, 0x8B, 0xD1, 0x85, 0x20,
    0xD0, 0xBF, 0xD0, 0xBE, 0xD0, 0xBB, 0xD1, 0x8E,
    0xD1, 0x81, 0xD0, 0xB0, 0x2C, 0x0A, 0xD0, 0x92,
    0xD0, 0xBE, 0x20, 0xD0, 0xB2, 0xD1, 0x81, 0xD1,
    0x91, 0xD0, 0xBC, 0x20, 0xD0, 0xB2, 0xD1, 0x80,
    0xD0, 0xB0, 0xD0, 0xB6, 0xD0, 0xB4, 0xD0, 0xB5,
    0xD0, 0xB1, 0xD0, 0xBD, 0xD1, 0x8B, 0x20, 0xD0,
    0xBC, 0xD1, 0x8B, 0x2E, 0x0A, 0xD0, 0x97, 0xD0,
    0xB0, 0x20, 0xD1, 0x81, 0xD0, 0xB2, 0xD0, 0xB5,
    0xD1, 0x82, 0x20, 0xD0, 0xB8, 0x20, 0xD0, 0xBC,
    0xD0, 0xB8, 0xD1, 0x80, 0x20, 0xD0, 0xBC, 0xD1,
    0x8B, 0x20, 0xD0, 0xB1, 0xD0, 0xBE, 0xD1, 0x80,
    0xD0, 0xB5, 0xD0, 0xBC, 0xD1, 0x81, 0xD1, 0x8F,
    0x2C, 0x0A, 0xD0, 0x9E, 0xD0, 0xBD, 0xD0, 0xB8,
    0x20, 0xE2, 0x80, 0x94, 0x20, 0xD0, 0xB7, 0xD0,
    0xB0, 0x20, 0xD1, 0x86, 0xD0, 0xB0, 0xD1, 0x80,
    0xD1, 0x81, 0xD1, 0x82, 0xD0, 0xB2, 0xD0, 0xBE,
    0x20, 0xD1, 0x82, 0xD1, 0x8C, 0xD0, 0xBC, 0xD1,
    0x8B, 0x2E, 0x0A, 0x0A
};

static const tsw::String base64_very_long_data =
    "0J/Rg9GB0YLRjCDRj9GA0L7RgdGC0Ywg0LHQu9Cw0LPQvtGA0L7QtNC90LDRjwrQktGB0LrQ\n"
    "uNC/0LDQtdGCLCDQutCw0Log0LLQvtC70L3QsCwg4oCUIArQmNC00ZHRgiDQstC+0LnQvdCw\n"
    "INC90LDRgNC+0LTQvdCw0Y8sCtCh0LLRj9GJ0LXQvdC90LDRjyDQstC+0LnQvdCwIQoK0JrQ\n"
    "sNC6INC00LLQsCDRgNCw0LfQu9C40YfQvdGL0YUg0L/QvtC70Y7RgdCwLArQktC+INCy0YHR\n"
    "kdC8INCy0YDQsNC20LTQtdCx0L3RiyDQvNGLLgrQl9CwINGB0LLQtdGCINC4INC80LjRgCDQ\n"
    "vNGLINCx0L7RgNC10LzRgdGPLArQntC90Lgg4oCUINC30LAg0YbQsNGA0YHRgtCy0L4g0YLR\n"
    "jNC80YsuCgo=";

static const tsw::String b64_prefix(reinterpret_cast<const char*>(tsw::base64_prefix),
                                    reinterpret_cast<const char*>(tsw::base64_prefix) + sizeof(tsw::base64_prefix));


TEST(Base64, Test)
{
    EXPECT_FALSE(tsw::is_base64(not_base64, boost::size(not_base64)));
    EXPECT_FALSE(tsw::is_base64(tsw::BinData(not_base64, not_base64 + boost::size(not_base64))));
    EXPECT_FALSE(tsw::is_base64(short_string));
    EXPECT_FALSE(tsw::is_base64(long_string));
    EXPECT_FALSE(tsw::is_base64(b64_prefix + short_string));
    EXPECT_FALSE(tsw::is_base64(b64_prefix + long_string));
    EXPECT_FALSE(tsw::is_base64(base64_as_string));
    EXPECT_FALSE(tsw::is_base64(very_long_data, boost::size(very_long_data)));
    EXPECT_FALSE(tsw::is_base64(b64_prefix + b64_prefix + base64_very_long_data));

    EXPECT_TRUE(tsw::is_base64(b64_prefix));

    EXPECT_TRUE(tsw::is_base64(base64_from_prev, boost::size(base64_from_prev)));
    EXPECT_TRUE(tsw::is_base64(tsw::BinData(base64_from_prev, base64_from_prev + boost::size(base64_from_prev))));
    EXPECT_TRUE(tsw::is_base64(b64_prefix + base64_short_string));
    EXPECT_TRUE(tsw::is_base64(b64_prefix + base64_long_string));
    EXPECT_TRUE(tsw::is_base64(b64_prefix + base64_as_string));
    EXPECT_TRUE(tsw::is_base64(b64_prefix + base64_as_string));
    EXPECT_TRUE(tsw::is_base64(b64_prefix + base64_very_long_data));
}


TEST(Base64, Encode)
{
    EXPECT_FALSE(ArraysMatch(reinterpret_cast<const char*>(base64_from_prev),
                             tsw::to_base64_no_lb(base64_from_prev, boost::size(base64_from_prev)).c_str(),
                             boost::size(base64_from_prev)));
    EXPECT_FALSE(ArraysMatch(reinterpret_cast<const char*>(base64_from_prev),
                             tsw::to_base64(base64_from_prev, boost::size(base64_from_prev)).c_str(),
                             boost::size(base64_from_prev)));
    EXPECT_TRUE(ArraysMatch(reinterpret_cast<const char*>(base64_from_prev),
                            tsw::to_base64_no_lb(not_base64, boost::size(not_base64)).c_str(),
                            boost::size(base64_from_prev)));
    EXPECT_TRUE(ArraysMatch(reinterpret_cast<const char*>(base64_from_prev),
                            tsw::to_base64(not_base64, boost::size(not_base64)).c_str(),
                            boost::size(base64_from_prev)));
    EXPECT_FALSE(ArraysMatch(base64_short_string.c_str(),
                             tsw::to_base64_no_lb(short_string).c_str(),
                             base64_short_string.size()));
    EXPECT_FALSE(ArraysMatch(base64_short_string.c_str(),
                             tsw::to_base64(short_string).c_str(),
                             base64_short_string.size()));
    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_short_string).c_str(),
                            tsw::to_base64_no_lb(short_string).c_str(),
                            (b64_prefix + base64_short_string).size()));
    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_short_string).c_str(),
                            tsw::to_base64(short_string).c_str(),
                            (b64_prefix + base64_short_string).size()));
    EXPECT_FALSE(ArraysMatch((b64_prefix + long_string).c_str(),
                            tsw::to_base64_no_lb(tsw::BinData(base64_long_string.c_str(),
                                                        base64_long_string.c_str() + base64_long_string.size())).c_str(),
                            (b64_prefix + long_string).size()));
    EXPECT_FALSE(ArraysMatch((b64_prefix + long_string).c_str(),
                            tsw::to_base64(tsw::BinData(base64_long_string.c_str(),
                                                        base64_long_string.c_str() + base64_long_string.size())).c_str(),
                            (b64_prefix + long_string).size()));
    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_long_string).c_str(),
                            tsw::to_base64_no_lb(tsw::BinData(long_string.c_str(),
                                                        long_string.c_str() + long_string.size())).c_str(),
                            (b64_prefix + base64_long_string).size()));

    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_long_string).c_str(),
                            tsw::to_base64_no_lb(tsw::BinData(long_string.c_str(),
                                                              long_string.c_str() + long_string.size())).c_str(),
                            (b64_prefix + base64_long_string).size()));
    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_long_string_lb).c_str(),
                            tsw::to_base64(tsw::BinData(long_string.c_str(),
                                                        long_string.c_str() + long_string.size())).c_str(),
                            (b64_prefix + base64_long_string_lb).size()));
    EXPECT_FALSE(ArraysMatch(base64_very_long_data.c_str(),
                             tsw::to_base64(very_long_data, boost::size(very_long_data)).c_str(),
                             base64_very_long_data.size()));
    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_very_long_data).c_str(),
                            tsw::to_base64(very_long_data, boost::size(very_long_data)).c_str(),
                            (b64_prefix + base64_very_long_data).size()));
    // Special case: two line breaks.
    EXPECT_TRUE(ArraysMatch((b64_prefix + base64_long_no_lb).c_str(),
                            tsw::to_base64_no_lb(tsw::BinData(100, 'o')).c_str(),
                            (b64_prefix + base64_long_no_lb).size()));
/*    tsw::String nolb_long_data64;
    std::remove_copy(base64_very_long_data.begin(), base64_very_long_data.end(), std::back_inserter(nolb_long_data64), '\n');
    EXPECT_TRUE(ArraysMatch((b64_prefix + nolb_long_data64).c_str(),
                            tsw::to_base64_no_lb(very_long_data, boost::size(very_long_data)).c_str(),
                            (b64_prefix + nolb_long_data64).size()));*/
}


TEST(Base64, Decode)
{
    EXPECT_THROW(tsw::from_base64(not_base64, boost::size(not_base64)), std::exception);
    EXPECT_TRUE(ArraysMatch(not_base64,
                            tsw::from_base64(base64_from_prev, boost::size(base64_from_prev)).data(),
                            boost::size(not_base64)));
    EXPECT_FALSE(ArraysMatch(base64_short_string.c_str(),
                             reinterpret_cast<const char*>(tsw::from_base64(short_string).data()),
                             base64_short_string.size()));
    EXPECT_TRUE(ArraysMatch(short_string.c_str(),
                            reinterpret_cast<const char*>(tsw::from_base64(base64_short_string).data()),
                            short_string.size()));
    EXPECT_TRUE(ArraysMatch(short_string.c_str(),
                            reinterpret_cast<const char*>(tsw::from_base64(b64_prefix + base64_short_string).data()),
                            short_string.size()));
    EXPECT_FALSE(ArraysMatch(long_string.c_str(),
                            reinterpret_cast<const char*>(tsw::from_base64(long_string.c_str(), long_string.size()).data()),
                            long_string.size()));
    EXPECT_TRUE(ArraysMatch(long_string.c_str(),
                            reinterpret_cast<const char*>(tsw::from_base64(base64_long_string.c_str(), base64_long_string.size()).data()),
                            long_string.size()));
    EXPECT_TRUE(ArraysMatch(long_string.c_str(),
                            reinterpret_cast<const char*>(tsw::from_base64(base64_long_string.c_str(), base64_long_string.size()).data()),
                            long_string.size()));
    EXPECT_TRUE(ArraysMatch(very_long_data,
                            tsw::from_base64(b64_prefix + base64_very_long_data).data(),
                            boost::size(very_long_data)));
    EXPECT_TRUE(ArraysMatch(very_long_data,
                            tsw::from_base64(base64_very_long_data).data(),
                            boost::size(very_long_data)));
}
