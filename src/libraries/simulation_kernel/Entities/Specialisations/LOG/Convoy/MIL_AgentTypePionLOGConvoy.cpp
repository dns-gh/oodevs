// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Convoy/MIL_AgentTypePionLOGConvoy.cpp $
// $Author: Nld $
// $Modtime: 8/02/05 17:44 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionLOGConvoy.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionLOGConvoy.h"

#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOGConvoy_Supply.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLog.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGConvoy::MIL_AgentTypePionLOGConvoy( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePionLOG_ABC( strName, strType, xis )
{
    // Don't serialize Convoys
    SetWrittenInODB( false );
    // $$$ ?? Checker que le type ne contient aucun equipement ?
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGConvoy destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionLOGConvoy::~MIL_AgentTypePionLOGConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionLOGMedical::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in AgentFactory
// -----------------------------------------------------------------------------
void MIL_AgentTypePionLOGConvoy::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    MIL_AgentTypePion::RegisterRoles( pion, ext );
    pion.RegisterRole( *new PHY_RolePionLOGConvoy_Supply( pion ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionLOGConvoy::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGConvoy( strName, strType, xis );
}

void MIL_AgentTypePionLOGConvoy::DeleteUnit( MIL_Agent_ABC& unit ) const
{
    MIL_AutomateLOG* logBrain = unit.GetAutomate().GetBrainLogistic();
    if( logBrain )
        logBrain->ResetConsignsForConvoyPion( unit );
    unit.GetAutomate().GetStockSupplyManager().ResetConsignsForConvoyPion( unit );
    unit.GetAutomate().GetDotationSupplyManager().ResetConsignsForConvoyPion( unit );
}
