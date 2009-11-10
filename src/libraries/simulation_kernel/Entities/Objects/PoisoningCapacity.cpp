// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PoisoningCapacity.h"
#include "Object.h"
#include "NBCTypeAttribute.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Agents/Units/Humans/MIL_Injury_Poison.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( PoisoningCapacity, "PoisoningCapacity" )

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity constructor
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
PoisoningCapacity::PoisoningCapacity( xml::xistream& xis )    
    : injuryID_( 0 )
{
    InitializeData( xis );
}

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity constructor
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
PoisoningCapacity::PoisoningCapacity() 
    : injuryID_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity constructor
// Created: RFT 2008-06-13
// -----------------------------------------------------------------------------
PoisoningCapacity::PoisoningCapacity( const PoisoningCapacity& /*from*/ )
    : injuryID_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity destructor
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
PoisoningCapacity::~PoisoningCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::InitializeSpeed
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
void PoisoningCapacity::InitializeData( xml::xistream& xis )
{
    xis >> xml::attribute( "injury-id", injuryID_ );
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::serialize
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PoisoningCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );        
}

// -----------------------------------------------------------------------------
// Name: BurnCapacity::Register
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void PoisoningCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity::Instanciate
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
void PoisoningCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new PoisoningCapacity( *this ) );
}
	
// -----------------------------------------------------------------------------
// Name: PoisoningCapacity::Update
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
void PoisoningCapacity::Update( Object& /*object*/, float /*time*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessAgentMovingInside
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
void PoisoningCapacity::ProcessAgentMovingInside( Object& object, MIL_Agent_ABC& agent )
{
    NBCTypeAttribute* attr = object.RetrieveAttribute< NBCTypeAttribute >();
    if ( attr && attr->GetAgentType().IsPoisoning() )
    {
        MIL_Injury_Poison injury( attr->GetConcentration() , attr->GetAgentType().GetName() , injuryID_ );
        agent.GetRole< PHY_RoleInterface_Composantes >().ApplyInjury( injury );
    }
}

// -----------------------------------------------------------------------------
// Name: AttritionCapacity::ProcessPopulationMovingInside
// Created: RFT 2008-06-06
// -----------------------------------------------------------------------------
void PoisoningCapacity::ProcessPopulationMovingInside( Object& object, MIL_PopulationElement_ABC& population )
{
    NBCTypeAttribute* attr = object.RetrieveAttribute< NBCTypeAttribute >();
    if( attr && attr->GetAgentType().IsPoisoning() )
    {
        MIL_Injury_Poison injury( attr->GetConcentration() , attr->GetAgentType().GetName() , injuryID_ ); //Number 4
        population.ApplyInjury( injury );
    }
}

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity::ProcessAgentInside
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
void PoisoningCapacity::ProcessAgentInside( Object& /*object*/, MIL_Agent_ABC& /*agent*/ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PoisoningCapacity::ProcessAgentEntering
// Created: RFT 2008-06-02
// -----------------------------------------------------------------------------
void PoisoningCapacity::ProcessAgentEntering( Object& /*object*/, MIL_Agent_ABC& /*agent*/ )
{
    //NOTHING
}
