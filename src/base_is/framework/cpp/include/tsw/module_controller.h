/**
  * @file module_controller.h
  * @author Artiom N.(cl)2017
  * @brief ModuleController class definition.
  *
  */

#ifndef _TSW_MODULE_CONTROLLER_H
#define _TSW_MODULE_CONTROLLER_H

#include <list>
#include <map>
#include <memory>

#include "magistral.h"
#include "module.h"
#include "types.h"


namespace tsw
{

typedef std::list<std::shared_ptr<Module>> ModulesList;

/// Module-specific executable files.
const Path module_binary_directory          = "bin";

/// Module-specific data files.
const Path module_data_directory            = "share";

/// Module-specific documentation.
const Path module_doc_directory             = "docs";

/// Description file for user.
const Path module_readme_filename           = "README.md";

/// Module-specific executable libraries.
const Path module_module_library_directory  = "lib";

/// Module-specific localisation files.
const Path module_locale_directory          = "locale";

/// Module-specific tests files.
const Path module_tests_directory           = "tests";

/// Actions description file. If exists, actions will be loaded from it.
const Path module_actions_description_file  = module_data_directory / "actions.json";

/// Module default settings file.
const Path module_default_settings_file     = module_data_directory / "defaults.json";

/// Events description file. If exists, events will be loaded from it.
const Path module_events_description_file   = module_data_directory / "events.json";

/// Module metadata file.
const Path module_description_file          = "package.json";


class ModuleController
{
public:
    ModuleController(std::shared_ptr<Magistral> magistral);

public:
   /**
    * @param path
    */
   ModulesList::value_type load_module(const Path& path);

   /**
    * @param name
    */
   void unload_module(const String& name);

   /**
    * @param module
    */
   void unload_module(const Module& module);

   /**
    * @param uid
    */
   void unload_module(const UID& uid);

   const ModulesList& get_modules_list() const;

   /**
    * @param name
    */
   const ModulesList&& get_modules_by_name(const String& name) const;

   /**
    * @param path
    */
   const ModulesList&& get_modules_by_path(const Path& path) const;

   /**
    * @param uid
    */
   const ModulesList::value_type get_module_by_uid(const UID& uid) const;

private:
   ModulesList modules_;
   std::weak_ptr<Magistral> magistral_;
};

} // namespace tsw

#endif // _TSW_MODULE_CONTROLLER_H
