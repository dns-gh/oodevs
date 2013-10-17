// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionExtractFromStockObject.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 17:46 $
// $Revision: 2 $
// $Workfile: PHY_ActionExtractFromStockObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionExtractFromStockObject.h"
#include "PHY_RoleAction_Objects.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtractFromStockObject constructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionExtractFromStockObject::PHY_ActionExtractFromStockObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pObject, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity )
    : PHY_DecisionCallbackAction_ABC( pion )
    , dotationTypes_( dotationTypes )
    , role_    ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pObject_ ( pObject )
    , quantity_( quantity )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtractFromStockObject destructor
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionExtractFromStockObject::~PHY_ActionExtractFromStockObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtractFromStockObject::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionExtractFromStockObject::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtractFromStockObject::Execute
// Mined: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionExtractFromStockObject::Execute()
{
    int nReturn = role_.ExtractFromStock( pObject_, dotationTypes_, quantity_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionExtractFromStockObject::ExecuteSuspended
// Mined: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionExtractFromStockObject::ExecuteSuspended()
{
    // NOTHING
}
