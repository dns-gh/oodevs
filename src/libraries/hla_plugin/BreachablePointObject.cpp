// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "BreachablePointObject.h"
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
// Name: BreachablePointObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
BreachablePointObject::BreachablePointObject( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& type,
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
// Name: BreachablePointObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
BreachablePointObject::BreachablePointObject( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject destructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
BreachablePointObject::~BreachablePointObject()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::Serialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::Deserialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::GetIdentifier
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
const std::string& BreachablePointObject::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::Register
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::Unregister
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::RegisterAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::RegisterAttributes()
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
    attributes_->Register( "BreachedStatus", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint8_t >( 0 ) ); // NoBreaching
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::SpatialChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

    center_ = rpr::WorldLocation( pos[0].latitude(), pos[0].longitude(), 0 );
    attributes_->Update( "Location", center_ );
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::ResetAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::Attach
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreachablePointObject::ResourcesChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachablePointObject::ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}

Agent_ABC* const BreachablePointObject::GetAgent() const
{
    return 0;
}
