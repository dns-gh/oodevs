// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_vision_Units_h
#define plugins_vision_Units_h

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <map>
#include <set>

namespace sword
{
    class SimToClient;
    class UnitId;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace client
{
    class ListEnabledVisionConesAck;
}

namespace plugins
{
namespace vision
{
// =============================================================================
/** @class  Units
    @brief  Mapping between clients and registered units
*/
// Created: MCO 2013-08-21
// =============================================================================
class Units
{
public:
    void Register( dispatcher::ClientPublisher_ABC& publisher, const sword::UnitId& id );

    void Unregister( dispatcher::ClientPublisher_ABC& publisher, const sword::UnitId& id );
    void Unregister( dispatcher::ClientPublisher_ABC& publisher );

    void List( dispatcher::ClientPublisher_ABC&, unsigned int start, std::size_t max, client::ListEnabledVisionConesAck& ack );
    void Send( unsigned int id, const sword::SimToClient& message );

private:
    struct Value
    {
        Value( dispatcher::ClientPublisher_ABC* publisher, unsigned int id )
            : publisher_( publisher )
            , id_       ( id )
        {}
        dispatcher::ClientPublisher_ABC* publisher_;
        unsigned int id_;
    };

    boost::multi_index::multi_index_container<
            Value,
            boost::multi_index::indexed_by<
                boost::multi_index::ordered_unique<
                    boost::multi_index::composite_key<
                        Value,
                        boost::multi_index::member<
                            Value, dispatcher::ClientPublisher_ABC*, &Value::publisher_ >,
                        boost::multi_index::member<
                            Value, unsigned int, &Value::id_ >
                    >
                >,
                boost::multi_index::ordered_non_unique<
                    boost::multi_index::member<
                        Value, unsigned int, &Value::id_ >
                >
            >
        > units_;
};

}
}

#endif // plugins_vision_Units_h
