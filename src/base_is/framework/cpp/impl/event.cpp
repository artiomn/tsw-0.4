/**
  * @file event.cpp
  * @author Artiom N.(cl)2017
  * @brief Event class implementation.
  *
  */

#include <memory>

#include <tsw/event.h>
#include <tsw/event_controller.h>
#include <tsw/metadata.h>
#include <tsw/types.h>


namespace tsw
{

Event::Event(const std::shared_ptr<EAMetadata> metadata, const NameValueMap& fields) :
    metadata_(metadata), fields_(fields)
{
}


const EAMetadata& Event::get_metadata() const
{
    return *metadata_;
}


const NameValueMap& Event::get_fields() const
{
    return fields_;
}

} // namespace tsw
