// *****************************************************************************
//
// $Destroyd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:47 $
// $Revision: 2 $
// $Workfile: PHY_ActionDestroyObject.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDestroyObject.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject constructor
// Destroyd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDestroyObject::PHY_ActionDestroyObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC    ( pion, diaCall )
    , role_              ( pion.GetRole< PHY_RoleAction_Objects >() )
    , diaReturnCode_     ( diaCall.GetParameter( 0 ) )
    , nKnowledgeID_      ( (uint)diaCall.GetParameter( 1 ).ToPtr() )
{    
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaCall.GetParameter( 1 ) ) );
    diaReturnCode_.SetValue( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject destructor
// Destroyd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDestroyObject::~PHY_ActionDestroyObject()
{
    diaReturnCode_.SetValue( role_.GetFinalReturnCode() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject::Execute
// Destroyd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDestroyObject::Execute()
{   
    int nReturn = role_.Destroy( nKnowledgeID_ );
    diaReturnCode_.SetValue( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDestroyObject::ExecuteSuspended
// Destroyd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionDestroyObject::ExecuteSuspended()
{
    role_.DestroySuspended();
}
