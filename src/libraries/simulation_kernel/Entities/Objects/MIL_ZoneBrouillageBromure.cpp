//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneBrouillageBromure.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:24 $
// $Revision: 4 $
// $Workfile: MIL_ZoneBrouillageBromure.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZoneBrouillageBromure.h"
#include "MIL_RealObjectType.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneBrouillageBromure, "MIL_ZoneBrouillageBromure" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBromure constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneBrouillageBromure::MIL_ZoneBrouillageBromure()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneBrouillageBromure_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBromure destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneBrouillageBromure::~MIL_ZoneBrouillageBromure()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBromure::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneBrouillageBromure::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}


// =============================================================================
// EVENTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBromure::ProcessAgentInside
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void MIL_ZoneBrouillageBromure::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( CanInteractWith( agent ) && GetArmy().IsAnEnemy( agent.GetArmy() ) == eTristate_True )
        agent.GetRole< PHY_RoleInterface_Communications >().Jam( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBromure::ProcessAgentExiting
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void MIL_ZoneBrouillageBromure::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Communications >().Unjam( *this );
}
