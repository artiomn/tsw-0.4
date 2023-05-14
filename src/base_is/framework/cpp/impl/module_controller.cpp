/**
  * @file module_controller.cpp
  * @author Artiom N.(cl)2017
  * @brief ModuleController class implementation.
  *
  */

#include <list>
#include <memory>

#include <tsw/module.h>
#include <tsw/module_controller.h>
#include <tsw/json_deserializer.h>
#include <tsw/types.h>


namespace tsw
{

ModuleController::ModuleController(std::shared_ptr<Magistral> magistral) : magistral_(magistral)
{

}


ModulesList::value_type ModuleController::load_module(const Path& path)
{
    auto m_path = path / module_description_file;
    auto modules = get_modules_by_path(m_path);
    auto module_iter = std::find_if(modules.cbegin(), modules.cend(), [&m_path](const ModulesList::value_type& m) { return m->get_path() == m_path && m->is_local(); });

    if (module_iter != modules.cend()) return *module_iter;

    auto module = std::make_shared<Module>(m_path);

    modules_.push_back(module);

    return module;
}


void ModuleController::unload_module(const String& name)
{
    modules_.remove_if([&name](const ModulesList::value_type& m) {return m->get_name() == name;});
}


void ModuleController::unload_module(const Module& module)
{
    modules_.remove_if([&module](const ModulesList::value_type& m) {return *m == module;});
}


void ModuleController::unload_module(const UID& uid)
{
    modules_.remove_if([&uid](const ModulesList::value_type& m) {return m->get_uid() == uid;});
}


const ModulesList& ModuleController::get_modules_list() const
{
    return modules_;
}


const ModulesList&& ModuleController::get_modules_by_name(const String& name) const
{
    decltype(modules_) result;

    for (const auto &m : modules_)
    {
        if (m->get_name() == name) result.push_back(m);
    }

    return std::move(result);
}


const ModulesList&& ModuleController::get_modules_by_path(const Path& path) const
{
    decltype(modules_) result;

    for (const auto &m : modules_)
    {
        if (m->get_path() == path) result.push_back(m);
    }

    return std::move(result);
}


const ModulesList::value_type ModuleController::get_module_by_uid(const UID& uid) const
{
    for (const auto &m : modules_)
    {
        if (m->get_uid() == uid) return m;
    }

    return ModulesList::value_type();
}

} // namespace tsw
