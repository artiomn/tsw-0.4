/**
  * @file action.h
  * @author Artiom N.(cl)2017
  * @brief Action class definition.
  *
  */

#ifndef _TSW_ACTION_H
#define _TSW_ACTION_H

#include <memory>

#include "metadata.h"
#include "types.h"


namespace tsw
{

class ActionController;


class Action
{
public:
    Action(const std::shared_ptr<EAMetadata> metadata, const NameValueMap& fields);
    virtual ~Action() = default;

    const EAMetadata&    get_metadata() const;
    const NameValueMap&  get_fields() const;
private:
    const std::shared_ptr<EAMetadata>	metadata_;
    const NameValueMap               	fields_;
};

}

#endif // _TSW_ACTION_H
