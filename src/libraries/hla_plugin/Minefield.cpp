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
void ReadWorldLocation( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, rpr::WorldLocation& loc )
{
    loc.Deserialize( deserializer );
}
void ReadPerimeter( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< rpr::PerimeterPoint >& loc, const rpr::WorldLocation& center )
{
    deserializer >> loc;
    geocoord::PlanarCartesian::Parameters params; params.SetOrigin( center.Latitude() * rPiOver180, center.Longitude() * rPiOver180 );
    std::vector< rpr::WorldLocation > wl;
    std::for_each( loc.begin(), loc.end(), [&](const rpr::PerimeterPoint& v)
        {
            geocoord::PlanarCartesian pc( v.X(), v.Y(), 0, params );
            geocoord::Geodetic geo( pc );
            wl.push_back( rpr::WorldLocation( geo.GetLatitude() / rPiOver180, geo.GetLongitude() / rPiOver180, 0 ) );
        });
    listener.GeometryChanged( identifier, wl, ObjectListener_ABC::eGeometryType_Polygon );
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
    attributes_->Register( "ForceIdentifier", boost::bind( &ReadForceIdentifier, _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributes_->Register( "MinefieldLocation", boost::bind( &ReadWorldLocation, _1, _2, _3, boost::ref( center_ ) ), center_);
    //attributes_->Register( "MinefieldOrientation" ); // FIXME AHC
    attributes_->Register( "MineTypes", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< std::vector< rpr::EntityType > >( mineTypes_ ) );
    attributes_->Register( "PerimeterPointCoordinates", boost::bind( &ReadPerimeter, _1, _2, _3, boost::ref( perimeter_ ), boost::cref( center_ ) ), Wrapper< std::vector< rpr::PerimeterPoint > >( perimeter_ ) );
    attributes_->Register( "ProtocolMode", boost::bind( &ReadNothing, _1, _2, _3 ), Wrapper< uint8_t >( 0 ) ); // HearbeatMode
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
    rpr::PerimeterPoint computePerimeter( const sword::CoordLatLong& v, const geocoord::PlanarCartesian::Parameters& params )
    {
        const geocoord::Geodetic pos( v.latitude() * rPiOver180, v.longitude() * rPiOver180, 0 );
        geocoord::PlanarCartesian pLoc( pos, params );
        return rpr::PerimeterPoint ( (float)pLoc.GetX(), (float)pLoc.GetY() );
    }
}

// -----------------------------------------------------------------------------
// Name: Minefield::SpatialChanged
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
void Minefield::SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

    double centerLat = std::accumulate( pos.begin(), pos.end(), 0., addLat ) / pos.size();
    double centerLong = std::accumulate( pos.begin(), pos.end(), 0., addLong ) / pos.size();
    center_ = rpr::WorldLocation( centerLat, centerLong, 0 );
    attributes_->Update( "MinefieldLocation", center_ );

    const geocoord::Geodetic center( centerLat * rPiOver180, centerLong * rPiOver180, 0 );
    perimeter_.resize( pos.size() );
    geocoord::PlanarCartesian::Parameters params; params.SetOrigin( centerLat * rPiOver180, centerLong * rPiOver180 );
    std::transform( pos.begin(), pos.end(), perimeter_.begin(), boost::bind( &computePerimeter, _1, boost::cref( params ) ) );
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
void Minefield::ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& res )
{
    mineTypes_ = res;
    attributes_->Update( "MineTypes",  Wrapper< std::vector< rpr::EntityType > >( mineTypes_ )  );
}

Agent_ABC* const Minefield::GetAgent() const
{
	return 0;
}
