/**
  * @file action.cpp
  * @author Artiom N.(cl)2017
  * @brief Action class implementation.
  *
  */

#include <memory>

#include <tsw/action.h>
#include <tsw/action_controller.h>
#include <tsw/types.h>


namespace tsw
{

Action::Action(const std::shared_ptr<EAMetadata> metadata, const NameValueMap& fields) :
    metadata_(metadata), fields_(fields)
{

}


const EAMetadata& Action::get_metadata() const
{
    return *metadata_;
}


const NameValueMap& Action::get_fields() const
{
    return fields_;
}

} // namespace tsw
