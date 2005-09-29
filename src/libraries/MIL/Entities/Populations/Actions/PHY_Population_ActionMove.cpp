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

#include "MIL_pch.h"

#include "PHY_Population_ActionMove.h"

#include "Entities/Populations/MIL_Population.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionMove::PHY_Population_ActionMove( MIL_Population& population, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC      ( population, diaCall )
    , population_         ( population )
//    , diaReturnCode_      ( diaCall.GetParameter( 0 ) )
    , destination_        ()
{    
    assert( DEC_Tools::CheckTypePoint( diaCall.GetParameter( 0 ) ) );

    MT_Vector2D* pTargetPosition = diaCall.GetParameter( 0 ).ToUserPtr( pTargetPosition );
    assert( pTargetPosition );
    destination_ = *pTargetPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_Population_ActionMove destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_Population_ActionMove::~PHY_Population_ActionMove()
{
    //diaReturnCode_.SetValue( PHY_RoleAction_Moving::eFinished );
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
    Execute();
}
