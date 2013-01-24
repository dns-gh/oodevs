// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "BreachableLinearObject.h"
#include "AttributesUpdater.h"
#include "ObjectListenerComposite.h"
#include "SerializationTools.h"
#include "TacticalObject_ABC.h"
#include "FOM_Serializer_ABC.h"

#pragma warning( push, 0 )
#include "protocol/proto/common.pb.h"
#pragma warning( pop )

#include <geocoord/Geodetic.h>
#include <geocoord/PlanarCartesian.h>
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
void ReadSegments( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, std::vector< rpr::BreachableSegmentStruct >& segments )
{
    segments.clear();
    deserializer >> segments;
}
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
BreachableLinearObject::BreachableLinearObject( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& type,
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
// Name: BreachableLinearObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
BreachableLinearObject::BreachableLinearObject( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject destructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
BreachableLinearObject::~BreachableLinearObject()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::Serialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::Deserialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::GetIdentifier
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
const std::string& BreachableLinearObject::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::Register
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::Unregister
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::RegisterAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::RegisterAttributes()
{
    attributes_->Register( "ObjectType", boost::bind( &ReadEntityType, _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributes_->Register( "ObjectIdentifier", boost::bind( &ReadEntityIdentifier, _1, _2, _3, boost::ref( entityIdentifier_ ) ), entityIdentifier_ );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier, _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributes_->Register( "SegmentRecords", boost::bind( &ReadSegments, _1, _2, _3, boost::ref( segments_ ) ), Wrapper< std::vector< rpr::BreachableSegmentStruct > >( segments_ ) );
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::SpatialChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::SpatialChanged( const TacticalObjectEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::ResetAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::Attach
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreachableLinearObject::ResourcesChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void BreachableLinearObject::ResourcesChanged( const TacticalObjectEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}
