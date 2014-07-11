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
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/WoundEffectsHandler_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"

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
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    MIL_BurnEffectManipulator manipulator( object.GetAttribute< FireAttribute >().GetBurnEffect() );
    agent.Apply( &WoundEffectsHandler_ABC::ApplyEffect, manipulator );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    MIL_BurnEffectManipulator manipulator( object.GetAttribute< FireAttribute >().GetBurnEffect() );
    agent.Apply( &WoundEffectsHandler_ABC::ApplyEffect, manipulator );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::ProcessAgentEntering
// Created: LDC 2014-07-11
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentEntering( MIL_Object_ABC&, MIL_Agent_ABC& agent )
{
    MIL_Report::PostEvent( agent, report::eRC_InFireObject );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessPopulationInside
// Created: RFT 2008-06-06
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    population.ApplyBurn( object.GetLocalisation() );
    population.GetPopulation().NotifyBurning();
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Update
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnCapacity::Update( MIL_Object_ABC& object, unsigned int /*time*/ )
{
    object.GetAttribute< BurnAttribute >().Burn( object );

    //attrition on objects
    const PHY_UrbanAttritionData& urbanAttritionData = object.GetAttribute< FireAttribute >().GetClass().GetUrbanAttritionData();
    std::vector< TER_Object_ABC* > objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinLocalisation( object.GetLocalisation(), objects );
    for( std::vector< TER_Object_ABC* >::iterator it = objects.begin(); it != objects.end(); ++it )
    {
        MIL_Object_ABC* pObject = static_cast< MIL_Object_ABC* >( *it );
        const TER_Localisation& location = object.GetLocalisation();
        if( location.GetType() == TER_Localisation::ePoint )
        {
            if( pObject->GetLocalisation().IsInside( location.GetPoints().front() ) )
                pObject->ApplyDestruction( location, urbanAttritionData );
        }
        else
            pObject->ApplyDestruction( location, urbanAttritionData );
    }
}
