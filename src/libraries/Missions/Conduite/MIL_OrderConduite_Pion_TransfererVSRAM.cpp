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

#include "Missions_pch.h"
#include "MIL_OrderConduite_Pion_TransfererVSRAM.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_Pion_TransfererVSRAM::nDIAPionRenforceIdx_ = 0 ;
int MIL_OrderConduite_Pion_TransfererVSRAM::nDIAPionARenforcerIdx_ = 0 ;
int MIL_OrderConduite_Pion_TransfererVSRAM::nDIANbrAmbulancesIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_TransfererVSRAM::InitializeDIA
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Pion_TransfererVSRAM::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAPionRenforceIdx_ = DEC_Tools::InitializeDIAField( "pionRenforce_", diaType );
    nDIAPionARenforcerIdx_ = DEC_Tools::InitializeDIAField( "pionARenforcer_", diaType );
    nDIANbrAmbulancesIdx_ = DEC_Tools::InitializeDIAField( "nbrAmbulances_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_TransfererVSRAM constructor
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_TransfererVSRAM::MIL_OrderConduite_Pion_TransfererVSRAM( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_TransfererVSRAM destructor
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_TransfererVSRAM::~MIL_OrderConduite_Pion_TransfererVSRAM()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_TransfererVSRAM::Initialize
// Created: AGR 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Pion_TransfererVSRAM::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Pion_TransfererVSRAM& asnMission = *asnMsg.u.order_conduite_pion_transferer_vs_ram;
    if( !NET_ASN_Tools::CopyAgent( asnMission.pion_renforce, GetVariable( nDIAPionRenforceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAgent( asnMission.pion_a_renforcer, GetVariable( nDIAPionARenforcerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.nbr_ambulances, GetVariable( nDIANbrAmbulancesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_TransfererVSRAM::Initialize
// Created: AGR 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Pion_TransfererVSRAM::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyAgent( diaParams[nCurDIAParamIdx++], GetVariable( nDIAPionRenforceIdx_));
    NET_ASN_Tools::CopyAgent( diaParams[nCurDIAParamIdx++], GetVariable( nDIAPionARenforcerIdx_));
    NET_ASN_Tools::CopyNumeric( diaParams[nCurDIAParamIdx++], GetVariable( nDIANbrAmbulancesIdx_));

}
