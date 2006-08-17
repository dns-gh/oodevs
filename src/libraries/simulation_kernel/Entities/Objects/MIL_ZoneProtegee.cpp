//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneProtegee.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_ZoneProtegee.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZoneProtegee.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneProtegee, "MIL_ZoneProtegee" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneProtegee constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneProtegee::MIL_ZoneProtegee()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneProtegee_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneProtegee destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneProtegee::~MIL_ZoneProtegee()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneProtegee::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneProtegee::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// =============================================================================
// EVENTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneProtegee::ProcessAgentExiting
// Created: NLD 2004-04-29
// Last modified: JVT 04-05-17
// -----------------------------------------------------------------------------
void MIL_ZoneProtegee::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Posture >().UnsetPosturePostePrepareGenie();
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneProtegee::ProcessAgentInside
// Created: NLD 2004-04-29
// Last modified: JVT 04-05-17
// -----------------------------------------------------------------------------
void MIL_ZoneProtegee::ProcessAgentInside( MIL_Agent_ABC& agent )
{   
    MIL_RealObject_ABC::ProcessAgentEntering( agent );

    if( !CanInteractWith( agent ) || GetConstructionPercentage() < 1. )
        return;

    if( GetOccupier() == &agent )
        agent.GetRole< PHY_RoleInterface_Posture >().SetPosturePostePrepareGenie();
    else
        agent.GetRole< PHY_RoleInterface_Posture >().UnsetPosturePostePrepareGenie();
}
