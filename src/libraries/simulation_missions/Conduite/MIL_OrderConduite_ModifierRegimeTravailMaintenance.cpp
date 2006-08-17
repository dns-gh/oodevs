// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Pion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:52 $
// $Revision: 5 $
// $Workfile: AGR_OrderConduite_Pion_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_OrderConduite_ModifierRegimeTravailMaintenance.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_ModifierRegimeTravailMaintenance::nDIAOrderConduiteModifierRegimeTravailMaintenanceIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierRegimeTravailMaintenance::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ModifierRegimeTravailMaintenance::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteModifierRegimeTravailMaintenanceIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteModifierRegimeTravailMaintenance_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierRegimeTravailMaintenance constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ModifierRegimeTravailMaintenance::MIL_OrderConduite_ModifierRegimeTravailMaintenance( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierRegimeTravailMaintenance destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ModifierRegimeTravailMaintenance::~MIL_OrderConduite_ModifierRegimeTravailMaintenance()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierRegimeTravailMaintenance::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ModifierRegimeTravailMaintenance::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_ModifierRegimeTravailMaintenance& asnMission = asnMsg.u.order_conduite_modifier_regime_travail_maintenance;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission, GetVariable( nDIAOrderConduiteModifierRegimeTravailMaintenanceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ModifierRegimeTravailMaintenance::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_ModifierRegimeTravailMaintenance::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyEnumeration( diaParams[nCurDIAParamIdx++], GetVariable( nDIAOrderConduiteModifierRegimeTravailMaintenanceIdx_));

}
