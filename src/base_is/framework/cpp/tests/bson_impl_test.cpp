/**
  * @file bson_impl_test.cpp
  * @author Artiom N.(cl)2017
  * @brief BSON serializer/deserializer implementation tests.
  *
  */

#include <chrono>
#include <ctime>
#include <fstream>

#include <impl/bson_impl.h>

#include "tests_common.h"


// Data from https://json-bson-converter.appspot.com/

// "test": "test"
static const tsw::BinData::value_type bson_test_test[] =
{
    0x14, 0x00, 0x00, 0x00, 0x02, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x05, 0x00, 0x00, 0x00, 0x74, 0x65,
    0x73, 0x74, 0x00, 0x00
};

// "test": 777
static const tsw::BinData::value_type bson_test_777[] =
{
    0x0F, 0x00, 0x00, 0x00, 0x10, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x09, 0x03, 0x00, 0x00, 0x00
};

// "test": -1
static const tsw::BinData::value_type bson_test_m1[] =
{
    0x0F, 0x00, 0x00, 0x00, 0x10, 0x74, 0x65, 0x73,
    0x74, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

// "test": 0.777
static const tsw::BinData::value_type bson_test_0_777[] =
{
    0x13, 0x00, 0x00, 0x00, 0x01, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x77, 0xBE, 0x9F, 0x1A, 0x2F, 0xDD,
    0xE8, 0x3F, 0x00
};

// "test": true
static const tsw::BinData::value_type bson_test_true[] =
{
    0x0C, 0x00, 0x00, 0x00, 0x08, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x01, 0x00
};

// "test": false
static const tsw::BinData::value_type bson_test_false[] =
{
    0x0C, 0x00, 0x00, 0x00, 0x08, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x00, 0x00
};

// "test": { "a": "b", "b": -100000000000 }
static const tsw::BinData::value_type bson_test_a_b_b_m1[] =
{
    0x24, 0x00, 0x00, 0x00, 0x03, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x61,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x62, 0x00, 0x12,
    0x62, 0x00, 0x00, 0x18, 0x89, 0xB7, 0xE8, 0xFF,
    0xFF, 0xFF, 0x00, 0x00
};

// "test": null
static const tsw::BinData::value_type bson_test_null[] =
{
    0x0B, 0x00, 0x00, 0x00, 0x0A, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x00
};

// Tested, using bson_tools.
// "test": undefined
static const tsw::BinData::value_type bson_test_undefined[] =
{
    0x0B, 0x00, 0x00, 0x00, 0x06, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x00
};


// Tested, using bson_tools.
// "test": 0xde 0xad 0xbe 0xef
static const tsw::BinData::value_type bson_test_bindata[] =
{
    0x14, 0x00, 0x00, 0x00, 0x05, 0x74, 0x65, 0x73,
    0x74, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xDE,
    0xAD, 0xBE, 0xEF, 0x00
};


// Tested, using bson_tools.
// "time": 30.09.2017 14:25
static const tsw::BinData::value_type bson_time_time[] =
{
    0x13, 0x00, 0x00, 0x00, 0x09, 0x74, 0x69, 0x6D,
    0x65, 0x00, 0x00, 0x58, 0xA1, 0x7D, 0xCA, 0x29,
    0xE9, 0x14, 0x00
};


// Tested, using bson_tools.
// "test_arr": ["aaa", 123, true],
// "test_list": [123, tsw::String("bbb"), true]
static const tsw::BinData::value_type bson_test_containers[] =
{
    0x5B, 0x00, 0x00, 0x00, 0x04, 0x74, 0x65, 0x73,
    0x74, 0x5F, 0x61, 0x72, 0x72, 0x00, 0x1B, 0x00,
    0x00, 0x00, 0x02, 0x30, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x61, 0x61, 0x61, 0x00, 0x10, 0x31, 0x00,
    0x7B, 0x00, 0x00, 0x00, 0x08, 0x32, 0x00, 0x01,
    0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x5F, 0x6C,
    0x69, 0x73, 0x74, 0x00, 0x26, 0x00, 0x00, 0x00,
    0x10, 0x30, 0x00, 0x7B, 0x00, 0x00, 0x00, 0x02,
    0x31, 0x00, 0x04, 0x00, 0x00, 0x00, 0x62, 0x62,
    0x62, 0x00, 0x08, 0x32, 0x00, 0x01, 0x12, 0x33,
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x00
};


// Tested, using bson_tools.
// {
//   test1: [ "testxxx", "sss", "FFFFFFFF" ],
//   test2: [ "testxxx", "sss", "FFFFFFFF" ],
//   binda: BinData(0, 6F6F6F6F6F6F6F6F6F x 100),
//   obj:
//   {
//     aname: "val_a",
//     bname: "b_val",
//     cname: "val_c1"
//   },
//   undefined: undefined,
//   undef1: undefined,
//   nullfield: null,
//   objarr: [ { v1: "testxxx", v2: [ "x", "y", "z", { a: "b" }, [ "i", "j", "k" ] ], v3: "FFFFFFFF" } ]
// }
static const tsw::BinData::value_type bson_test_complex[] =
{
    0xCB, 0x01, 0x00, 0x00, 0x04, 0x74, 0x65, 0x73,
    0x74, 0x31, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x02,
    0x30, 0x00, 0x08, 0x00, 0x00, 0x00, 0x74, 0x65,
    0x73, 0x74, 0x78, 0x78, 0x78, 0x00, 0x02, 0x31,
    0x00, 0x04, 0x00, 0x00, 0x00, 0x73, 0x73, 0x73,
    0x00, 0x02, 0x32, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46,
    0x00, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x32,
    0x00, 0x2F, 0x00, 0x00, 0x00, 0x02, 0x30, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74,
    0x78, 0x78, 0x78, 0x00, 0x02, 0x31, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x73, 0x73, 0x73, 0x00, 0x02,
    0x32, 0x00, 0x09, 0x00, 0x00, 0x00, 0x46, 0x46,
    0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x00, 0x00,
    0x05, 0x62, 0x69, 0x6E, 0x64, 0x61, 0x00, 0x64,
    0x00, 0x00, 0x00, 0x00, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F,
    0x03, 0x6F, 0x62, 0x6A, 0x00, 0x39, 0x00, 0x00,
    0x00, 0x02, 0x61, 0x6E, 0x61, 0x6D, 0x65, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x76, 0x61, 0x6C, 0x5F,
    0x61, 0x00, 0x02, 0x62, 0x6E, 0x61, 0x6D, 0x65,
    0x00, 0x06, 0x00, 0x00, 0x00, 0x62, 0x5F, 0x76,
    0x61, 0x6C, 0x00, 0x02, 0x63, 0x6E, 0x61, 0x6D,
    0x65, 0x00, 0x07, 0x00, 0x00, 0x00, 0x76, 0x61,
    0x6C, 0x5F, 0x63, 0x31, 0x00, 0x00, 0x06, 0x75,
    0x6E, 0x64, 0x65, 0x66, 0x69, 0x6E, 0x65, 0x64,
    0x00, 0x06, 0x75, 0x6E, 0x64, 0x65, 0x66, 0x31,
    0x00, 0x0A, 0x6E, 0x75, 0x6C, 0x6C, 0x66, 0x69,
    0x65, 0x6C, 0x64, 0x00, 0x04, 0x6F, 0x62, 0x6A,
    0x61, 0x72, 0x72, 0x00, 0x86, 0x00, 0x00, 0x00,
    0x03, 0x30, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x02,
    0x76, 0x31, 0x00, 0x08, 0x00, 0x00, 0x00, 0x74,
    0x65, 0x73, 0x74, 0x78, 0x78, 0x78, 0x00, 0x04,
    0x76, 0x32, 0x00, 0x54, 0x00, 0x00, 0x00, 0x02,
    0x30, 0x00, 0x02, 0x00, 0x00, 0x00, 0x78, 0x00,
    0x02, 0x31, 0x00, 0x02, 0x00, 0x00, 0x00, 0x79,
    0x00, 0x02, 0x32, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x7A, 0x00, 0x03, 0x33, 0x00, 0x0E, 0x00, 0x00,
    0x00, 0x02, 0x61, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x62, 0x00, 0x00, 0x04, 0x34, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x02, 0x30, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x69, 0x00, 0x02, 0x31, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x6A, 0x00, 0x02, 0x32, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x02,
    0x76, 0x33, 0x00, 0x09, 0x00, 0x00, 0x00, 0x46,
    0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x46, 0x00,
    0x00, 0x00, 0x00
};


TEST(BsonImpl, ValueString)
{
    tsw::impl::BsonSerializerImpl bson_simpl;

    EXPECT_NO_THROW(bson_simpl.append_field("test", "test"));
    EXPECT_TRUE(ArraysMatch(bson_test_test, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_NO_THROW(bson_simpl.append_field("test", tsw::String("test")));
    EXPECT_TRUE(ArraysMatch(bson_test_test, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("tESt"), "test"));
    EXPECT_FALSE(ArraysMatch(bson_test_test, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("tESt"), tsw::String("test")));
    EXPECT_FALSE(ArraysMatch(bson_test_test, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    EXPECT_TRUE(v.as_object()["test"].is_undefined());
    ASSERT_TRUE(v.as_object()["tESt"].is_string());
    EXPECT_EQ(v.as_object()["tESt"].as_string(), "test");
}


TEST(BsonImpl, ValueInt)
{
    tsw::impl::BsonSerializerImpl bson_simpl;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), 777));
    EXPECT_TRUE(ArraysMatch(bson_test_777, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_test, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_int32());
    EXPECT_EQ(v.as_object()["test"].as_int32(), 777);
}


TEST(BsonImpl, ValueDouble)
{
    tsw::impl::BsonSerializerImpl bson_simpl;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), 0.777));
    EXPECT_TRUE(ArraysMatch(bson_test_0_777, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_777, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_double());
    EXPECT_DOUBLE_EQ(v.as_object()["test"].as_double(), 0.777);
}


TEST(BsonImpl, ValueBool)
{
    tsw::impl::BsonSerializerImpl bson_simpl;    
    tsw::impl::BsonDeserializerImpl bson_dsimpl;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), true));
    EXPECT_TRUE(ArraysMatch(bson_test_true, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_false, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_bool());
    EXPECT_EQ(v.as_object()["test"].as_bool(), true);

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), false));
    EXPECT_TRUE(ArraysMatch(bson_test_false, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_true, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    EXPECT_TRUE(v.as_object()["test"].is_bool());
    EXPECT_EQ(v.as_object()["test"].as_bool(), false);
}


TEST(BsonImpl, ValueNameValueMap)
{
    tsw::impl::BsonSerializerImpl bson_simpl;
    tsw::NameValueMap field_data =
    {
        std::make_pair("a", tsw::Value("b")),
        std::make_pair("b", tsw::Value(-100000000000))
    };

    tsw::NameValueMap field_data_incorrect =
    {
        std::make_pair("a", tsw::Value("b")),
        std::make_pair("b", tsw::Value(100000000000))
    };

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), field_data));
    EXPECT_TRUE(ArraysMatch(bson_test_a_b_b_m1, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), field_data_incorrect));
    EXPECT_FALSE(ArraysMatch(bson_test_a_b_b_m1, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_object());
    ASSERT_TRUE(v.as_object()["test"].as_object()["a"].is_string());
    EXPECT_EQ(v.as_object()["test"].as_object()["a"].as_string(), "b");
}


