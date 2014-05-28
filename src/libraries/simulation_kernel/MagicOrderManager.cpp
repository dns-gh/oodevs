// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MagicOrderManager.h"
#include "MIL_Time_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/Serialization.h"

#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

BOOST_SERIALIZATION_SPLIT_FREE( sword::MagicOrder );

BOOST_CLASS_EXPORT_IMPLEMENT( MagicOrderManager );

MagicOrderManager::MagicOrderManager()
    : ids_( 0 )
{
    // NOTHING
}

MagicOrderManager::~MagicOrderManager()
{
    // NOTHING
}

template< typename Archive >
void MagicOrderManager::serialize( Archive& ar, const unsigned int )
{
    ar & ids_ & orders_;
}

namespace
{
    const std::string MakeNow()
    {
        sword::DateTime now;
        NET_ASN_Tools::WriteGDH( MIL_Time_ABC::GetTime().GetRealTime(), now );
        return now.data();
    }

    template< typename T, typename U >
    sword::MagicOrder MakeOrder( const T& src, const U& mutator )
    {
        sword::MagicOrder order;
        *( order.*mutator )() = src;
        order.set_start_time( MakeNow() );
        return order;
    }

    void Send( const sword::MagicOrder& src )
    {
        client::MagicOrder msg;
        msg() = src;
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
}

void MagicOrderManager::Send( uint32_t id, int32_t code, const std::string& msg )
{
    auto it = orders_.find( id );
    if( it == orders_.end() )
        return;
    it->second.set_error_code( code );
    if( !msg.empty() )
        it->second.set_error_msg( msg );
    ::Send( it->second );
}

uint32_t MagicOrderManager::Register( const sword::MagicOrder& msg )
{
    ++ids_;
    auto it = orders_.insert( std::make_pair( ids_, msg ) );
    auto& order = it.first->second;
    order.set_id( ids_ );
    return ids_;
}

uint32_t MagicOrderManager::Register( const sword::MagicAction& msg )
{
    return Register( MakeOrder( msg, &sword::MagicOrder::mutable_magic_action ) );
}

uint32_t MagicOrderManager::Register( const sword::UnitMagicAction& msg )
{
    return Register( MakeOrder( msg, &sword::MagicOrder::mutable_unit_magic_action ) );
}

uint32_t MagicOrderManager::Register( const sword::ObjectMagicAction& msg )
{
    return Register( MakeOrder( msg, &sword::MagicOrder::mutable_object_magic_action ) );
}

uint32_t MagicOrderManager::Register( const sword::KnowledgeMagicAction& msg )
{
    return Register( MakeOrder( msg, &sword::MagicOrder::mutable_knowledge_magic_action ) );
}

uint32_t MagicOrderManager::Register( const sword::SetAutomatMode& msg )
{
    return Register( MakeOrder( msg, &sword::MagicOrder::mutable_set_automat_mode ) );
}

void MagicOrderManager::Unregister( uint32_t id )
{
    orders_.erase( id );
}

void MagicOrderManager::SendStateToNewClient()
{
    for( auto it = orders_.begin(); it != orders_.end(); ++it )
        ::Send( it->second );
}
