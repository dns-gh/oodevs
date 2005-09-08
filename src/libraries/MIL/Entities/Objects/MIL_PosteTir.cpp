//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PosteTir.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:20 $
// $Revision: 6 $
// $Workfile: MIL_PosteTir.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_PosteTir.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

BOOST_CLASS_EXPORT_GUID( MIL_PosteTir, "MIL_PosteTir" )

//-----------------------------------------------------------------------------
// Name: MIL_PosteTir constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PosteTir::MIL_PosteTir()
    : MIL_RealObject_ABC( MIL_RealObjectType::posteTir_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_PosteTir destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PosteTir::~MIL_PosteTir()
{
	
}


// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_PosteTir::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_PosteTir::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PosteTir::ProcessAgentExiting
// Created: NLD 2004-04-29
// Last modified: JVT 04-05-17
// -----------------------------------------------------------------------------
void MIL_PosteTir::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Posture >().UnsetPosturePostePrepareGenie();
}

// -----------------------------------------------------------------------------
// Name: MIL_PosteTir::ProcessAgentInside
// Created: NLD 2004-04-29
// Last modified: JVT 04-05-17
// -----------------------------------------------------------------------------
void MIL_PosteTir::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );

    if( !CanInteractWithAgent( agent ) || GetConstructionPercentage() < 1. )
        return;

    if( GetOccupier() == &agent )
        agent.GetRole< PHY_RoleInterface_Posture >().SetPosturePostePrepareGenie();
    else
        agent.GetRole< PHY_RoleInterface_Posture >().UnsetPosturePostePrepareGenie();
}
