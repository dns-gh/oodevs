// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_ActionRecoverHumanTransporters.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 14:50 $
// $Revision: 2 $
// $Workfile: PHY_ActionRecoverHumanTransporters.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_ActionRecoverHumanTransporters.h"

#include "PHY_RoleAction_Loading.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"

#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoverHumanTransportersconstructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionRecoverHumanTransporters::PHY_ActionRecoverHumanTransporters( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC      ( pion, diaCall )
    , role_               ( pion.GetRole< PHY_RoleAction_Loading >() )
    , diaReturnCode_      ( diaCall.GetParameter( 0 ) )
    , vRecoveringPosition_()
{    
    if ( diaCall.GetParameters().GetParameters().size() > 1 )
    {
        assert( DEC_Tools::CheckTypePoint( diaCall.GetParameter( 1 ) ) );
        
        MT_Vector2D* vDummy; // $$$$ JVT : No Comment !
        vRecoveringPosition_ = *diaCall.GetParameter( 1 ).ToUserPtr( vDummy );
    }
    else
        vRecoveringPosition_ = pion.GetRole< PHY_RolePion_Location >().GetPosition();
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoverHumanTransportersdestructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionRecoverHumanTransporters::~PHY_ActionRecoverHumanTransporters()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoverHumanTransporters::Execute
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionRecoverHumanTransporters::Execute()
{   
    int nResult = role_.RecoverHumanTransporters( vRecoveringPosition_ );
    diaReturnCode_.SetValue( nResult );   
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoverHumanTransporters::ExecuteSuspended
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionRecoverHumanTransporters::ExecuteSuspended()
{
    role_.RecoverHumanTransportersSuspended();
}
