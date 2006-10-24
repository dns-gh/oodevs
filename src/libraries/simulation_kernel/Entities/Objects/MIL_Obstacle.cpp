//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Obstacle.cpp $
// $Author: Nld $
// $Modtime: 29/03/05 17:51 $
// $Revision: 1 $
// $Workfile: MIL_Obstacle.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Obstacle.h"

#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"

//-----------------------------------------------------------------------------
// Name: MIL_Obstacle constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Obstacle::MIL_Obstacle( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
{    
}

// -----------------------------------------------------------------------------
// Name: MIL_Obstacle constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Obstacle::MIL_Obstacle()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_Obstacle destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Obstacle::~MIL_Obstacle()
{
	
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Obstacle::CanInteractWith
// Created: NLD 2005-03-29
// -----------------------------------------------------------------------------
bool MIL_Obstacle::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    if( agent.GetArmy() == GetArmy() )
        return false;

    return MIL_RealObject_ABC::CanInteractWith( agent );
}
