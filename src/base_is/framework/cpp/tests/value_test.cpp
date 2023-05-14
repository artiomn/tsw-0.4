/**
  * @file value_test.cpp
  * @author Artiom N.(cl)2017
  * @brief Value class tests.
  *
  */

#include <chrono>
#include <cmath>

#include "tests_common.h"


TEST(Value, EmptyValue)
{
    EXPECT_TRUE(tsw::Value().is_undefined());
    EXPECT_EQ(tsw::Value().as<tsw::Undefined>(), tsw::Value().as<tsw::Undefined>());
    EXPECT_THROW(tsw::Value(tsw::Null()).as<tsw::Undefined>(), std::bad_variant_access);
    EXPECT_THROW(tsw::Value().as<tsw::String>(), std::bad_variant_access);
    EXPECT_EQ(tsw::Value().value_type(), tsw::ValueType::Undefined);
}


TEST(Value, NullValue)
{
    EXPECT_FALSE(tsw::Value(tsw::Null()).is_undefined());
    EXPECT_TRUE(tsw::Value(tsw::Null()).is_null());
    EXPECT_EQ(tsw::Value(tsw::Null()).value_type(), tsw::ValueType::Null);
}


TEST(Value, ArrayValue)
{
    const int arr_size = 5;

    tsw::ValueArray a(arr_size, tsw::Value("a"));
    tsw::Value v(std::move(a));

    EXPECT_TRUE(v.is_array());
    EXPECT_EQ(v.as<tsw::ValueArray>().size(), arr_size);

    for (auto i: v.as_array())
    {
        EXPECT_EQ(i.as<tsw::String>(), "a");
        EXPECT_NE(i.as<tsw::String>(), "b");
    }

    EXPECT_EQ(v.value_type(), tsw::ValueType::Array);
}


TEST(Value, BinDataValue)
{
    const int arr_size = 5;

    tsw::BinData a(arr_size, 0x14);
    tsw::BinData b(arr_size, 0x88);

    tsw::Value v1(std::move(a));
    tsw::Value v2(b);

    EXPECT_TRUE(v1.is_bindata());
    EXPECT_TRUE(v2.is_bindata());
    EXPECT_EQ(v1.as<tsw::BinData>().size(), arr_size);
    EXPECT_EQ(v2.as_bindata().size(), arr_size);

    EXPECT_NE(v1.as_bindata(), v2.as<tsw::BinData>());

    for (auto i: v1.as_bindata())
    {
        EXPECT_EQ(i, 0x14);
        EXPECT_NE(i, 0x88);
    }

    for (auto i: v2.as_bindata())
    {
        EXPECT_NE(i, 0x14);
        EXPECT_EQ(static_cast<unsigned char>(i), 0x88);
    }

    EXPECT_EQ(v1.value_type(), tsw::ValueType::BinData);
    EXPECT_NE(v2.value_type(), tsw::ValueType::Array);
}


TEST(Value, BoolValue)
{
    tsw::Value b1(true);
    tsw::Value b2 = false;

    EXPECT_TRUE(b1.is_bool());
    EXPECT_NE(b1.as_bool(), b2.as_bool());
    EXPECT_EQ(b1.as_bool(), true);
    EXPECT_NE(b2.as_bool(), true);

    b1 = b2;
    b2 = true;

    EXPECT_EQ(b1.as_bool(), false);
    EXPECT_NE(b2.as_bool(), false);

    EXPECT_EQ(b1.value_type(), tsw::ValueType::Boolean);
    EXPECT_NE(b2.value_type(), tsw::ValueType::Int64Number);
}


TEST(Value, DoubleValue)
{
    tsw::Value v1(M_PI);

    // todo: This is not compiling.
    // tsw::Value v2 = M_PIl;
    EXPECT_TRUE(v1.is_double());
    EXPECT_DOUBLE_EQ(v1.as_double(), M_PI);
    EXPECT_DOUBLE_EQ(2 / sqrt(v1.as<double>()), M_2_SQRTPIl);
    EXPECT_EQ(v1.value_type(), tsw::ValueType::DoubleNumber);
}


TEST(Value, Int64_tValue)
{
    tsw::Value v1(123L);

    EXPECT_TRUE(v1.is_int64());
    EXPECT_EQ(v1.as_int64(), 123L);
    EXPECT_EQ(v1, tsw::Value(123L));
    EXPECT_LT(v1, tsw::Value(1232L));
    EXPECT_EQ(v1.value_type(), tsw::ValueType::Int64Number);
}


TEST(Value, StringValue)
{
    tsw::Value v1 = "test";
    tsw::Value v2 = "set";

    EXPECT_TRUE(v1.is_string());
    EXPECT_EQ(v1.as_string(), "test");
    EXPECT_NE(v1.as_string(), "tset");

    EXPECT_NE(v1.as<tsw::String>(), v2.as_string());

    v2 = "test";

    EXPECT_EQ(v1.as_string(), v2.as<tsw::String>());

    EXPECT_EQ(v1.value_type(), tsw::ValueType::String);
    EXPECT_NE(v2.value_type(), tsw::ValueType::BinData);
}


TEST(Value, TimeValue)
{
    tsw::Value v1;

    EXPECT_EQ(v1.value_type(), tsw::ValueType::Undefined);
    v1 = tsw::Time(std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(125)));
    EXPECT_EQ(tsw::Time(std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(125))), v1.as_time());
    EXPECT_LT(tsw::Time(std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(123))), v1.as_time());
    EXPECT_TRUE(v1.is_time());
    EXPECT_EQ(v1.value_type(), tsw::ValueType::Time);
}


TEST(Value, ObjectValue)
{
    tsw::Value v1 = tsw::Object();

    EXPECT_NO_THROW(v1.as<tsw::Object>()["test"] = tsw::Value("1"));

    EXPECT_TRUE(v1.is_object());
    EXPECT_EQ(v1.as_object()["test"].as_string(), "1");
    EXPECT_EQ(v1.value_type(), tsw::ValueType::Object);
    EXPECT_NE(v1.value_type(), tsw::ValueType::String);
}
