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
#include "MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::nDIAPionRenforceIdx_ = 0 ;
int MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::nDIANbrRemorqueursIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAPionRenforceIdx_ = DEC_Tools::InitializeDIAField( "pionRenforce_", diaType );
    nDIANbrRemorqueursIdx_ = DEC_Tools::InitializeDIAField( "nbrRemorqueurs_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::~MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs& asnMission = *asnMsg.u.order_conduite_pion_reprendre_aux_ordres_remorqueurs;
    if( !NET_ASN_Tools::CopyAgent( asnMission.pion_renforce, GetVariable( nDIAPionRenforceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.nbr_remorqueurs, GetVariable( nDIANbrRemorqueursIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyAgent( diaParams[nCurDIAParamIdx++], GetVariable( nDIAPionRenforceIdx_));
    NET_ASN_Tools::CopyNumeric( diaParams[nCurDIAParamIdx++], GetVariable( nDIANbrRemorqueursIdx_));

}
