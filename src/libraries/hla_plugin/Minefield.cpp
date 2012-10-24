// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Minefield.h"
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
    int8 tmpForce;
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
void ReadPerimeter( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< rpr::PerimeterPoint >& loc )
{
    deserializer >> loc;
    listener.PerimeterChanged( identifier, loc );
}
}

// -----------------------------------------------------------------------------
// Name: Minefield constructor
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
Minefield::Minefield( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier force, const rpr::EntityType& type,
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
// Name: Minefield constructor
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
Minefield::Minefield( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: Minefield destructor
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
Minefield::~Minefield()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Minefield::Serialize
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void Minefield::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: Minefield::Deserialize
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void Minefield::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: Minefield::GetIdentifier
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
const std::string& Minefield::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: Minefield::Register
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void Minefield::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: Minefield::Unregister
// Created: AHC 2012-08-07
// -----------------------------------------------------------------------------
void Minefield::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: Minefield::RegisterAttributes
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void Minefield::RegisterAttributes()
{
    attributes_->Register( "MinefieldType", boost::bind( &ReadEntityType, _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributes_->Register( "MinefieldIdentifier", boost::bind( &ReadEntityIdentifier, _1, _2, _3, boost::ref( entityIdentifier_ ) ), entityIdentifier_ );
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier, _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8 >( static_cast< int8 >( force_ ) ) );
    attributes_->Register( "MinefieldLocation", boost::bind( &ReadWorldLocation, _1, _2, _3, boost::ref( center_ ) ), center_);
    //attributes_->Register( "MinefieldOrientation" ); // FIXME AHC
    attributes_->Register( "MineTypes", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< std::vector< rpr::EntityType > >( mineTypes_ ) );
    attributes_->Register( "PerimeterPointCoordinates", boost::bind( &ReadPerimeter, _1, _2, _3, boost::ref( perimeter_ ) ), Wrapper< std::vector< rpr::PerimeterPoint > >( perimeter_ ) );
    attributes_->Register( "ProtocolMode", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint8 >( 0 ) ); // HearbeatMode
    attributes_->Register( "State", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( false ) ); // FIXME AHC
    attributes_->Register( "ActiveStatus", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< bool >( true ) ); // FIXME AHC
}

namespace
{
    double addLat( double v, const sword::CoordLatLong& l )
    {
        return v + l.latitude();
    }
    double addLong( double v, const sword::CoordLatLong& l )
    {
        return v + l.longitude();
    }
    rpr::PerimeterPoint computePerimeter( const sword::CoordLatLong& v, double cx, double cy )
    {
        rpr::WorldLocation loc( v.latitude(), v.longitude(), 0 );
        return rpr::PerimeterPoint( static_cast< float >( loc.X() - cx ), static_cast< float >( loc.Y() - cy ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Minefield::SpatialChanged
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
void Minefield::SpatialChanged( const TacticalObjectEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

    double centerLat = std::accumulate( pos.begin(), pos.end(), 0., addLat );
    double centerLong = std::accumulate( pos.begin(), pos.end(), 0., addLong );
    center_ = rpr::WorldLocation( centerLat/pos.size(), centerLong/pos.size(), 0 );
    attributes_->Update( "MinefieldLocation", center_ );

    perimeter_.resize( pos.size() );
    std::transform( pos.begin(), pos.end(), perimeter_.begin(), boost::bind( &computePerimeter, _1, center_.X(), center_.Y() ) );
    attributes_->Update( "PerimeterPointCoordinates", Wrapper< std::vector< rpr::PerimeterPoint > >( perimeter_ ) );
}

// -----------------------------------------------------------------------------
// Name: Minefield::ResetAttributes
// Created: AHC 2012-08-30
// -----------------------------------------------------------------------------
void Minefield::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Minefield::Attach
// Created: AHC 2012-08-30
// -----------------------------------------------------------------------------
void Minefield::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Minefield::ResourcesChanged
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void Minefield::ResourcesChanged( const TacticalObjectEventListener_ABC::T_ResourceVector& res )
{
    mineTypes_ = res;
    attributes_->Update( "MineTypes",  Wrapper< std::vector< rpr::EntityType > >( mineTypes_ )  );
}
