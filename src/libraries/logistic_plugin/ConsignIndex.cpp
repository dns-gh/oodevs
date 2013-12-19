// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ConsignIndex.h"
#include "protocol/MessageParameters.h"
#include "protocol/Simulation.h"
#include <boost/make_shared.hpp>

using namespace plugins::logistic;

ConsignEvent::ConsignEvent( LogisticPlugin::E_LogisticType t, E_ConsignEvent a, int i )
    : type( t ), action( a ), id( i )
{
}

namespace
{

template< typename T >
ConsignEvent MakeEvent( const T& msg, LogisticPlugin::E_LogisticType type, E_ConsignEvent action )
{
    return ConsignEvent( type, action,  msg.has_request() ? msg.request().id() : -1 );
}

#define MAKE_EVENT( M, T, E ) \
    if( msg.has_##M() ) return MakeEvent( msg.M(), LogisticPlugin::T, E )

ConsignEvent GetEventType( const sword::SimToClient& message )
{
    const auto& msg = message.message();
    MAKE_EVENT( log_funeral_handling_creation,    eLogisticType_Funeral, eConsignCreation );
    MAKE_EVENT( log_funeral_handling_update,      eLogisticType_Funeral, eConsignUpdate );
    MAKE_EVENT( log_funeral_handling_destruction, eLogisticType_Funeral, eConsignDestruction );

    MAKE_EVENT( log_maintenance_handling_creation,    eLogisticType_Maintenance, eConsignCreation );
    MAKE_EVENT( log_maintenance_handling_update,      eLogisticType_Maintenance, eConsignUpdate );
    MAKE_EVENT( log_maintenance_handling_destruction, eLogisticType_Maintenance, eConsignDestruction );

    MAKE_EVENT( log_medical_handling_creation,    eLogisticType_Medical, eConsignCreation );
    MAKE_EVENT( log_medical_handling_update,      eLogisticType_Medical, eConsignUpdate );
    MAKE_EVENT( log_medical_handling_destruction, eLogisticType_Medical, eConsignDestruction );

    MAKE_EVENT( log_supply_handling_creation,    eLogisticType_Supply, eConsignCreation );
    MAKE_EVENT( log_supply_handling_update,      eLogisticType_Supply, eConsignUpdate );
    MAKE_EVENT( log_supply_handling_destruction, eLogisticType_Supply, eConsignDestruction );

    return ConsignEvent( LogisticPlugin::eNbrLogisticType, eConsignUnknown, -1 );
}
#undef MAKE_EVENT

} // namespace

struct plugins::logistic::LiveConsign
{
    LiveConsign()
        : entry_( boost::make_shared< sword::LogHistoryEntry >() )
    {
    }

    void Update( const sword::SimToClient& message, std::vector< uint32_t >& entities );

    boost::shared_ptr< sword::LogHistoryEntry > entry_;
};

void LiveConsign::Update( const sword::SimToClient& message,
        std::vector< uint32_t >& entities )
{
    const auto& msg = message.message();
    if( msg.has_log_funeral_handling_creation() )
    {
        const auto& sub = msg.log_funeral_handling_creation();
        *entry_->mutable_funeral()->mutable_creation() = sub;
        entities.push_back( sub.unit().id() );
    }
    else if( msg.has_log_funeral_handling_update() )
    {
        const auto& sub = msg.log_funeral_handling_update();
        entry_->mutable_funeral()->mutable_update()->MergeFrom( sub );
        if( sub.has_handling_unit() )
            entities.push_back( protocol::GetParentEntityId( sub.handling_unit() ));
        if( sub.has_convoying_unit() )
            entities.push_back( sub.convoying_unit().id() );
    }
    else if( msg.has_log_funeral_handling_destruction() )
    {
        *entry_->mutable_funeral()->mutable_destruction() =
            msg.log_funeral_handling_destruction();
    }
    else if( msg.has_log_maintenance_handling_creation() )
    {
        const auto& sub = msg.log_maintenance_handling_creation();
        *entry_->mutable_maintenance()->mutable_creation() = sub;
        entities.push_back( sub.unit().id() );
    }
    else if( msg.has_log_maintenance_handling_update() )
    {
        const auto& sub = msg.log_maintenance_handling_update();
        entry_->mutable_maintenance()->mutable_update()->MergeFrom( sub );
        entities.push_back( sub.unit().id() );
        entities.push_back( sub.provider().id() );
    }
    else if( msg.has_log_maintenance_handling_destruction() )
    {
        *entry_->mutable_maintenance()->mutable_destruction() =
            msg.log_maintenance_handling_destruction();
    }
    else if( msg.has_log_medical_handling_creation() )
    {
        const auto& sub = msg.log_medical_handling_creation();
        *entry_->mutable_medical()->mutable_creation() = sub;
        entities.push_back( sub.unit().id() );
    }
    else if( msg.has_log_medical_handling_update() )
    {
        const auto& sub = msg.log_medical_handling_update();
        entry_->mutable_medical()->mutable_update() ->MergeFrom( sub );
        entities.push_back( sub.unit().id() );
        if( sub.has_provider() )
            entities.push_back( sub.provider().id() );
    }
    else if( msg.has_log_medical_handling_destruction() )
    {
        *entry_->mutable_medical()->mutable_destruction() =
            msg.log_medical_handling_destruction();
    }
    else if( msg.has_log_supply_handling_creation() )
    {
        const auto& sub = msg.log_supply_handling_creation();
        *entry_->mutable_supply()->mutable_creation()  = sub;
        entities.push_back( protocol::GetParentEntityId( sub.supplier() ));
        entities.push_back( protocol::GetParentEntityId( sub.transporters_provider() ));
    }
    else if( msg.has_log_supply_handling_update() )
    {
        // Sub-messages are merged recursively and repeated fields are *appended*.
        // Clear them before merging. This is fragile but saves tons of code
        // right now.
        const auto& sub = msg.log_supply_handling_update();
        entry_->mutable_supply()->mutable_update()->mutable_requests()->Clear();
        entry_->mutable_supply()->mutable_update()->MergeFrom( sub );
        if( sub.has_convoyer() )
            entities.push_back( sub.convoyer().id() );
        if( sub.has_requests() )
        {
            const int count = sub.requests().requests().size();
            for( int i = 0; i != count; ++i )
                entities.push_back( sub.requests().requests( i ).recipient().id() );
        }
    }
    else if( msg.has_log_supply_handling_destruction() )
    {
        *entry_->mutable_supply()->mutable_destruction() =
            msg.log_supply_handling_destruction();
    }
}

ConsignIndex::ConsignIndex()
{
}

ConsignIndex::~ConsignIndex()
{
}

ConsignEvent ConsignIndex::Update( const sword::SimToClient& message,
        std::vector< uint32_t >& entities, int tick )
{
    auto ev = GetEventType( message );
    if( ev.id <= 0 )
        return ev;
    auto it = consigns_.find( ev.id );
    if( ev.action == eConsignCreation || it == consigns_.end() )
    {
        auto consign = std::auto_ptr< LiveConsign >( new LiveConsign() );
        if( it == consigns_.end() )
            it = consigns_.insert( ev.id, consign ).first;
        else
            consigns_.replace( it, consign );
    }
    it->second->Update( message, entities );
    it->second->entry_->set_tick( tick );
    ev.entry = it->second->entry_;
    if( ev.action == eConsignDestruction )
        consigns_.erase( it );
    return ev;
}

