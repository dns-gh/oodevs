// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAggregate.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "SilentEntity.h"
#include "rpr/EntityType.h"
#include "RemoteAgentListener_ABC.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer.h>
#include <boost/bind.hpp>
#include <boost/assign.hpp>

using namespace plugins::hla;

namespace
{
    void ReadSpatial( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        Spatial spatial;
        spatial.Deserialize( deserializer );
        listener.Moved( identifier, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
    }
    void ReadForceIdentifier( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        int8 force;
        deserializer >> force;
        listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( force ) );
    }
    void ReadAggregateMarking( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        AggregateMarking marking;
        marking.Deserialize( deserializer );
        listener.NameChanged( identifier, marking.str() );
    }
    void ReadEntityType( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener )
    {
        rpr::EntityType type;
        type.Deserialize( deserializer );
        listener.TypeChanged( identifier, type );
    }
    void ReadNumberOfSilentEntities( ::hla::Deserializer& deserializer, const std::string& /*identifier*/, RemoteAgentListener_ABC& /*listener*/, unsigned int& numberOfSilentEntities )
    {
        uint16 number = 0;
        deserializer >> number;
        numberOfSilentEntities = number;
    }
    void ReadSilentEntities( ::hla::Deserializer& deserializer, const std::string& identifier, RemoteAgentListener_ABC& listener, unsigned int numberOfSilentEntities )
    {
        for( unsigned int i = 0; i < numberOfSilentEntities; ++i )
        {
            SilentEntity entity;
            entity.Deserialize( deserializer );
            listener.EquipmentUpdated( identifier, entity.entityType_, entity.numberOfEntitiesOfThisType_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregate::RemoteAggregate( const std::string& identifier, RemoteAgentListener_ABC& listener )
    : identifier_            ( identifier )
    , listener_              ( listener )
    , numberOfSilentEntities_( 0 )
    , notifications_         ( boost::assign::list_of< std::pair< std::string, T_Notification > >
                              ( "Spatial"               , boost::bind( &ReadSpatial               , _1, _2, _3 ) )
                              ( "ForceIdentifier"       , boost::bind( &ReadForceIdentifier       , _1, _2, _3 ) )
                              ( "AggregateMarking"      , boost::bind( &ReadAggregateMarking      , _1, _2, _3 ) )
                              ( "EntityType"            , boost::bind( &ReadEntityType            , _1, _2, _3 ) )
                              ( "NumberOfSilentEntities", boost::bind( &ReadNumberOfSilentEntities, _1, _2, _3, boost::ref( numberOfSilentEntities_ ) ) )
                              ( "SilentEntities"        , boost::bind( &ReadSilentEntities        , _1, _2, _3, boost::ref( numberOfSilentEntities_ ) ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteAggregate::~RemoteAggregate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAggregate::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "RPR remote aggregate can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: RemoteAggregate::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteAggregate::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer deserializer )
{
    T_Notifications::const_iterator notification = notifications_.find( identifier.ToString() );
    if( notification != notifications_.end() )
        notification->second( deserializer, identifier_, listener_ );
}
