/**
  * @file action_controller.h
  * @author Artiom N.(cl)2017
  * @brief ActionController class definition and ActionHandler typedef.
  *
  */

#ifndef _TSW_ACTION_CONTROLLER_H
#define _TSW_ACTION_CONTROLLER_H

#include <functional>
#include <memory>

#include "action.h"
#include "magistral.h"
#include "metadata.h"
#include "module.h"
#include "types.h"


namespace tsw
{

typedef std::function<bool(Module&, const Action&)> ActionHandler;

class ActionController
{
public:
    ActionController(std::shared_ptr<Magistral> magistral);
    virtual ~ActionController();

public:
   /**
    * @param actions
    */
   void announce_actions(const Module &module, const EAMetadataList& actions);

   /**
    * @param action
    */
   void announce_action(const Module &module, const EAMetadata& action);

   /**
    * @param action_name
    */
   void denounce_actions(const Module &module, const StringList& action_names);

   /**
    * @param action_name
    */
   void denounce_action(const Module &module, const String& action_name);

   /**
    * @param module_name
    */
   const EAMetadataList& get_actions(const Module& module) const;
   const EAMetadataList& get_actions() const;

   /**
    * @param action_name
    * @param fields
    */
   void activate_action(const Module &module, const String& action_name, const NameValueMap& fields);

   /**
    * @param action_name
    * @param handler
    */
   void register_action_handler(const Module &module, const String& action_name, ActionHandler handler);

   /**
    * @param action_name
    * @param handler
    */
   void unregister_action_handler(const Module &module, const String& action_name, ActionHandler handler);

private:
   class ActionControllerImpl;
   std::unique_ptr<ActionControllerImpl> controller_;
};

} // namespace tsw

#endif //_TSW_ACTION_CONTROLLER_H
