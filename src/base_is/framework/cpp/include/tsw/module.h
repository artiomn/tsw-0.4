/**
  * @file module.h
  * @author Artiom N.(cl)2017
  * @brief Module class definition.
  *
  */

#ifndef _TSW_MODULE_H
#define _TSW_MODULE_H

#include "types.h"


namespace tsw
{

typedef String ModuleClass;

class Module
{
public:
    Module(const UID& uid, const Path& path, const String& name, const String& version,
           const String& metadata_version,
           const StringArray& class_list, const StringArray& deps_list,
           const String& description, const String& author, const URL& update_url,
           const URL& homepage_url, const StringStringMap& contributors,
           const String& min_system_version, const String& license,
           bool local = false);
    Module(const Path& description_json);
    Module(const BinData& json, const Path& module_path);
    Module() = delete;
    virtual ~Module();

public:
    bool operator==(const Module&) const;

public:
    const UID&              get_uid() const;
    const Path&             get_path() const;
    const String&           get_name() const;
    const String&           get_version() const;
    const String&           get_metadata_version() const;
    const StringArray&      get_class_list() const;
    const StringArray&      get_deps_list() const;
    const String&           get_description() const;
    const String&           get_author() const;
    const URL&              get_update_url() const;
    const URL&              get_homepage_url() const;
    const StringStringMap&  get_contributors() const;
    const String&           get_min_system_version() const;
    const String&           get_license() const;
    const std::map<String, StringArray>& get_eps() const;
    const bool              is_local() const;

public:
    bool run();

private:
    void load_description(const Object &module_fields);

private:
   UID              uid_;
   Path             path_;
   String           name_;
   String           version_;
   String           metadata_version_;
   StringArray      class_list_;
   StringArray      deps_list_;
   String           description_;
   String           author_;
   URL              update_url_;
   URL              homepage_url_;
   StringStringMap  contributors_;
   String           min_system_version_;
   String           license_;
   std::map<String, StringArray> eps_;
   bool local_;
};

} // namespace tsw

#endif // _TSW_MODULE_H
