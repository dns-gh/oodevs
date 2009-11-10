// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDistributeObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:46 $
// $Revision: 2 $
// $Workfile: PHY_ActionDistributeObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionDistributeObject.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDistributeObject constructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDistributeObject::PHY_ActionDistributeObject( MIL_AgentPion& pion
                                                       , boost::shared_ptr< DEC_Knowledge_Object > pObject
                                                       , boost::shared_ptr< DEC_Knowledge_Population > pPeople
                                                       , int quantity )
    : PHY_DecisionCallbackAction_ABC    ( pion )
    , role_                             ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pObject_                          ( pObject )
    , pPeople_                          ( pPeople )
    , quantity_                         ( quantity )
{    
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDistributeObject destructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDistributeObject::~PHY_ActionDistributeObject()
{
    Callback( role_.GetFinalReturnCode() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionDistributeObject::Execute
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionDistributeObject::Execute()
{   
    int nReturn = role_.Distribute( pObject_, pPeople_, 1 );
    
    if ( --quantity_ <= 0 )
        nReturn = role_.GetFinalReturnCode();
    
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDistributeObject::ExecuteSuspended
// Mined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionDistributeObject::ExecuteSuspended()
{
    // NOTHING
}