TEST(BsonImpl, ValueBinData)
{
    tsw::impl::BsonSerializerImpl bson_simpl;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), tsw::BinData { 0xde, 0xad, 0xbe, 0xef }));
    EXPECT_TRUE(ArraysMatch(bson_test_bindata, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_bindata());
    ASSERT_EQ(v.as_object()["test"].as_bindata(), (tsw::BinData { 0xde, 0xad, 0xbe, 0xef }));
}

TEST(BsonImpl, ValueValue)
{
    tsw::impl::BsonSerializerImpl bson_simpl;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), tsw::Value(-1)));
    EXPECT_TRUE(ArraysMatch(bson_test_m1, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_777, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_int32());
    ASSERT_EQ(v.as_object()["test"].as_int32(), -1);
}


TEST(BsonImpl, ValueTime)
{
    tsw::impl::BsonSerializerImpl bson_simpl;
    auto t = std::chrono::high_resolution_clock::now().time_since_epoch();

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test_time"), t));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test_time"].is_time());
    ASSERT_EQ(v.as_object()["test_time"].as_time(), t);

    tm fixed_time;
    memset(&fixed_time, 0, sizeof(tm));
    // From 1900.
    fixed_time.tm_year = 117;
    // month - 1.
    fixed_time.tm_mon = 8;
    fixed_time.tm_mday = 30;
    fixed_time.tm_hour = 14;
    fixed_time.tm_min = 25;
    auto ft = timegm(&fixed_time);
    bson_simpl.clear();
    bson_simpl.append_field("time", tsw::Time(std::chrono::seconds(ft)));

    EXPECT_TRUE(ArraysMatch(bson_time_time, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["time"].is_time());
    EXPECT_EQ(v.as_object()["time"].as_time().count(), tsw::Time(std::chrono::seconds(ft)).count());
}


