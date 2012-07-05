// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteSurfaceVessel.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "ObjectListenerComposite.h"
#include "AttributesDeserializer.h"
#include "rpr/EntityType.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadSpatial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        Spatial spatial;
        spatial.Deserialize( deserializer );
        listener.Moved( identifier, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
    }
    void ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        int8 force;
        deserializer >> force;
        listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( force ) );
    }
    void ReadMarking( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        Marking marking;
        marking.Deserialize( deserializer );
        listener.NameChanged( identifier, marking.str() );
    }
    void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener )
    {
        rpr::EntityType type;
        type.Deserialize( deserializer );
        listener.TypeChanged( identifier, type );
        listener.EquipmentUpdated( identifier, type, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteSurfaceVessel::RemoteSurfaceVessel( const std::string& identifier )
    : identifier_( identifier )
    , listeners_ ( new ObjectListenerComposite() )
    , attributes_( new AttributesDeserializer( identifier, *listeners_ ) )
{
    attributes_->Register( "Spatial"        , boost::bind( &ReadSpatial         , _1, _2, _3 ) );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier , _1, _2, _3 ) );
    attributes_->Register( "Marking"        , boost::bind( &ReadMarking         , _1, _2, _3 ) );
    attributes_->Register( "EntityType"     , boost::bind( &ReadEntityType      , _1, _2, _3 ) );
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteSurfaceVessel::~RemoteSurfaceVessel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::Serialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::Serialize( ::hla::UpdateFunctor_ABC& /*functor*/, bool /*updateAll*/ ) const
{
    throw std::runtime_error( "RPR remote surface vessel can not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::Deserialize
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::GetIdentifier
// Created: AHC 2012-07-05
// -----------------------------------------------------------------------------
const std::string& RemoteSurfaceVessel::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::SetIdentifier
// Created: AHC 2012-07-05
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::SetIdentifier( const std::string& id )
{
    identifier_ = id;
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: RemoteSurfaceVessel::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void RemoteSurfaceVessel::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener ) ;
}
