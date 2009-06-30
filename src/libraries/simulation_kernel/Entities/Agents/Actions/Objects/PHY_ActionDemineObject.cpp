// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDemineObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:47 $
// $Revision: 2 $
// $Workfile: PHY_ActionDemineObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionDemineObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject constructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDemineObject::PHY_ActionDemineObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , diaReturnCode_( diaCall.GetParameter( 0 ) )
    , nKnowledgeID_ ( (uint)diaCall.GetParameter( 1 ).ToPtr() )
{    
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaCall.GetParameter( 1 ) ) );
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject destructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDemineObject::~PHY_ActionDemineObject()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject::Execute
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDemineObject::Execute()
{   
    int nReturn = role_.Demine( nKnowledgeID_ );
    diaReturnCode_.SetValue( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDemineObject::ExecuteSuspended
// Demined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionDemineObject::ExecuteSuspended()
{
    role_.DemineSuspended();
}
