// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionExtinguishObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:47 $
// $Revision: 2 $
// $Workfile: PHY_ActionExtinguishObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionExtinguishObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject constructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionExtinguishObject::PHY_ActionExtinguishObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC( pion, diaCall )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , diaReturnCode_( diaCall.GetParameter( 0 ) )
    , nKnowledgeID_ ( (uint)diaCall.GetParameter( 1 ).ToPtr() )
{    
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaCall.GetParameter( 1 ) ) );
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject destructor
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionExtinguishObject::~PHY_ActionExtinguishObject()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject::Execute
// Demined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionExtinguishObject::Execute()
{   
    int nReturn = role_.Extinguish( nKnowledgeID_ );
    diaReturnCode_.SetValue( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtinguishObject::ExecuteSuspended
// Demined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionExtinguishObject::ExecuteSuspended()
{
    role_.ExtinguishSuspended();
}