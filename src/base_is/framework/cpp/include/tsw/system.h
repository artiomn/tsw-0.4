/**
  * @file system.h
  * @author Artiom N.(cl)2017
  * @brief System class definition and RunHandler typedef.
  *
  */

#pragma once

#include <functional>
#include <memory>

#include "action_controller.h"
#include "cmd_line_parser.h"
#include "event_controller.h"
#include "localization.h"
#include "logger.h"
#include "magistral.h"
#include "module_controller.h"
#include "settings.h"
#include "types.h"


namespace tsw
{

class System;
typedef std::function<bool(System& system)> RunHandler;


class System
{
public:
    /**
     * @param magistral
     */
    System(std::shared_ptr<Magistral> magistral, const CmdLineParser &cmd_line_parser);

    /**
     * @param user_parameters
     */
    System(const CmdLineParser &cmd_line_parser);
    ~System();

public:
    Magistral&          get_magistral();
    ActionController&   get_action_controller();
    EventController&    get_event_controller();
    ModuleController&   get_module_controller();

    /**
     * @param module_data
     * @param announce
     */
    void register_me(const Module& module_data, bool announce = true);
    void unregister_me();

    static const String& get_version();
    static const String& get_install_path();

    /**
     * @param type
     */
    const SettingsMap& get_settings(const SettingsType type = SettingsType::stJoined) const;

    /**
     * @param settings
     * @param global
     */
    void save_settings(const SettingsMap& settings, bool global = false);

    /**
     * @param filename
     */
    void load_and_announce_events(const Path& filename = module_events_description_file);

    /**
     * @param filename
     */
    void load_and_announce_actions(const Path& filename = module_actions_description_file);

    /**
     * @param RunHandler handler
     */
    void run(RunHandler handler);

private:
    Settings                    settings_;
    std::shared_ptr<Magistral>  magistral_;
    ModuleController            module_controller_;
    ActionController            action_controller_;
    EventController             event_controller_;
};

} // namespace tsw
