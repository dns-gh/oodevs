// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "OtherArealObject.h"
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
    force = static_cast< rpr::ForceIdentifier >( tmpForce );
    listener.SideChanged( identifier, force );
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
void ReadPoints( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< rpr::WorldLocation >& loc )
{
    loc.clear();
    deserializer >> loc;
    listener.GeometryChanged( identifier, loc, ObjectListener_ABC::eGeometryType_Polygon );
}
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
OtherArealObject::OtherArealObject( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& type,
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
// Name: OtherArealObject constructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
OtherArealObject::OtherArealObject( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject destructor
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
OtherArealObject::~OtherArealObject()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::Serialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::Deserialize
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::GetIdentifier
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
const std::string& OtherArealObject::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::Register
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::Unregister
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::RegisterAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::RegisterAttributes()
{
    attributes_->Register( "ObjectType", boost::bind( &ReadEntityType, _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributes_->Register( "ObjectIdentifier", boost::bind( &ReadEntityIdentifier, _1, _2, _3, boost::ref( entityIdentifier_ ) ), entityIdentifier_ );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier, _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributes_->Register( "PointsData", boost::bind( &ReadPoints, _1, _2, _3, boost::ref( points_ ) ), Wrapper< std::vector< rpr::WorldLocation > >( points_ ) );
    attributes_->Register( "Flaming", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "Smoking", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "Deactivated", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "ObjectPreDistributed", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) );
    attributes_->Register( "PercentComplete", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint32_t >( 100u ) );
    attributes_->Register( "DamagedAppearance", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint32_t >( 0 ) ); // NoDamage
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::SpatialChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

    points_.clear();
    std::for_each( pos.begin(), pos.end(), [&](ObjectLocationEventListener_ABC::T_PositionVector::const_reference v)
        {
            rpr::WorldLocation wl( v.latitude(), v.longitude(), 0 );
            points_.push_back( wl );
        });
    attributes_->Update( "PointsData", Wrapper< std::vector< rpr::WorldLocation > >( points_ ) );
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::ResetAttributes
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::Attach
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OtherArealObject::ResourcesChanged
// Created: AHC 2013-01-22
// -----------------------------------------------------------------------------
void OtherArealObject::ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}

Agent_ABC* const OtherArealObject::GetAgent() const
{
    return 0;
}
