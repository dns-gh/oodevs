// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Fly.cpp $
// $Author: Nld $
// $Modtime: 7/03/05 10:24 $
// $Revision: 4 $
// $Workfile: MIL_Effect_Fly.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_Effect_Fly.h"

#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fly::MIL_Effect_Fly
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MIL_Effect_Fly::MIL_Effect_Fly( PHY_RoleAction_InterfaceFlying& roleFlying )
    : MIL_Effect_ABC()
    , roleFlying_   ( roleFlying )
    , rHeight_      ( 0. )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fly::~MIL_Effect_Fly
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MIL_Effect_Fly::~MIL_Effect_Fly()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fly::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_Fly::Execute()
{
    roleFlying_.Apply( rHeight_ );
    return false; // Effect must be stopped
}
