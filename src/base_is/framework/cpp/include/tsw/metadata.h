/**
  * @file metadata.h
  * @author Artiom N.(cl)2017
  * @brief EAMetadata class definition.
  *
  */

#ifndef _TSW_METADATA_H
#define _TSW_METADATA_H

#include <list>
#include <map>
#include <memory>

#include "field.h"
#include "module.h"
#include "types.h"


namespace tsw
{

class EAMetadata;
typedef std::map<UID, FieldHeader>  FieldUIDFieldHeaderMap;
typedef std::list<EAMetadata>       EAMetadataList;


class EAMetadata
{
public:
   EAMetadata(const String& name, const std::shared_ptr<const Module> module,
              const String& description, const FieldUIDFieldHeaderMap &field_scheme);

public:
   const String& 						get_name() const;
   const String& 						get_description() const;
   const std::shared_ptr<const Module> 	get_linked_module() const;
   const FieldUIDFieldHeaderMap& 		get_field_scheme() const;

private:
   const String                         name_;
   const String                         description_;
   const FieldUIDFieldHeaderMap         field_scheme_;
   const std::weak_ptr<const Module>    linked_module_;
};

} // namespace tsw

#endif // _TSW_METADATA_H
