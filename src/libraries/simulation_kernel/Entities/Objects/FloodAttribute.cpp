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
#include "MIL_Object_ABC.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( FloodAttribute )

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute()
    : depth_  ( 0 )
    , refDist_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( xml::xistream& xis, const TER_Localisation& objectLocation )
    : depth_   ( xis.attribute< int >( "depth" ) )
    , refDist_ ( xis.attribute< int >( "reference-distance" ) )
    , location_( objectLocation.ComputeBarycenter(), refDist_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( const sword::MissionParameter_Value& attributes, const TER_Localisation& objectLocation )
    : depth_   ( attributes.list( 1 ).quantity() )
    , refDist_ ( attributes.list( 2 ).quantity() )
    , location_( objectLocation.ComputeBarycenter(), refDist_ )
{
    // NOTHING
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
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::serialize
// Created: JSR 2010-12-15
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
// Name: FloodAttribute::SendFullState
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_flood()->set_depth( depth_ );
    asn.mutable_flood()->set_reference_distance( refDist_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetLocalisation
// Created: JSR 2010-12-16
// -----------------------------------------------------------------------------
const TER_Localisation& FloodAttribute::GetLocalisation() const
{
    return location_;
}
