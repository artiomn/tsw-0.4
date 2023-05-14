/**
  * @file action_controller.cpp
  * @author Artiom N.(cl)2017
  * @brief AcrionController class implementation.
  *
  */

#include <tsw/action_controller.h>

#include <tsw/magistral.h>
#include <tsw/metadata.h>
#include <tsw/module.h>
#include <tsw/types.h>

#include "controller_impl.h"


namespace tsw
{

class ActionController::ActionControllerImpl : public impl::ControllerImpl<Action, ActionHandler>
{
public:
    ActionControllerImpl(std::shared_ptr<Magistral> magistral) : ControllerImpl(magistral) {}
};


ActionController::ActionController(std::shared_ptr<Magistral> magistral) :
    controller_(std::make_unique<ActionController::ActionControllerImpl>(magistral))
{
}


ActionController::~ActionController()
{
}


void ActionController::announce_actions(const Module &module, const EAMetadataList& actions)
{
    controller_->announce_metadata(module.get_uid(), actions, MessageType::AnnounceModuleActions);
}


void ActionController::announce_action(const Module &module, const EAMetadata& action)
{
    controller_->announce_metadata(module.get_uid(), EAMetadataList{ action }, MessageType::AnnounceModuleActions);
}


void ActionController::denounce_actions(const Module &module, const StringList& action_names)
{
    controller_->denounce_metadata(module.get_uid(), action_names, MessageType::DenounceModuleActions);
}


void ActionController::denounce_action(const Module &module, const String& action_name)
{
    controller_->denounce_metadata(module.get_uid(), StringList{ action_name }, MessageType::DenounceModuleActions);
}


const EAMetadataList& ActionController::get_actions(const Module &module) const
{
    return controller_->get_metadata(module.get_uid(), MessageType::ReceiveActionsList);
}


const EAMetadataList& ActionController::get_actions() const
{
    return controller_->get_metadata(MessageType::ReceiveActionsList);
}


void ActionController::activate_action(const Module &module, const String& action_name, const NameValueMap& fields)
{
    controller_->generate_activity(module.get_uid(), action_name, fields, MessageType::Action);
}


void ActionController::register_action_handler(const Module &module, const String& action_name, ActionHandler handler)
{
    controller_->register_activity_handler(module.get_uid(), action_name, handler);
}


void ActionController::unregister_action_handler(const Module &module, const String& action_name, ActionHandler handler)
{
    controller_->unregister_activity_handler(module.get_uid(), action_name, handler);
}

} // namespace tsw
