/**
  * @file module.cpp
  * @author Artiom N.(cl)2017
  * @brief Module class implementation.
  *
  */

#include <fstream>
#include <iostream>

#include <tsw/logger.h>
#include <tsw/module.h>
#include <tsw/json_deserializer.h>
#include <tsw/types.h>
#include <tsw/uid_generator.h>


namespace tsw
{

Module::Module(const UID& uid, const Path& path, const String& name, const String& version,
               const String& metadata_version,
               const StringArray& class_list, const StringArray& deps_list,
               const String& description, const String& author, const URL& update_url,
               const URL& homepage_url, const StringStringMap& contributors,
               const String& min_system_version, const String& license,
               bool local) :
   uid_(uid), path_(path), name_(name), version_(version), metadata_version_(metadata_version),
   class_list_(class_list), deps_list_(deps_list), description_(description), author_(author),
   update_url_(update_url), homepage_url_(homepage_url), contributors_(contributors),
   min_system_version_(min_system_version),
   license_(license), local_(local)
{
}


Module::Module(const Path& description_json) : local_(true)
{
    std::ifstream module_file;
    BinData data;

    module_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        LOG(LogLevel::Debug, "Opening module description file \"%s\"...", String(description_json).c_str() );
        module_file.open(description_json, std::ifstream::binary);

        BinData buf((std::istreambuf_iterator<std::ifstream::char_type>(module_file)),
                    (std::istreambuf_iterator<std::ifstream::char_type>()));

        data = std::move(buf);
        module_file.close();
    }
    catch (const std::ifstream::failure& e)
    {
        LOG(LogLevel::Warning, e.what());
        throw;
    }

    JsonDeserializer module_json;
    auto module_fields = module_json.DeserializeObject(data);

    path_ = description_json;
    uid_ = UIDGenerator::generate_uid();

    load_description(module_fields);
}


Module::Module(const BinData& json, const Path& module_path) : path_(module_path)
{
    JsonDeserializer module_json;
    auto module_fields = module_json.DeserializeObject(json);

    uid_ = UIDGenerator::generate_uid();

    load_description(module_fields);
}


Module::~Module()
{

}


const UID& Module::get_uid() const
{
    return uid_;
}


const Path& Module::get_path() const
{
    return path_;
}


const String& Module::get_name() const
{
    return name_;
}


const String& Module::get_version() const
{
    return version_;
}


const String& Module::get_metadata_version() const
{
    return metadata_version_;
}


const StringArray& Module::get_class_list() const
{
    return class_list_;
}


const StringArray& Module::get_deps_list() const
{
    return deps_list_;
}


const String& Module::get_description() const
{
    return description_;
}


const String& Module::get_author() const
{
    return author_;
}


const URL& Module::get_update_url() const
{
    return update_url_;
}


const URL& Module::get_homepage_url() const
{
    return homepage_url_;
}


const StringStringMap& Module::get_contributors() const
{
    return contributors_;
}


const String& Module::get_min_system_version() const
{
    return min_system_version_;
}


const String& Module::get_license() const
{
    return license_;
}


const std::map<String, StringArray>& Module::get_eps() const
{
    return eps_;
}


const bool Module::is_local() const
{
    return local_;
}


bool Module::run()
{
    if (!local_) return false;

    // Module main filename format:
    // main_<os>[_<arch>][.py|.exe|]
    // os: windows, linux, macos, android, ios.
    // arch: x32, x64, arm or empty value.

    StringArray eps;

#if defined(ENVIRONMENT64)
    const String platform_tag = "x64";
#elif defined(ENVIRONMENT32)
    const String platform_tag = "x32";
#else
#   error "Unknown word size!"
#endif

#if defined(LINUX)
    const String os_tag = "linux";
#elif defined(MACOS)
    const String os_tag = "macos";
#elif defined(WINDOWS)
    const String os_tag = "windows";
#else
#   error "Unknown operating system!"
#endif

}


void Module::load_description(const Object &module_fields)
{
    for (auto const &m_data : module_fields)
    {
        if (m_data.first == "uid")
        {
            // Fake UID for the debug purposes.
            uid_ = static_cast<UID>(m_data.second.as_int64());
        }
        else if (m_data.first == "name")
        {
            name_ = m_data.second.as_string();
        }
        else if (m_data.first == "version")
        {
            version_ = m_data.second.as_string();
        }
        else if (m_data.first == "metadata_version")
        {
            metadata_version_ = m_data.second.as_string();
        }
        else if (m_data.first == "classes")
        {
            auto css = m_data.second.as_array();
            class_list_.reserve(css.size());

            for (auto const &v : css) class_list_.push_back(v.as_string());

        }
        else if (m_data.first == "dependencies")
        {
            // Modules or classes.
            auto mfa = m_data.second.as_array();
            deps_list_.reserve(mfa.size());

            for (auto const &v : mfa) deps_list_.push_back(v.as_string());
        }
        else if (m_data.first == "description")
        {
            description_ = m_data.second.as_string();
        }
        else if (m_data.first == "author")
        {
            author_ = m_data.second.as_string();
        }
        else if (m_data.first == "update_url")
        {
            update_url_ = m_data.second.as_string();
        }
        else if (m_data.first == "homepage")
        {
            homepage_url_ = m_data.second.as_string();
        }
        else if (m_data.first == "contributors")
        {
            // Additional module authors.
            for (auto const &v : m_data.second.as_object())
                contributors_[v.first] = v.second.as_string();
        }
        else if (m_data.first == "min_system_version")
        {
            min_system_version_ = m_data.second.as_string();
        }
        else if (m_data.first == "license")
        {
            license_ = m_data.second.as_string();
        }
        else if (m_data.first == "eps")
        {
            for (const auto &ep : m_data.second.as_object())
            {
                for (const auto &tag : ep.second.as_array())
                {
                    eps_[ep.first].push_back(tag.as_string());
                }
            }
        }
        else
        {
            LOG(LogLevel::Warning, "Unknown module description field: \"%s\"!", m_data.first.c_str());
        }
    }
}

bool Module::operator==(const Module& m) const
{
    // Is it enough?
    return m.uid_ == uid_ && m.name_ == name_ && m.path_ == path_ && m.author_ == author_;
}

} // namespace tsw
