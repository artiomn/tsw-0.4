/**
  * @file controller_impl.h
  * @author Artiom N.(cl)2017
  * @brief Common controller class definition.
  *
  */

#pragma once

#include <list>
#include <map>

#include "tsw/error.h"
#include <tsw/magistral.h>
#include "tsw/message.h"
#include "tsw/metadata.h"
#include "tsw/types.h"

#include "functional_helper.h"


namespace tsw
{

namespace impl
{

template<typename ActivityType, typename HandlerType>
class ControllerImpl
{
public:
    ControllerImpl(std::shared_ptr<Magistral> magistral) : magistral_(magistral) {}
    ~ControllerImpl()
    {
        // Denounce metadata.
    }

public:
    /**
     * @brief Append metadata to the controller map.
     */
    void append_metadata(const UID module_uid, const EAMetadataList &metadata)
    {
        auto module_metadata = metadata_container_[module_uid];

        for (auto i: metadata)
        {
            module_metadata.emplace(i.get_name(), i);
        }
    }

    void remove_metadata(UID module_uid, const StringList &metadata)
    {
        auto module_metadata = metadata_container_[module_uid];

        for (auto i: metadata)
        {
            module_metadata.erase(i);
        }
    }

    void announce_metadata(UID module_uid, const EAMetadataList &metadata, MessageType message_type)
    {
        append_metadata(module_uid, metadata);

        NameValueMap msg_fields;
        ValueArray md;

        for (auto const& i : metadata)
        {
            Object m_entry;

            m_entry["name"] = i.get_name();
            m_entry["description"] = i.get_description();
            m_entry["linked_module"] = std::to_string(i.get_linked_module()->get_uid());

            Object fs;

            for (const auto& f : i.get_field_scheme())
            {
                const auto& h_value = f.second;
                auto uid_str = std::to_string(f.first);
                Object fv;

                fv["uid"] = std::to_string(h_value.uid);
                fv["type"] = static_cast<int32_t>(h_value.type);
                fv["name"] = h_value.name;
                fv["position"] = h_value.position;
                fv["description"] = h_value.description;
                fs[uid_str] = std::move(fv);
            }

            m_entry["field_scheme"] = std::move(fs);
            md.push_back(std::move(m_entry));
        }

        msg_fields["metadata"] = std::move(md);

        magistral_->send_message(message_type, msg_fields, module_uid);
    }

    void denounce_metadata(UID module_uid, const StringList &metadata_list, MessageType message_type)
    {
        remove_metadata(module_uid, metadata_list);

        Object md;
        ValueArray va;

        for (auto const& i : metadata_list) va.push_back(i);
        md["metadata"] = std::move(va);
        magistral_->send_message(message_type, std::move(md), module_uid);
    }

    void generate_activity(UID module_uid, const String activity_name, const NameValueMap &fields,
                           MessageType message_type)
    {
        auto msg_fields = std::move(fields);

        msg_fields["name"] = activity_name;
        magistral_->send_message(message_type, fields, module_uid);
    }

    EAMetadataList &&get_metadata(MessageType message_type)
    {

    }

    EAMetadataList &&get_metadata(UID module_uid, MessageType message_type)
    {
        EAMetadataList result;

        auto metadata_from_storage = [&]()
        {
            for (auto activity: metadata_container_.at(module_uid))
            {
                result.push_back(activity.second.metadata);
            }
        };

        try
        {

        }
        catch(const std::out_of_range&)
        {
            //magistral_->send_message(message_type,
            metadata_from_storage();
        }

        //metadata_container_;

        //[](const Message& message) -> bool { return true; }
        return std::move(result);
    }

    void register_activity_handler(UID module_uid, const String &activity_name, HandlerType handler)
    {
        auto module_activity = metadata_container_[module_uid].at(activity_name);
        module_activity.handlers.push_back(handler);
    }

    void unregister_activity_handler(UID module_uid, String activity_name, HandlerType handler)
    {
        auto module_activity = metadata_container_[module_uid][activity_name];
        HandlerList &handlers = module_activity.handlers;

        for (auto i = handlers.begin(); i != handlers.end(); ++i)
        {
            if (*i == handler) handlers.erase(i);
        }
    }

    void run_activity_handlers(UID module_uid, const String &activity_name, const NameValueMap &parameters, Module &generator)
    {
        auto module_metadata = metadata_container_[module_uid][activity_name];

        for (auto i: module_metadata.handlers)
        {
            i(generator, parameters);
        }
    }

private:
    typedef std::list<HandlerType> HandlerList;

    struct ActivityData
    {
        ActivityData(const EAMetadata &m) : metadata(m) {}
        ActivityData(EAMetadata &&m) : metadata(std::move(m)) {}
        ActivityData() : metadata("", nullptr, "", FieldUIDFieldHeaderMap())
        {
            static_assert(true, "ActivityData is not default constructible!");
        }
        ActivityData &operator=(const ActivityData&) = default;

        const EAMetadata metadata;
        HandlerList handlers;
    };

    typedef std::map<String, ActivityData> ModuleMetadata;

private:
    std::shared_ptr<Magistral> magistral_;
    std::map<UID, ModuleMetadata> metadata_container_;
};

} // namespace impl

} // namespace tsw
