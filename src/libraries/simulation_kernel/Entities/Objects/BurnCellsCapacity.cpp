// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BurnCellsCapacity.h"
#include "FireAttribute.h"
#include "MIL_Object_ABC.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( BurnCellsCapacity )

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::BurnCellsCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCellsCapacity::BurnCellsCapacity( xml::xistream& xis )
{
    InitializeData( xis );
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::BurnCellsCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCellsCapacity::BurnCellsCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCellsCapacity::~BurnCellsCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCellsCapacity::BurnCellsCapacity( const BurnCellsCapacity& /*other*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::InitializeSpeed
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
void BurnCellsCapacity::InitializeData( xml::xistream& /*xis*/ )
{
    int todo = 0;
    //xis >> xml::attribute( "injury-id", injuryID_ );
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void BurnCellsCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void BurnCellsCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void BurnCellsCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    BurnCellsCapacity* pBurnCellsCapacity = new BurnCellsCapacity( *this );
    object.AddCapacity( pBurnCellsCapacity );
    object.SetAttribute< FireAttribute, FireAttribute >( FireAttribute() );
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::Finalize
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnCellsCapacity::Finalize( MIL_Object_ABC& object )
{
    // $$$$ BCI 2010-12-17: todo
}

// -----------------------------------------------------------------------------
// Name: BurnCellsCapacity::Update
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnCellsCapacity::Update( MIL_Object_ABC& object, unsigned int time )
{
     // $$$$ BCI 2010-12-17: todo
}
