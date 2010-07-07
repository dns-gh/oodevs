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
BurnCapacity::BurnCapacity( xml::xistream& xis )
{
    InitializeData( xis );
}

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
BurnCapacity::BurnCapacity( const BurnCapacity& /*from*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::InitializeSpeed
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
void BurnCapacity::InitializeData( xml::xistream& xis )
{
    xis >> xml::attribute( "injury-id", injuryID_ );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void BurnCapacity::serialize( Archive& file, const uint )
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
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void BurnCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new BurnCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Update
// Created: RFT 2008-05-22
// -----------------------------------------------------------------------------
void BurnCapacity::Update( MIL_Object_ABC& /*object*/, float /*time*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    FireAttribute& attr = object.GetAttribute< FireAttribute >();
    MIL_Injury_Fire injury( attr.GetHeat() , attr.GetClass().GetName() , injuryID_ );
    agent.GetRole< PHY_RoleInterface_Composantes >().ApplyInjury( injury );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    FireAttribute& attr = object.GetAttribute< FireAttribute >();
    MIL_Injury_Fire injury( attr.GetHeat() , attr.GetClass().GetName() , injuryID_ );
    agent.GetRole< PHY_RoleInterface_Composantes >().ApplyInjury( injury );
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessPopulationInside
// Created: RFT 2008-06-06
// -----------------------------------------------------------------------------
void BurnCapacity::ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population )
{
    FireAttribute& attr = object.GetAttribute< FireAttribute >();
    MIL_Injury_Fire injury( attr.GetHeat() , attr.GetClass().GetName() , injuryID_ );
    population.ApplyInjury( injury );
}
