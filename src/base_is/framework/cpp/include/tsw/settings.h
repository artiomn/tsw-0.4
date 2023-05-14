/**
  * @file settings.h
  * @author Artiom N.(cl)2017
  * @brief Settings class definition and SettingsType enum.
  *
  */

#ifndef _TSW_SETTINGS_H
#define _TSW_SETTINGS_H

#include "types.h"


namespace tsw
{

enum class SettingsType
{
    stDefaultSystem, stDefaultModule, stGlobal, stUser, stCommandLine, stJoined
};


#if defined(LINUX) || defined(MACOS)
/// Constant system-wide settings.
const Path system_settings_file = "/usr/share/tsw/settings.json";

/// User-defined system-wide setting.
const Path global_settings_file = "/etc/tsw/settings.json";

/// User settings database.
const Path user_settings_file = get_user_home() / ".tsw/settings.json";

#elif defined(WINDOWS)
const Path system_settings_file = WINDIR / "tsw" / "settings.json";
#endif


typedef NameValueMap SettingsMap;


class Settings
{
public:


public:
    Settings(const Path &system_defaults_path,
             const Path &module_path,
             const Path &global_path,
             const Path &user_path,
             const SettingsMap &user_settings);
    Settings(const Path &module_path,
             const SettingsMap &user_settings);

public:
    /**
     * @param type
     */
    const SettingsMap &get_settings(const SettingsType type = SettingsType::stJoined) const;

    /**
     * @param settings
     * @param global
     */
    void save_settings(const SettingsMap &settings, bool global = false);

    void reload_settings();

private:
    const Path system_defaults_path_;
    const Path module_path_;
    const Path global_path_;
    const Path user_path_;

    SettingsMap system_default_settings_;
    SettingsMap module_default_settings_;
    SettingsMap global_settings_;
    SettingsMap current_user_settings_;
    SettingsMap user_defined_settings_;

    SettingsMap joined_settings_cache_;
};

} // namespace tsw

#endif // _TSW_SETTINGS_H
