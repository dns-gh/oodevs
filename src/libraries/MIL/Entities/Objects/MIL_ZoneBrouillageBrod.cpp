//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneBrouillageBrod.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:23 $
// $Revision: 4 $
// $Workfile: MIL_ZoneBrouillageBrod.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_ZoneBrouillageBrod.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneBrouillageBrod, "MIL_ZoneBrouillageBrod" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBrod constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneBrouillageBrod::MIL_ZoneBrouillageBrod()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneBrouillageBrod_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBrod destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneBrouillageBrod::~MIL_ZoneBrouillageBrod()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBrod::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneBrouillageBrod::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBrod::ProcessAgentInside
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void MIL_ZoneBrouillageBrod::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( CanInteractWithAgent( agent ) )
        agent.GetRole< PHY_RoleInterface_Communications >().Jam( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneBrouillageBrod::ProcessAgentExiting
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void MIL_ZoneBrouillageBrod::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    agent.GetRole< PHY_RoleInterface_Communications >().Unjam( *this );
}
