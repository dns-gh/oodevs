// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ActionManager.h"
#include "MIL_Time_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

namespace bip = boost::interprocess;

namespace boost {
namespace serialization {
template< class Archive >
void save( Archive& ar, const sword::Action& src, const unsigned int )
{
    bip::basic_vectorstream< std::vector< char > > stream;
    src.SerializeToOstream( &stream );
    ar << stream.vector();
}

template< class Archive >
void load( Archive& ar, sword::Action& dst, const unsigned int )
{
    std::vector< char > data;
    ar >> data;
    bip::ibufferstream stream( &data[0], data.size() );
    dst.ParseFromIstream( &stream );
}
}
}

BOOST_SERIALIZATION_SPLIT_FREE( sword::Action );

BOOST_CLASS_EXPORT_IMPLEMENT( ActionManager );

ActionManager::ActionManager()
    : ids_( 0 )
{
    // NOTHING
}

ActionManager::~ActionManager()
{
    // NOTHING
}

template< typename Archive >
void ActionManager::serialize( Archive& ar, const unsigned int )
{
    ar & ids_ & actions_ & orders_;
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
    sword::Action MakeAction( const T& src, const U& mutator )
    {
        sword::Action action;
        *( action.*mutator )() = src;
        action.set_start_time( MakeNow() );
        return action;
    }

    void Send( const sword::Action& src )
    {
        client::Action action;
        action() = src;
        action.Send( NET_Publisher_ABC::Publisher() );
    }
}

void ActionManager::Send( uint32_t id, int32_t code, const std::string& msg )
{
    auto it = actions_.find( id );
    if( it == actions_.end() )
        return;
    it->second.set_error_code( code );
    if( !msg.empty() )
        it->second.set_error_msg( msg );
    ::Send( it->second );
}

uint32_t ActionManager::Register( const sword::Action& msg )
{
    ++ids_;
    auto it = actions_.insert( std::make_pair( ids_, msg ) );
    auto& action = it.first->second;
    action.set_id( ids_ );
    return ids_;
}

uint32_t ActionManager::Register( const sword::MagicAction& msg )
{
    return Register( MakeAction( msg, &sword::Action::mutable_magic_action ) );
}

uint32_t ActionManager::Register( const sword::UnitMagicAction& msg )
{
    return Register( MakeAction( msg, &sword::Action::mutable_unit_magic_action ) );
}

uint32_t ActionManager::Register( const sword::ObjectMagicAction& msg )
{
    return Register( MakeAction( msg, &sword::Action::mutable_object_magic_action ) );
}

uint32_t ActionManager::Register( const sword::KnowledgeMagicAction& msg )
{
    return Register( MakeAction( msg, &sword::Action::mutable_knowledge_magic_action ) );
}

uint32_t ActionManager::Register( const sword::SetAutomatMode& msg )
{
    return Register( MakeAction( msg, &sword::Action::mutable_set_automat_mode ) );
}

namespace
{
    ActionManager::Order MakeOrder( bool created, uint32_t id )
    {
        ActionManager::Order rpy = { created, id };
        return rpy;
    }
}

ActionManager::Order ActionManager::RegisterOrder( uint32_t order, const sword::Action& msg )
{
    auto it = orders_.find( order );
    if( it != orders_.end() )
        return MakeOrder( false, it->second );
    const uint32_t id = Register( msg );
    orders_.insert( std::make_pair( order, id ) );
    return MakeOrder( true, id );
}

ActionManager::Order ActionManager::Register( const sword::UnitOrder& msg )
{
    return RegisterOrder( msg.id(), MakeAction( msg, &sword::Action::mutable_unit_order ) );
}

ActionManager::Order ActionManager::Register( const sword::AutomatOrder& msg )
{
    return RegisterOrder( msg.id(), MakeAction( msg, &sword::Action::mutable_automat_order ) );
}

ActionManager::Order ActionManager::Register( const sword::CrowdOrder& msg )
{
    return RegisterOrder( msg.id(), MakeAction( msg, &sword::Action::mutable_crowd_order ) );
}

uint32_t ActionManager::Register( const sword::FragOrder& msg )
{
    return Register( MakeAction( msg, &sword::Action::mutable_frag_order ) );
}

void ActionManager::Unregister( uint32_t id )
{
    orders_.erase( id );
}

void ActionManager::SendStateToNewClient()
{
    for( auto it = actions_.begin(); it != actions_.end(); ++it )
        ::Send( it->second );
}
