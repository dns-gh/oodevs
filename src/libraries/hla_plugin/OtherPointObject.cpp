// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "OtherPointObject.h"
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

static const double rPiOver180 = std::acos( -1. ) / 180.;

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
void ReadWorldLocation( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::WorldLocation& loc )
{
    loc.Deserialize( deserializer );
    listener.Moved( identifier, loc.Latitude(), loc.Longitude() );
}
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
OtherPointObject::OtherPointObject( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& type,
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
// Name: OtherPointObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
OtherPointObject::OtherPointObject( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject destructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
OtherPointObject::~OtherPointObject()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::Serialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::Deserialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::GetIdentifier
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
const std::string& OtherPointObject::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::Register
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::Unregister
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::RegisterAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::RegisterAttributes()
{
    attributes_->Register( "ObjectType", boost::bind( &ReadEntityType, _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributes_->Register( "ObjectIdentifier", boost::bind( &ReadEntityIdentifier, _1, _2, _3, boost::ref( entityIdentifier_ ) ), entityIdentifier_ );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier, _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributes_->Register( "Location", boost::bind( &ReadWorldLocation, _1, _2, _3, boost::ref( center_ ) ), center_);
    attributes_->Register( "Orientation", boost::bind( &ReadNothing, _1, _2, _3 ), rpr::Orientation() );
    attributes_->Register( "Flaming", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "Smoking", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "Deactivated", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "ObjectPreDistributed", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "PercentComplete", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint32_t >( 100u ) );
    attributes_->Register( "DamagedAppearance", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint32_t >( 0 ) ); // NoDamage
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::SpatialChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

    center_ = rpr::WorldLocation( pos[0].latitude(), pos[0].longitude(), 0 );
    attributes_->Update( "Location", center_ );
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::ResetAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::Attach
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OtherPointObject::ResourcesChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherPointObject::ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}

Agent_ABC* const OtherPointObject::GetAgent() const
{
	return 0;
}
