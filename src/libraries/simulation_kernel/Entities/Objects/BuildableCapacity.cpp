// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BuildableCapacity.h"
#include "ConstructionAttribute.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BuildableCapacity )

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::BuildableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity( const BuildableCapacity& from )
    : SuppliableCapacity( from )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity constructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
BuildableCapacity::BuildableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis )
    : SuppliableCapacity( consumption, type, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BuildableCapacity::~BuildableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< SuppliableCapacity >( *this );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< SuppliableCapacity >( *this );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BuildableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BuildableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new BuildableCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Construct
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::Construct( MIL_Object_ABC& object )
{
    ChangeConstructionPercentage( object, 1. );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Construct
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void BuildableCapacity::Construct( MIL_Object_ABC& object, float rDeltaPercentage )
{
    object.GetAttribute< ConstructionAttribute >().Build( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::Destroy( MIL_Object_ABC& object )
{
    ChangeConstructionPercentage( object, 0. );
}

// -----------------------------------------------------------------------------
// Name: BuildableCapacity::ChangeConstructionPercentage
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void BuildableCapacity::ChangeConstructionPercentage( MIL_Object_ABC& object, float rNewConstructionPercentage )
{
    if( object.IsMarkedForDestruction() )
        return;
    const float state = static_cast< float >( object.GetAttribute< ConstructionAttribute >().GetState() );
    if( state == rNewConstructionPercentage )
        return;
    if( rNewConstructionPercentage == 0 )
        Construct( object, -1 );
    else if( rNewConstructionPercentage == 1 )
        Construct( object, 1 );
    else
        Construct( object, rNewConstructionPercentage - state );
}
