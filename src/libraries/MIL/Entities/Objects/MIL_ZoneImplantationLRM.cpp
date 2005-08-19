//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneImplantationLRM.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 15:16 $
// $Revision: 6 $
// $Workfile: MIL_ZoneImplantationLRM.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_ZoneImplantationLRM.h"
#include "MIL_RealObjectType.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneImplantationLRM, "MIL_ZoneImplantationLRM" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationLRM constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneImplantationLRM::MIL_ZoneImplantationLRM()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneImplantationLRM_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationLRM destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneImplantationLRM::~MIL_ZoneImplantationLRM()
{
	
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationLRM::ProcessAgentEntering
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void MIL_ZoneImplantationLRM::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 2. );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationLRM::ProcessAgentExiting
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void MIL_ZoneImplantationLRM::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 1. );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationLRM::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneImplantationLRM::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
