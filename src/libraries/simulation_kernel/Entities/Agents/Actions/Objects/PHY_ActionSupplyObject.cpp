// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionSupplyObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:46 $
// $Revision: 2 $
// $Workfile: PHY_ActionSupplyObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionSupplyObject.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionSupplyObject constructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionSupplyObject::PHY_ActionSupplyObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > object, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity )
    : PHY_DecisionCallbackAction_ABC( pion )
    , dotationTypes_( dotationTypes )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , object_( object )
    , quantity_( quantity )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionSupplyObject destructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionSupplyObject::~PHY_ActionSupplyObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionSupplyObject::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionSupplyObject::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionSupplyObject::Execute
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionSupplyObject::Execute()
{
    int nReturn = role_.SupplyStock( object_, dotationTypes_, quantity_ );
    Callback ( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionSupplyObject::ExecuteSuspended
// Mined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionSupplyObject::ExecuteSuspended()
{
    // NOTHING
}
