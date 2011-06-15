// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "OccupantAttribute.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( OccupantAttribute )

template< typename Archive >
void save_construct_data( Archive& archive, const OccupantAttribute* attr, const unsigned int /*version*/ )
{
    const MIL_Agent_ABC* const pion = attr->pOccupant_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, OccupantAttribute* attr, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( attr )OccupantAttribute( pion );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute()
    : pOccupant_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: JSR 2011-06-15
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute( const MIL_Agent_ABC* pOccupant )
    : pOccupant_( pOccupant )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute( xml::xistream& /*xis*/ )
    : pOccupant_( 0 )
{
    // NOTHING @TODO MGD manage other size than 1
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
OccupantAttribute::~OccupantAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
OccupantAttribute& OccupantAttribute::operator=( const OccupantAttribute& rhs )
{
    pOccupant_ = rhs.pOccupant_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::serialize
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive >
void OccupantAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    // $$$$ _RC_ SBO 2009-06-10: what about occupant?
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void OccupantAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< OccupantAttribute, OccupantAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::AddOccupant
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void OccupantAttribute::AddOccupant( const MIL_Agent_ABC& agent )
{
    if( ! pOccupant_ || pOccupant_ == &agent )
        pOccupant_ = &agent;
    else
        throw std::exception( "try to use an object already occupied" );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::ReleaseOccupant
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void OccupantAttribute::ReleaseOccupant( const MIL_Agent_ABC& agent )
{
    if( pOccupant_ == &agent )
        pOccupant_ = 0;
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::GetOccupant
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
const MIL_Agent_ABC* OccupantAttribute::GetOccupant() const
{
    return pOccupant_;
}