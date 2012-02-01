// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionResumeWorkObject.cpp $
// $Author: Nld $
// $Modtime: 19/01/05 14:57 $
// $Revision: 1 $
// $Workfile: PHY_ActionResumeWorkObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionResumeWorkObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkObject constructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionResumeWorkObject::PHY_ActionResumeWorkObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, bool valorizeIt )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pKnowledge_   ( pKnowledge )
    , valorizeIt_   ( valorizeIt )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkObject destructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionResumeWorkObject::~PHY_ActionResumeWorkObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkObject::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionResumeWorkObject::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkObject::Execute
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionResumeWorkObject::Execute()
{
    int nReturn = role_.ResumeWork( pKnowledge_, valorizeIt_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkObject::ExecuteSuspended
// Mined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionResumeWorkObject::ExecuteSuspended()
{
    role_.ResumeWorkSuspended();
}
