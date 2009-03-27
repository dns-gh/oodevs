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
#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( OccupantAttribute, "OccupantAttribute" )

// -----------------------------------------------------------------------------
// Name: OccupantAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
OccupantAttribute::OccupantAttribute()
{
    // NOTHING
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
OccupantAttribute& OccupantAttribute::operator=( const OccupantAttribute& /*rhs*/ )
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void OccupantAttribute::serialize
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive > 
void OccupantAttribute::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: OccupantAttribute::AddOccupant
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void OccupantAttribute::AddOccupant( const MIL_Agent_ABC& agent )
{
    if ( ! pOccupant_ )
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
    if ( pOccupant_ == &agent )
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