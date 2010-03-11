// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_Population_ActionMove.cpp $
// $Author: Nld $
// $Modtime: 16/03/05 16:04 $
// $Revision: 5 $
// $Workfile: PHY_Population_ActionMove.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Population_ActionMove.h"
#include "Entities/Populations/MIL_Population.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionMove::PHY_Population_ActionMove( MIL_Population& population, MT_Vector2D* pTargetPosition )
    : PHY_Action_ABC      ()
    , population_         ( population )
    , destination_        ()
{    
    assert( pTargetPosition );
    destination_ = *pTargetPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionMove::~PHY_Population_ActionMove()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove::Stop
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_Population_ActionMove::Stop()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_Population_ActionMove::Execute()
{   
    population_.Move( destination_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_Population_ActionMove::ExecuteSuspended()
{
    // NOTHING
}
