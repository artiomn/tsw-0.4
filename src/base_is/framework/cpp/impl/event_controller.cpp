/**
  * @file event_controller.cpp
  * @author Artiom N.(cl)2017
  * @brief EventController class implementation.
  *
  */

#include <memory>

#include <tsw/event_controller.h>
#include <tsw/magistral.h>
#include <tsw/metadata.h>
#include <tsw/types.h>

#include "controller_impl.h"


namespace tsw
{

class EventController::EventControllerImpl : public impl::ControllerImpl<Event, EventHandler>
{
public:
    EventControllerImpl(std::shared_ptr<Magistral> magistral) : ControllerImpl(magistral) {}
};


EventController::EventController(std::shared_ptr<Magistral> magistral) :
    controller_(new EventControllerImpl(magistral))
{
}


EventController::~EventController()
{
}


void EventController::announce_events(const Module &module, const EAMetadataList& events)
{
    controller_->announce_metadata(module.get_uid(), events, MessageType::AnnounceModuleEvents);
}


void EventController::announce_event(const Module &module, const EAMetadata& event)
{
    controller_->announce_metadata(module.get_uid(), EAMetadataList{ event }, MessageType::AnnounceModuleEvents);
}


void EventController::denounce_events(const Module &module, const StringList& event_names)
{
    controller_->denounce_metadata(module.get_uid(), event_names, MessageType::DenounceModuleEvents);
}


void EventController::denounce_event(const Module &module, const String& event_name)
{
    controller_->denounce_metadata(module.get_uid(), StringList{ event_name }, MessageType::DenounceModuleEvents);
}


void EventController::subscribe(const Module &module, const String& event_name, EventHandler handler)
{
    controller_->register_activity_handler(module.get_uid(), event_name, handler);
}


void EventController::unsubscribe(const Module &module, const String& event_name, EventHandler handler)
{
    controller_->unregister_activity_handler(module.get_uid(), event_name, handler);
}


const EAMetadataList& EventController::get_events(const Module &module) const
{
    return controller_->get_metadata(module.get_uid(), MessageType::ReceiveEventsList);
}


const EAMetadataList& EventController::get_events() const
{
    return controller_->get_metadata(MessageType::ReceiveEventsList);
}


void EventController::generate_event(const Module &module, const String& event_name, NameValueMap fields)
{
    controller_->generate_activity(module.get_uid(), event_name, fields, MessageType::Event);
}

} // namespace tsw
