// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionWorkOnDecontaminationSite.cpp $
// $Author: Nld $
// $Modtime: 2/11/04 18:45 $
// $Revision: 1 $
// $Workfile: PHY_ActionWorkOnDecontaminationSite.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_ActionWorkOnDecontaminationSite.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionWorkOnDecontaminationSite constructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionWorkOnDecontaminationSite::PHY_ActionWorkOnDecontaminationSite( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC    ( pion, diaCall )
    , role_             ( pion.GetRole< PHY_RoleAction_Objects >() )
    , nKnowledgeID_     ( (uint)diaCall.GetParameter( 0 ).ToPtr() )
{    
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaCall.GetParameter( 0 ) ) );
    role_.StartWorkingOnDecontaminationSite( nKnowledgeID_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionWorkOnDecontaminationSite destructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionWorkOnDecontaminationSite::~PHY_ActionWorkOnDecontaminationSite()
{
    role_.StopWorkingOnDecontaminationSite( nKnowledgeID_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionWorkOnDecontaminationSite::Execute
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionWorkOnDecontaminationSite::Execute()
{   
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionWorkOnDecontaminationSite::ExecuteSuspended
// Demined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionWorkOnDecontaminationSite::ExecuteSuspended()
{
    // NOTHING
}
