// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BurnSurfaceCapacity.h"
#include "FireAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "MIL_Object_ABC.h"
#include "MIL_BurningCells.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BurnSurfaceCapacity )

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::BurnSurfaceCapacity
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
BurnSurfaceCapacity::BurnSurfaceCapacity( xml::xistream& /*xis*/ )
    : burningCells_( MIL_AgentServer::GetWorkspace().GetBurningCells() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::BurnSurfaceCapacity
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
BurnSurfaceCapacity::BurnSurfaceCapacity()
    : burningCells_( MIL_AgentServer::GetWorkspace().GetBurningCells() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity constructor
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
BurnSurfaceCapacity::BurnSurfaceCapacity( const BurnSurfaceCapacity& other )
    : burningCells_( other.burningCells_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity destructor
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
BurnSurfaceCapacity::~BurnSurfaceCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::serialize
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
template< typename Archive >
void BurnSurfaceCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::Register
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::Instanciate
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    BurnSurfaceCapacity* pBurnSurfaceCapacity = new BurnSurfaceCapacity( *this );
    object.AddCapacity( pBurnSurfaceCapacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( pBurnSurfaceCapacity ) );
    object.SetAttribute< BurnSurfaceAttribute, BurnSurfaceAttribute >( BurnSurfaceAttribute( &object ) );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::Finalize
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::Finalize( MIL_Object_ABC& object )
{
    burningCells_.StartBurn( object );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::Update
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::Update( MIL_Object_ABC& object, unsigned int time )
{
    burningCells_.Update( object, time );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::ProcessAgentMovingInside
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    burningCells_.BurnAgent( object, agent );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::ProcessAgentInside
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    burningCells_.BurnAgent( object, agent );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceCapacity::ProcessPopulationInside
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
void BurnSurfaceCapacity::ProcessPopulationInside( MIL_Object_ABC&, MIL_PopulationElement_ABC& agent )
{
    burningCells_.BurnPopulation( agent );
}
