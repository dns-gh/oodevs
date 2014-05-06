// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ImprovableCapacity.h"
#include "MineAttribute.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ImprovableCapacity )

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::ImprovableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::ImprovableCapacity( const PHY_ConsumptionType& consumption, ConstructionCapacity::E_UnitType type, xml::xistream& xis )
    : SuppliableCapacity( consumption, type, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::ImprovableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::ImprovableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::ImprovableCapacity( const ImprovableCapacity& from )
    : SuppliableCapacity( from )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ImprovableCapacity::~ImprovableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< SuppliableCapacity >( *this );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< SuppliableCapacity >( *this );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ImprovableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ImprovableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new ImprovableCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ImprovableCapacity::Mine
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void ImprovableCapacity::Mine( MIL_Object_ABC& object )
{
    if( object.IsMarkedForDestruction() )
        return;
    const double rNewMiningPercentage = 1.;
    const double rDeltaPercentage = rNewMiningPercentage - object.GetAttribute< MineAttribute >().GetState();
    if( rDeltaPercentage == 0 )
        return;
    object.GetAttribute< MineAttribute >().Update( rDeltaPercentage );
}
