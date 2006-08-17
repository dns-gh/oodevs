//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneImplantationMortier.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 15:46 $
// $Revision: 4 $
// $Workfile: MIL_ZoneImplantationMortier.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZoneImplantationMortier.h"
#include "MIL_RealObjectType.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneImplantationMortier, "MIL_ZoneImplantationMortier" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationMortier constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneImplantationMortier::MIL_ZoneImplantationMortier()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneImplantationMortier_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationMortier destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneImplantationMortier::~MIL_ZoneImplantationMortier()
{
	
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationMortier::ProcessAgentEntering
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void MIL_ZoneImplantationMortier::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( CanInteractWith( agent ) )
        agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 2. );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationMortier::ProcessAgentExiting
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void MIL_ZoneImplantationMortier::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 1. );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationMortier::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneImplantationMortier::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
