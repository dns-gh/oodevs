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

void UpdateEntry( const sword::SimToClient& message, sword::LogHistoryEntry& entry )
{
    const auto& msg = message.message();
    if( msg.has_log_funeral_handling_creation() )
    {
        *entry.mutable_funeral()->mutable_creation() =
            msg.log_funeral_handling_creation();
    }
    else if( msg.has_log_funeral_handling_update() )
    {
        *entry.mutable_funeral()->mutable_update() =
            msg.log_funeral_handling_update();
    }
    else if( msg.has_log_funeral_handling_destruction() )
    {
        *entry.mutable_funeral()->mutable_destruction() =
            msg.log_funeral_handling_destruction();
    }
    else if( msg.has_log_maintenance_handling_creation() )
    {
        *entry.mutable_maintenance()->mutable_creation() =
            msg.log_maintenance_handling_creation();
    }
    else if( msg.has_log_maintenance_handling_update() )
    {
        *entry.mutable_maintenance()->mutable_update() =
            msg.log_maintenance_handling_update();
    }
    else if( msg.has_log_maintenance_handling_destruction() )
    {
        *entry.mutable_maintenance()->mutable_destruction() =
            msg.log_maintenance_handling_destruction();
    }
    else if( msg.has_log_medical_handling_creation() )
    {
        *entry.mutable_medical()->mutable_creation() =
            msg.log_medical_handling_creation();
    }
    else if( msg.has_log_medical_handling_update() )
    {
        *entry.mutable_medical()->mutable_update() =
            msg.log_medical_handling_update();
    }
    else if( msg.has_log_medical_handling_destruction() )
    {
        *entry.mutable_medical()->mutable_destruction() =
            msg.log_medical_handling_destruction();
    }
    else if( msg.has_log_supply_handling_creation() )
    {
        *entry.mutable_supply()->mutable_creation() =
            msg.log_supply_handling_creation();
    }
    else if( msg.has_log_supply_handling_update() )
    {
        *entry.mutable_supply()->mutable_update() =
            msg.log_supply_handling_update();
    }
    else if( msg.has_log_supply_handling_destruction() )
    {
        *entry.mutable_supply()->mutable_destruction() =
            msg.log_supply_handling_destruction();
    }
}

} // namespace

ConsignIndex::ConsignIndex()
{
}

ConsignIndex::~ConsignIndex()
{
}

ConsignEvent ConsignIndex::Update( const sword::SimToClient& message, int tick )
{
    auto ev = GetEventType( message );
    if( ev.id <= 0 )
        return ev;
    auto it = consigns_.find( ev.id );
    if( ev.action == eConsignCreation || it == consigns_.end() )
    {
        auto e = boost::make_shared< sword::LogHistoryEntry >();
        if( it == consigns_.end() )
            it = consigns_.insert( std::make_pair( ev.id, e ) ).first;
        else
            it->second.swap( e );
    }
    UpdateEntry( message, *it->second );
    it->second->set_tick( tick );
    ev.entry = it->second;
    if( ev.action == eConsignDestruction )
        consigns_.erase( it );
    return ev;
}
