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

#include "simulation_kernel_pch.h"
#include "MIL_Effect_Move.h"
#include "Decision/Path/DEC_PathWalker.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Move::MIL_Effect_Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MIL_Effect_Move::MIL_Effect_Move( DEC_PathWalker& roleMoving )
    : MIL_Effect_ABC()
    , roleMoving_( roleMoving )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Move::~MIL_Effect_Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MIL_Effect_Move::~MIL_Effect_Move()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Move::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_Move::Execute()
{
    roleMoving_.Apply();
    return false; // Effect must be stopped
}
