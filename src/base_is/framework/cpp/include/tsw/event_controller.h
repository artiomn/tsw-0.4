/**
  * @file event_controller.h
  * @author Artiom N.(cl)2017
  * @brief EventController class definition and EventHandler typedef.
  *
  */

#ifndef _TSW_EVENT_CONTROLLER_H
#define _TSW_EVENT_CONTROLLER_H

#include <functional>
#include <memory>

#include "event.h"
#include "magistral.h"
#include "metadata.h"
#include "types.h"


namespace tsw
{

typedef std::function<bool(Module&, const Event&)> EventHandler;


class EventController
{
public:
    EventController(std::shared_ptr<Magistral> magistral);
    ~EventController();

public:
   /**
    * @param events events list, provided by the module.
    */
   void announce_events(const Module &module, const EAMetadataList& events);

   /**
    * @param event
    */
   void announce_event(const Module &module, const EAMetadata& event);

   /**
    * @param event_name
    */
   void denounce_events(const Module &module, const StringList& event_names);

   /**
    * @param event_name
    */
   void denounce_event(const Module &module, const String& event_name);

   /**
    * @param event_name
    * @param handler
    */
   void subscribe(const Module &module, const String& event_name, EventHandler handler);

   /**
    * @param event_name
    */
   void unsubscribe(const Module &module, const String& event_name, EventHandler handler);

   /**
    * @param module_name
    */
   const EAMetadataList& get_events(const Module &module) const;
   const EAMetadataList& get_events() const;

   /**
    * @param event_name
    * @param fields
    */
   void generate_event(const Module &module, const String& event_name, NameValueMap fields);

private:
   class EventControllerImpl;
   std::unique_ptr<EventControllerImpl> controller_;
};

} // namespace tsw
#endif // _TSW_EVENT_CONTROLLER
