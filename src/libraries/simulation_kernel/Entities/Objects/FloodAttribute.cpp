// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodAttribute.h"
#include "MIL_AgentServer.h"
#include "MIL_Object_ABC.h"
#include "flood/FloodModel.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( FloodAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< FloodAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< FloodAttribute > )

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute()
    : floodModel_ ( new flood::FloodModel( *this ) )
    , depth_      ( 0 )
    , refDist_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( xml::xistream& xis, const TER_Localisation& objectLocation )
    : floodModel_ ( new flood::FloodModel( *this ) )
    , depth_      ( xis.attribute< int >( "depth" ) )
    , refDist_    ( xis.attribute< int >( "reference-distance" ) )
    , location_   ( objectLocation.ComputeBarycenter(), refDist_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( const sword::MissionParameter_Value& attributes, const TER_Localisation& objectLocation )
    : floodModel_ ( new flood::FloodModel( *this ) )
    , depth_      ( attributes.list( 1 ).quantity() )
    , refDist_    ( attributes.list( 2 ).quantity() )
    , location_   ( objectLocation.ComputeBarycenter(), refDist_ )
{
    GenerateFlood();
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute destructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::~FloodAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::operator=
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodAttribute& FloodAttribute::operator=( const FloodAttribute& from )
{
    depth_ = from.depth_;
    refDist_ = from.refDist_;
    location_.Reset( from.location_ );
    *floodModel_ = *from.floodModel_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Update
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
bool FloodAttribute::Update( const FloodAttribute& rhs )
{
    if( depth_ != rhs.depth_ || refDist_ != rhs.refDist_ || !( location_ == rhs.location_ ) )
    {
        NotifyAttributeUpdated( eOnUpdate );
        depth_ = rhs.depth_;
        refDist_ = rhs.refDist_;
        location_.Reset( rhs.location_ );
        *floodModel_ = *rhs.floodModel_;
    }
    return NeedUpdate( eOnUpdate );
}


// -----------------------------------------------------------------------------
// Name: FloodAttribute::serialize
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
template< typename Archive >
void FloodAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & depth_
         & refDist_
         & location_;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Register
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< FloodAttribute, FloodAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Instanciate
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
void FloodAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< FloodAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetElevationAt
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
short FloodAttribute::GetElevationAt( const Point2f& point ) const
{
    return static_cast< short >( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( point.X(), point.Y() ) );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::WriteODB
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "flood" )
            << xml::attribute( "depth", depth_ )
            << xml::attribute( "reference-distance", refDist_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::OnUpdate
// Created: JSR 2011-09-06
// -----------------------------------------------------------------------------
void FloodAttribute::OnUpdate( const sword::MissionParameter_Value& /*attribute*/ )
{
    GenerateFlood( true );
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::SendFullState
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_flood()->set_depth( depth_ );
    asn.mutable_flood()->set_reference_distance( refDist_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::SendUpdate
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
bool FloodAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GenerateFlood
// Created: JSR 2011-05-20
// -----------------------------------------------------------------------------
void FloodAttribute::GenerateFlood( bool force )
{
    MT_Vector2D center = location_.ComputeBarycenter();
    floodModel_->GenerateFlood( Point2d( center.rX_, center.rY_ ), depth_, refDist_, force );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetLocalisation
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
const TER_Localisation& FloodAttribute::GetLocalisation() const
{
    // TODO : ne doit être que le polygone englobant l'inondation? (ou des bounding box éventuellement)
    return location_;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetDeepAreas
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
const std::vector< Polygon2f* >& FloodAttribute::GetDeepAreas() const
{
    return floodModel_->GetDeepAreas();
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetLowAreas
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
const std::vector< Polygon2f* >& FloodAttribute::GetLowAreas() const
{
    return floodModel_->GetLowAreas();
}
