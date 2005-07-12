// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_Move.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_Effect_Move.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_Effect_Move.h"

#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Move::MIL_Effect_Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MIL_Effect_Move::MIL_Effect_Move( PHY_RoleAction_Moving& roleMoving )
    : MIL_Effect_ABC()
    , roleMoving_    ( roleMoving )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Move::~MIL_Effect_Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MIL_Effect_Move::~MIL_Effect_Move()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Move::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_Move::Execute()
{
    roleMoving_.Apply();
    return false; // Effect must be stopped
}
