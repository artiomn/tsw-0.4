/**
  * @file json_impl_test.cpp
  * @author Artiom N.(cl)2017
  * @brief JSON serializer/deserializer implementation tests.
  *
  */

#include <chrono>
#include <fstream>

#include <boost/variant.hpp>

#include <impl/json_impl.h>

#include "tests_common.h"

static const tsw::BinData::value_type json_test_test[] =
R"_({
    "test": "test"
})_";


static const tsw::BinData::value_type json_test_777[] =
R"_({
    "test": 777
})_";


static const tsw::BinData::value_type json_test_0_777[] =
R"_({
    "test": 0.777
})_";


static const tsw::BinData::value_type json_test_true[] =
R"_({
    "test": true
})_";


static const tsw::BinData::value_type json_test_false[] =
R"_({
    "test": false
})_";


static const tsw::BinData::value_type json_test_m1[] =
R"_({
    "test": -1
})_";


static const tsw::BinData::value_type json_test_a_b_b_m1[] =
R"_({
    "test": {
        "a": "b",
        "b": -100000000000
    }
})_";


// 0xde 0xad 0xbe 0xef.
static const tsw::BinData::value_type json_test_bindata[] =
R"_({
    "test": "BASE64:\n3q2+7w=="
})_";


static const tsw::BinData::value_type json_time_time[] =
R"_({
    "time": "2017-10-01T18:52:23.0Z"
})_";


static const tsw::BinData::value_type json_test_containers[] =
R"_({
    "test_arr": [
        "aaa",
        123,
        true
    ],
    "test_list": [
        "bbb",
        123,
        true,
        -1
    ]
})_";


static const tsw::BinData::value_type json_test_complex[] =
R"_({
    "test1": [
        "testxxx",
        "sss",
        "FFFFFFFF"
    ],
    "test2": [
        "testxxx",
        "sss",
        "FFFFFFFF"
    ],
    "binda": "BASE64:\nb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vb29vbw==",
    "obj": {
        "aname": "val_a",
        "bname": "b_val",
        "cname": "val_c1"
    },
    "undefined": null,
    "undef1": null,
    "nullfield": null,
    "objarr": [
        {
            "v1": "testxxx",
            "v2": [
                "x",
                "y",
                "z",
                {
                    "a": "b"
                },
                [
                    "i",
                    "j",
                    "k"
                ]
            ],
            "v3": "FFFFFFFF"
        }
    ]
})_";


TEST(JsonImpl, ValueString)
{
    tsw::impl::JsonSerializerImpl json_simpl;

    EXPECT_NO_THROW(json_simpl.append_field("test", "test"));
    EXPECT_TRUE(ArraysMatch(json_test_test, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_NO_THROW(json_simpl.append_field("test", tsw::String("test")));
    EXPECT_TRUE(ArraysMatch(json_test_test, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("tESt"), "test"));
    EXPECT_FALSE(ArraysMatch(json_test_test, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("tESt"), tsw::String("test")));
    EXPECT_FALSE(ArraysMatch(json_test_test, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    EXPECT_TRUE(v.as_object()["test"].is_undefined());
    ASSERT_TRUE(v.as_object()["tESt"].is_string());
    EXPECT_EQ(v.as_object()["tESt"].as_string(), "test");
}


TEST(JsonImpl, ValueInt)
{
    tsw::impl::JsonSerializerImpl json_simpl;

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), 777));

    EXPECT_TRUE(ArraysMatch(json_test_777, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(json_test_test, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_int32());
    EXPECT_EQ(v.as_object()["test"].as_int32(), 777);
}


TEST(JsonImpl, ValueDouble)
{
    tsw::impl::JsonSerializerImpl json_simpl;

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), 0.777));
    EXPECT_TRUE(ArraysMatch(json_test_0_777, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(json_test_777, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_double());
    EXPECT_DOUBLE_EQ(v.as_object()["test"].as_double(), 0.777);
}


TEST(JsonImpl, ValueBool)
{
    tsw::impl::JsonSerializerImpl json_simpl;
    tsw::impl::JsonDeserializerImpl json_dsimpl;

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), true));
    EXPECT_TRUE(ArraysMatch(json_test_true, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(json_test_false, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_bool());
    EXPECT_EQ(v.as_object()["test"].as_bool(), true);

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), false));
    EXPECT_TRUE(ArraysMatch(json_test_false, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(json_test_true, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    EXPECT_TRUE(v.as_object()["test"].is_bool());
    EXPECT_EQ(v.as_object()["test"].as_bool(), false);
}


TEST(JsonImpl, ValueNameValueMap)
{
    tsw::impl::JsonSerializerImpl json_simpl;
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

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), field_data));
    EXPECT_TRUE(ArraysMatch(json_test_a_b_b_m1, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), field_data_incorrect));
    EXPECT_FALSE(ArraysMatch(json_test_a_b_b_m1, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_object());
    ASSERT_TRUE(v.as_object()["test"].as_object()["a"].is_string());
    EXPECT_EQ(v.as_object()["test"].as_object()["a"].as_string(), "b");
}


TEST(JsonImpl, ValueBinData)
{
    tsw::impl::JsonSerializerImpl json_simpl;

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), tsw::BinData { 0xde, 0xad, 0xbe, 0xef }));
    EXPECT_TRUE(ArraysMatch(json_test_bindata, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_bindata());
    ASSERT_EQ(v.as_object()["test"].as_bindata(), (tsw::BinData { 0xde, 0xad, 0xbe, 0xef }));
}

