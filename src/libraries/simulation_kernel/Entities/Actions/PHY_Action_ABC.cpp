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

MIL_IDManager PHY_Action_ABC::idManager_;
// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
PHY_Action_ABC::PHY_Action_ABC()
    : id_( idManager_.GetFreeId() )
    , bSuspended_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Action_ABC::~PHY_Action_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::GetId
// Created: MGD 2010-03-12
// -----------------------------------------------------------------------------
unsigned int PHY_Action_ABC::GetId() const
{
    return id_;
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

// -----------------------------------------------------------------------------
// Name: PHY_DecisionCallbackAction_ABC::Stop
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
void PHY_Action_ABC::Stop()
{
    //NOTHING
}