TEST(BsonImpl, ValueContainer)
{
    tsw::impl::BsonSerializerImpl bson_simpl;
    tsw::ValueArray a_data = {"aaa", 123, true};
    tsw::ValueArray l_data = {123, tsw::String("bbb"), true, -1L};

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test_arr"), a_data));
    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test_list"), l_data));

    EXPECT_TRUE(ArraysMatch(bson_test_containers, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());

    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test_list"].is_array());
    ASSERT_EQ(v.as_object()["test_list"].as_array().size(), l_data.size());
    ASSERT_TRUE(v.as_object()["test_list"].as_array()[l_data.size() - 1].is_int64());
    EXPECT_EQ(v.as_object()["test_list"].as_array()[l_data.size() - 1].as_int64(), l_data[l_data.size() - 1].as_int64());

    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test_arr"].is_array());
    ASSERT_TRUE(v.as_object()["test_arr"].as_array()[0].is_string());
    ASSERT_EQ(v.as_object()["test_arr"].as_array().size(), a_data.size());
    EXPECT_EQ(v.as_object()["test_arr"].as_array()[0].as_string(), a_data[0].as_string());
}


TEST(BsonImpl, ValueUID)
{
    tsw::impl::BsonSerializerImpl bson_simpl;
    tsw::UID u = 54321098l;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("+++"), static_cast<int64_t>(u)));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    tsw::Value v = bson_dsimpl.Deserialize(bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["+++"].is_int64());
    ASSERT_EQ(v.as_object()["+++"].as_int64(), u);
}