TEST(JsonImpl, ValueValue)
{
    tsw::impl::JsonSerializerImpl json_simpl;

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), tsw::Value(-1)));

    EXPECT_TRUE(ArraysMatch(json_test_m1, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_FALSE(ArraysMatch(json_test_777, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test"].is_int32());
    ASSERT_EQ(v.as_object()["test"].as_int32(), -1);
}


TEST(JsonImpl, ValueTime)
{
    tsw::impl::JsonSerializerImpl json_simpl;
    auto t = std::chrono::high_resolution_clock::now().time_since_epoch();

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test_time"), t));
    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test_time"].is_time());
    ASSERT_EQ(v.as_object()["test_time"].as_time(), t);

    tm fixed_time;
    memset(&fixed_time, 0, sizeof(tm));
    // From 1900.
    fixed_time.tm_year = 117;
    // month - 1.
    fixed_time.tm_mon = 9;
    fixed_time.tm_mday = 01;
    fixed_time.tm_hour = 18;
    fixed_time.tm_min = 52;
    fixed_time.tm_sec = 23;
    auto ft = timegm(&fixed_time);
    json_simpl.clear();
    json_simpl.append_field("time", tsw::Time(std::chrono::seconds(ft)));
    EXPECT_TRUE(ArraysMatch(json_time_time, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["time"].is_time());
    EXPECT_EQ(v.as_object()["time"].as_time().count(), tsw::Time(std::chrono::seconds(ft)).count());
}


TEST(JsonImpl, ValueContainer)
{
    tsw::impl::JsonSerializerImpl json_simpl;
    tsw::ValueArray a_data = {"aaa", 123, true};
    tsw::ValueArray l_data = {tsw::String("bbb"), 123, true, -1L};

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test_arr"), a_data));
    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test_list"), l_data));

    EXPECT_TRUE(ArraysMatch(json_test_containers, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());

    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test_list"].is_array());
    ASSERT_EQ(v.as_object()["test_list"].as_array().size(), l_data.size());

    // Both types: int32 and int64 are possible. JSON isn't very type-strict.
    ASSERT_TRUE(v.as_object()["test_list"].as_array()[l_data.size() - 1].is_int64() ||
                v.as_object()["test_list"].as_array()[l_data.size() - 1].is_int32());
    EXPECT_EQ(v.as_object()["test_list"].as_array()[l_data.size() - 1].as_int32(), l_data[l_data.size() - 1].as_int64());

    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["test_arr"].is_array());
    ASSERT_TRUE(v.as_object()["test_arr"].as_array()[0].is_string());
    ASSERT_EQ(v.as_object()["test_arr"].as_array().size(), a_data.size());
    EXPECT_EQ(v.as_object()["test_arr"].as_array()[0].as_string(), a_data[0].as_string());
}


TEST(JsonImpl, ValueUID)
{
    tsw::impl::JsonSerializerImpl json_simpl;
    tsw::UID u = 54321098000l;

    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("+++"), static_cast<int64_t>(u)));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    tsw::Value v = json_dsimpl.Deserialize(json_simpl.get_buffer().data(), json_simpl.get_buffer().size());
    ASSERT_TRUE(v.is_object());
    ASSERT_TRUE(v.as_object()["+++"].is_int64());
    ASSERT_EQ(v.as_object()["+++"].as_int64(), u);
}


TEST(JsonImpl, ValueVoid)
{
    tsw::impl::JsonSerializerImpl json_simpl;

//    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), tsw::Null()));
//    EXPECT_TRUE(ArraysMatch(Json_test_null, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
//    EXPECT_FALSE(ArraysMatch(Json_test_undefined, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
    EXPECT_NO_THROW(json_simpl.append_field(tsw::String("test"), tsw::Value()));
//    EXPECT_TRUE(ArraysMatch(Json_test_undefined, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
//    EXPECT_FALSE(ArraysMatch(Json_test_null, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));
}


TEST(JsonImpl, ComplexSerialization)
{
    tsw::impl::JsonSerializerImpl json_simpl;
    tsw::Object ox{std::make_pair("aname", "val_a"), std::make_pair("bname", "b_val"), std::make_pair("cname", "val_c1")};

    EXPECT_NO_THROW(json_simpl.append_field("test", "test"));
    EXPECT_TRUE(ArraysMatch(json_test_test, json_simpl.get_buffer().data(), json_simpl.get_buffer().size()));

    EXPECT_NO_THROW(json_simpl.append_field("test1", tsw::Value(tsw::ValueArray{"testxxx", "sss", "FFFFFFFF"})));
    //json_simpl.append_field("test1", tsw::Value(tsw::ValueArray{"testxxx", "sss", "FFFFFFFF"}));
    EXPECT_NO_THROW(json_simpl.append_field("test2", tsw::ValueArray{"testxxx", "sss", "FFFFFFFF"}));
    EXPECT_NO_THROW(json_simpl.append_field("binda", tsw::Value(tsw::BinData(100, 'o'))));

    EXPECT_NO_THROW(json_simpl.append_field("obj", tsw::Value(ox)));
    EXPECT_NO_THROW(json_simpl.append_field("undefined", tsw::Value()));
    EXPECT_NO_THROW(json_simpl.append_undefined_field("undef1"));
    EXPECT_NO_THROW(json_simpl.append_field("nullfield", tsw::Value(tsw::Null())));
    EXPECT_NO_THROW(json_simpl.append_field("objarr",
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

    auto bd = json_simpl.get_buffer();

    EXPECT_TRUE(ArraysMatch(json_test_complex, bd.data(), bd.size()));

    tsw::impl::JsonDeserializerImpl json_dsimpl;
    EXPECT_NO_THROW(json_dsimpl.Deserialize(bd.data(), bd.size(), true));
    EXPECT_NO_THROW(json_simpl.append_field("val", json_dsimpl.Deserialize(bd.data(), bd.size())));
    bd = json_simpl.get_buffer();
    EXPECT_FALSE(ArraysMatch(json_test_complex, bd.data(), bd.size()));
}
