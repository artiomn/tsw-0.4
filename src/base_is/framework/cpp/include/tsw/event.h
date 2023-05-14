/**
  * @file event.h
  * @author Artiom N.(cl)2017
  * @brief Event class definition.
  *
  */

#ifndef _TSW_EVENT_H
#define _TSW_EVENT_H

#include <memory>

#include "metadata.h"


namespace tsw
{

class EventController;


class Event
{
public:
   Event(const std::shared_ptr<EAMetadata> metadata, const NameValueMap& fields);

public:
   const EAMetadata& 	get_metadata() const;
   const NameValueMap& 	get_fields() const;

private:
   const std::shared_ptr<EAMetadata>    metadata_;
   const NameValueMap               	fields_;
};

} // namespace tsw

#endif // _TSW_EVENT_H