TEST(BsonImpl, ValueVoid)
{
    tsw::impl::BsonSerializerImpl bson_simpl;

    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), tsw::Null()));
    EXPECT_TRUE(ArraysMatch(bson_test_null, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_undefined, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_NO_THROW(bson_simpl.append_field(tsw::String("test"), tsw::Value()));
    EXPECT_TRUE(ArraysMatch(bson_test_undefined, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(bson_test_null, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));
}


TEST(BsonImpl, ComplexSerialization)
{
    tsw::impl::BsonSerializerImpl bson_simpl;
    tsw::Object ox{std::make_pair("aname", "val_a"), std::make_pair("bname", "b_val"), std::make_pair("cname", "val_c1")};

    EXPECT_NO_THROW(bson_simpl.append_field("test", "test"));
    EXPECT_TRUE(ArraysMatch(bson_test_test, bson_simpl.get_buffer().data(), bson_simpl.get_buffer().size()));

    EXPECT_NO_THROW(bson_simpl.append_field("test1", tsw::Value(tsw::ValueArray{"testxxx", "sss", "FFFFFFFF"})));
    //bson_simpl.append_field("test1", tsw::Value(tsw::ValueArray{"testxxx", "sss", "FFFFFFFF"}));
    EXPECT_NO_THROW(bson_simpl.append_field("test2", tsw::ValueArray{"testxxx", "sss", "FFFFFFFF"}));
    EXPECT_NO_THROW(bson_simpl.append_field("binda", tsw::Value(tsw::BinData(100, 'o'))));

    EXPECT_NO_THROW(bson_simpl.append_field("obj", tsw::Value(ox)));
    EXPECT_NO_THROW(bson_simpl.append_field("undefined", tsw::Value()));
    EXPECT_NO_THROW(bson_simpl.append_undefined_field("undef1"));
    EXPECT_NO_THROW(bson_simpl.append_field("nullfield", tsw::Value(tsw::Null())));
    EXPECT_NO_THROW(bson_simpl.append_field("objarr",
        tsw::ValueArray
        {
            tsw::Object
            {
                std::make_pair("v1", "testxxx"),
                std::make_pair("v2",
                    tsw::ValueArray
                    {
                        "x", "y", "z",
                        tsw::Object
                        {
                            std::make_pair("a", "b")
                        },
                        tsw::ValueArray{"i", "j", "k"}
                    }),
                std::make_pair("v3", "FFFFFFFF")
            }
        })
    );

    auto bd = bson_simpl.get_buffer();
    EXPECT_TRUE(ArraysMatch(bson_test_complex, bd.data(), bd.size()));

    tsw::impl::BsonDeserializerImpl bson_dsimpl;
    EXPECT_NO_THROW(bson_dsimpl.Deserialize(bd.data(), bd.size(), true));
    EXPECT_NO_THROW(bson_simpl.append_field("val", bson_dsimpl.Deserialize(bd.data(), bd.size())));
    bd = bson_simpl.get_buffer();
    EXPECT_FALSE(ArraysMatch(bson_test_complex, bd.data(), bd.size()));
}
