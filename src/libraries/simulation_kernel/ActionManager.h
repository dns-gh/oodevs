// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ActionManager_h_
#define __ActionManager_h_

#include <cstdint>
#include <map>

namespace sword
{
    class Action;
    class AutomatOrder;
    class CrowdOrder;
    class FragOrder;
    class KnowledgeMagicAction;
    class MagicAction;
    class ObjectMagicAction;
    class SetAutomatMode;
    class UnitMagicAction;
    class UnitOrder;
}

// ActionManager stores all orders and magic actions and assign to them a
// unique id When a new client connects, it will receive all previously
// stored magic orders.
class ActionManager
{
public:
             ActionManager();
    virtual ~ActionManager();

    template< typename Archive > void serialize( Archive& file, const unsigned int version );

    void Hide( uint32_t client, uint32_t id );

    struct Order { bool created; uint32_t id; };
    Order Register( uint32_t client, const sword::AutomatOrder& msg );
    Order Register( uint32_t client, const sword::CrowdOrder& msg );
    Order Register( uint32_t client, const sword::UnitOrder& msg );

    uint32_t Register  ( uint32_t client, const sword::FragOrder& msg );
    uint32_t Register  ( uint32_t client, const sword::KnowledgeMagicAction& msg );
    uint32_t Register  ( uint32_t client, const sword::MagicAction& msg );
    uint32_t Register  ( uint32_t client, const sword::ObjectMagicAction& msg );
    uint32_t Register  ( uint32_t client, const sword::SetAutomatMode& msg );
    uint32_t Register  ( uint32_t client, const sword::UnitMagicAction& msg );
    void     Send      ( uint32_t id, int32_t code, const std::string& msg );
    void     Unregister( uint32_t id );

    virtual void SendStateToNewClient();

private:
    uint32_t Register( uint32_t client, const sword::Action& msg );
    Order    RegisterOrder( uint32_t client, uint32_t order, const sword::Action& msg );

private:
    uint32_t ids_;
    std::map< uint32_t, sword::Action > actions_;
    std::map< uint32_t, uint32_t > orders_; // map order ids to action ids
    std::set< uint32_t > hidden_; // hidden client ids
};

BOOST_CLASS_EXPORT_KEY( ActionManager )

#endif // __ActionManager_h_
