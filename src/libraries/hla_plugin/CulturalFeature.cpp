// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "CulturalFeature.h"
#include "AttributesUpdater.h"
#include "ObjectListenerComposite.h"
#include "SerializationTools.h"
#include "TacticalObject_ABC.h"
#include "FOM_Serializer_ABC.h"

#pragma warning( push, 0 )
#include "protocol/proto/common.pb.h"
#pragma warning( pop )

#include <hla/Deserializer_ABC.h>
#include <hla/Serializer_ABC.h>
#include <hla/AttributeIdentifier.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <numeric>

using namespace plugins::hla;

namespace
{

void ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::ForceIdentifier& force )
{
    int8_t tmpForce;
    deserializer >> tmpForce;
    listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( tmpForce ) );
    force = static_cast< rpr::ForceIdentifier >( tmpForce );
}
void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityType& type )
{
    type.Deserialize( deserializer );
    listener.TypeChanged( identifier, type );
}
void ReadEntityIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, rpr::EntityIdentifier& entityId )
{
    entityId.Deserialize( deserializer );
}
void ReadNothing( ::hla::Deserializer_ABC& /*deserializer*/, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/ )
{
    // NOTHING
}
void ReadSpatial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, Spatial& spatial )
{
    spatial.Deserialize( deserializer );
    listener.Moved( identifier, spatial.worldLocation_.Latitude(), spatial.worldLocation_.Longitude() );
}
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature constructor
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
CulturalFeature::CulturalFeature( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& type,
        const rpr::EntityIdentifier& entityId, const std::string& rtiId )
    : object_( &object )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( rtiId )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
    , type_( type )
    , entityIdentifier_( entityId )
    , force_( force )
{
    RegisterAttributes();
    object_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature constructor
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
CulturalFeature::CulturalFeature( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature destructor
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
CulturalFeature::~CulturalFeature()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::Serialize
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::Deserialize
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::GetIdentifier
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
const std::string& CulturalFeature::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::Register
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::Unregister
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::RegisterAttributes
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::RegisterAttributes()
{
    attributes_->Register( "EntityType", boost::bind( &ReadEntityType, _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributes_->Register( "EntityIdentifier", boost::bind( &ReadEntityIdentifier, _1, _2, _3, boost::ref( entityIdentifier_ ) ), entityIdentifier_ );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier, _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributes_->Register( "Spatial", boost::bind( &ReadSpatial, _1, _2, _3, boost::ref( spatial_ ) ), spatial_);
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::SpatialChanged
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

    spatial_.Refresh( true, pos[0].latitude(), pos[0].longitude(), 0, 0, 0);
    attributes_->Update( "Spatial", spatial_ );
    listeners_->Moved( identifier_, pos[0].latitude(), pos[0].longitude() );
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::ResetAttributes
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::Attach
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CulturalFeature::ResourcesChanged
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
void CulturalFeature::ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}

Agent_ABC* const CulturalFeature::GetAgent() const
{
    return 0;
}
