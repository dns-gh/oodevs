// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FirePropagationModifierCapacity.h"
#include "FireAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "MIL_Object_ABC.h"
#include "MIL_BurningCells.h"
#include "MIL_AgentServer.h"
#include "MIL_FireClass.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( FirePropagationModifierCapacity )

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::FirePropagationModifierCapacity
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
FirePropagationModifierCapacity::FirePropagationModifierCapacity( xml::xistream& xis )
    : burningCells_( MIL_AgentServer::GetWorkspace().GetBurningCells() )
    , pObject_( 0 )
{
    xis >> xml::list( "modifier", *this, &FirePropagationModifierCapacity::ReadModifier );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::FirePropagationModifierCapacity
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
FirePropagationModifierCapacity::FirePropagationModifierCapacity()
: burningCells_( MIL_AgentServer::GetWorkspace().GetBurningCells() )
, pObject_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity destructor
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
FirePropagationModifierCapacity::~FirePropagationModifierCapacity()
{
    if( pObject_ )
        burningCells_.StopModifyBurn( *pObject_ );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity constructor
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
FirePropagationModifierCapacity::FirePropagationModifierCapacity( const FirePropagationModifierCapacity& other )
    : burningCells_( other.burningCells_ )
    , modifiers_( other.modifiers_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::serialize
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
template< typename Archive >
void FirePropagationModifierCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::Register
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void FirePropagationModifierCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::Instanciate
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void FirePropagationModifierCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    FirePropagationModifierCapacity* pFirePropagationModifierCapacity = new FirePropagationModifierCapacity( *this );
    object.AddCapacity( pFirePropagationModifierCapacity );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::Finalize
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void FirePropagationModifierCapacity::Finalize( MIL_Object_ABC& object )
{
    pObject_ = &object;
    burningCells_.StartModifyBurn( object );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::ReadModifier
// Created: BCI 2011-01-11
// -----------------------------------------------------------------------------
void FirePropagationModifierCapacity::ReadModifier( xml::xistream& xis )
{
    std::string className( xml::attribute( xis, "fire-class", std::string() ) );
    const MIL_FireClass* pClass = MIL_FireClass::Find( className );
    if( !pClass )
        xis.error( "Unknown 'Fire class' '" + className + "' for fire propagation modifier capacity" );
    Modifier& modifier = modifiers_[ pClass ];
    xis >> xml::attribute( "ignition-threshold", modifier.ignitionThreshold_ )
        >> xml::attribute( "max-combustion-energy", modifier.maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: FirePropagationModifierCapacity::Modify
// Created: BCI 2011-01-11
// -----------------------------------------------------------------------------
void FirePropagationModifierCapacity::Modify( const MIL_FireClass& fireClass, int& ignitionThreshold, int& maxCombustionEnergy ) const
{
    ModifierMap::const_iterator it = modifiers_.find( &fireClass );
    if( it != modifiers_.end() )
    {
        ignitionThreshold = std::max( ignitionThreshold, it->second.ignitionThreshold_ );
        maxCombustionEnergy = std::min( maxCombustionEnergy, it->second.maxCombustionEnergy_ );
    }
}
