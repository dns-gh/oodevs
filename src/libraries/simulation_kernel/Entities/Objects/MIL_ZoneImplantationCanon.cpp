//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneImplantationCanon.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 15:16 $
// $Revision: 6 $
// $Workfile: MIL_ZoneImplantationCanon.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_ZoneImplantationCanon.h"
#include "MIL_RealObjectType.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneImplantationCanon, "MIL_ZoneImplantationCanon" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationCanon constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneImplantationCanon::MIL_ZoneImplantationCanon()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneImplantationCanon_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationCanon destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneImplantationCanon::~MIL_ZoneImplantationCanon()
{
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationCanon::ProcessAgentEntering
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void MIL_ZoneImplantationCanon::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( CanInteractWith( agent ) )
        agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 2. );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationCanon::ProcessAgentExiting
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void MIL_ZoneImplantationCanon::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Posture >().SetTimingFactor( 1. );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneImplantationCanon::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneImplantationCanon::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
