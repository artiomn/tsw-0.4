/**
  * @file metadata.cpp
  * @author Artiom N.(cl)2017
  * @brief EAMetadata class implementation.
  *
  */

#include <list>
#include <map>
#include <memory>

#include <tsw/field.h>
#include <tsw/metadata.h>
#include <tsw/module.h>
#include <tsw/types.h>


namespace tsw
{

EAMetadata::EAMetadata(const String& name, const std::shared_ptr<const Module> module,
        const String& description, const FieldUIDFieldHeaderMap &field_scheme) :
    name_(name), description_(description), field_scheme_(field_scheme),
    linked_module_(module)
{
}


const String& EAMetadata::get_name() const
{
    return name_;
}


const String& EAMetadata::get_description() const
{
    return description_;
}


const std::shared_ptr<const Module> EAMetadata::get_linked_module() const
{
    return linked_module_.lock();
}


const FieldUIDFieldHeaderMap& EAMetadata::get_field_scheme() const
{
    return field_scheme_;
}

} // namespace tsw
