// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/PHY_Action_ABC.cpp $
// $Author: Nld $
// $Modtime: 21/10/04 10:19 $
// $Revision: 2 $
// $Workfile: PHY_Action_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Action_ABC.h"

#include "PHY_Actor.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_Action_ABC::PHY_Action_ABC( PHY_Actor& actor )
    : bSuspended_( false )
    , actor_     ( actor )
{
    actor_.RegisterAction( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Action_ABC::~PHY_Action_ABC()
{
    actor_.UnregisterAction( *this );
}


// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::Suspend
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Action_ABC::Suspend()
{
    bSuspended_ = true;    
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::Resume
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Action_ABC::Resume()
{
    bSuspended_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_Action_ABC::Update()
{
    if( bSuspended_ )
        ExecuteSuspended();
    else
        Execute();
}