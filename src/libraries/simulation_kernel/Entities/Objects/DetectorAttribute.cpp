// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DetectorAttribute.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DetectorAttribute )

template< typename Archive >
void save_construct_data( Archive& archive, const DetectorAttribute* attr, const unsigned int /*version*/ )
{
    const MIL_Agent_ABC* const pion = attr->detector_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, DetectorAttribute* attr, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( attr )DetectorAttribute( pion );
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute constructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
DetectorAttribute::DetectorAttribute()
    : detector_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute constructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
DetectorAttribute::DetectorAttribute( const MIL_Agent_ABC* agent )
    : detector_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute destructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
DetectorAttribute::~DetectorAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute::serialize
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
template< typename Archive >
void DetectorAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void DetectorAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< DetectorAttribute, DetectorAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute::AddAnimator
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void DetectorAttribute::AddDetector( const MIL_Agent_ABC& agent )
{
    if( detector_ )
        throw( std::exception( "Detector already registered" ) );
    detector_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute::GetAnimators
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
const MIL_Agent_ABC* DetectorAttribute::GetDetector() const
{
    return detector_;
}
