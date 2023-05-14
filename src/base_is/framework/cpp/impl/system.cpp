/**
  * @file system.cpp
  * @author Artiom N.(cl)2017
  * @brief System class implementation.
  *
  */

#include <memory>

#include <tsw/action_controller.h>
#include <tsw/event_controller.h>
#include <tsw/localization.h>
#include <tsw/magistral.h>
#include <tsw/module_controller.h>
#include <tsw/settings.h>
#include <tsw/system.h>
#include <tsw/types.h>


namespace tsw
{

System::System(std::shared_ptr<Magistral> magistral, const CmdLineParser &cmd_line_parser) :
    settings_(Path(""), cmd_line_parser.get_fields()),
    magistral_(magistral),
    module_controller_(magistral_),
    action_controller_(magistral_),
    event_controller_(magistral_)
{

}


System::System(const CmdLineParser &cmd_line_parser) :
    settings_(Path(""), cmd_line_parser.get_fields()),
    magistral_(std::make_shared<Magistral>(settings_.get_settings().at("magistral").as_string())),
    module_controller_(magistral_),
    action_controller_(magistral_),
    event_controller_(magistral_)
{

}


System::~System()
{

}


Magistral& System::get_magistral()
{
    return *magistral_;
}


ActionController& System::get_action_controller()
{
    return action_controller_;
}


EventController& System::get_event_controller()
{
    return event_controller_;
}


ModuleController& System::get_module_controller()
{
    return module_controller_;
}


void System::register_me(const Module& module_data, bool announce)
{
}


void System::unregister_me()
{
}


const String& System::get_version()
{
    //module_controller_.
}


const String& System::get_install_path()
{
}


const SettingsMap& System::get_settings(const SettingsType type) const
{
    return settings_.get_settings(type);
}


void System::save_settings(const SettingsMap& settings, bool global)
{
    settings_.save_settings(settings, global);
}


void System::load_and_announce_events(const Path& filename)
{
}


void System::load_and_announce_actions(const Path& filename)
{
}


void System::run(RunHandler handler)
{
}

} // namespace tsw
