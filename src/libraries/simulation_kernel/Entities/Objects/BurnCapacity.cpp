// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BurnCapacity.h"
#include "FireAttribute.h"
#include "BurnAttribute.h"
#include "MIL_FireClass.h"
#include "UrbanDestructionCapacity.h"
#include "MIL_Object_ABC.h"
#include "Entities\Agents\MIL_Agent_ABC.h"
#include "Entities\Agents\Roles\Composantes\PHY_RoleInterface_Composantes.h"
#include "Entities\Populations\MIL_PopulationElement_ABC.h"
#include "Entities\Agents\Units\Humans\MIL_Injury_Fire.h"

#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( BurnCapacity )

// -----------------------------------------------------------------------------
// Name: BurnCapacity::BurnCapacity
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCapacity::BurnCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity destructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCapacity::~BurnCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity constructor
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
BurnCapacity::BurnCapacity( const BurnCapacity& /*other*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void BurnCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void BurnCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void BurnCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    BurnCapacity* pBurnCapacity = new BurnCapacity( *this );
    object.AddCapacity( pBurnCapacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( pBurnCapacity ) );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Finalize
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnCapacity::Finalize( MIL_Object_ABC& object )
{
    BurnAttribute& burnAttribute = object.GetAttribute< BurnAttribute >();
    burnAttribute.StartBurn( object );

    // Overwrite urban destruction capacity
    const MIL_FireClass& fireClass = object.GetAttribute< FireAttribute >().GetClass();
    UrbanDestructionCapacity* destructionCapacity = object.Retrieve< UrbanDestructionCapacity >();
    if( !destructionCapacity )
    {
        destructionCapacity = new UrbanDestructionCapacity( fireClass.GetUrbanAttritionData() );
        object.AddCapacity( destructionCapacity );
    }
    else
        *destructionCapacity = UrbanDestructionCapacity( fireClass.GetUrbanAttritionData() );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Composantes >().ApplyBurn( object.GetAttribute< FireAttribute >().GetBurnEffect() );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Composantes >().ApplyBurn( object.GetAttribute< FireAttribute >().GetBurnEffect() );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessPopulationInside
// Created: RFT 2008-06-06
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    population.ApplyBurn( object.GetAttribute< FireAttribute >().GetBurnEffect() );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Update
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    object.GetAttribute< BurnAttribute >().Burn( object );
}
