// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OrderConduite_Automate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 16:14 $
// $Revision: 5 $
// $Workfile: AGR_OrderConduite_Automate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_OrderConduite_Automate_TransfererRenforts.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_Automate_TransfererRenforts::nDIAPionATransfererIdx_ = 0 ;
int MIL_OrderConduite_Automate_TransfererRenforts::nDIAAutomateARenforcerIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenforts::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Automate_TransfererRenforts::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAPionATransfererIdx_ = DEC_Tools::InitializeDIAField( "pionATransferer_", diaType );
    nDIAAutomateARenforcerIdx_ = DEC_Tools::InitializeDIAField( "automateARenforcer_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenforts constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_TransfererRenforts::MIL_OrderConduite_Automate_TransfererRenforts( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenforts destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_TransfererRenforts::~MIL_OrderConduite_Automate_TransfererRenforts()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenforts::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Automate_TransfererRenforts::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Automate_TransfererRenforts& asnMission = *asnMsg.u.order_conduite_automate_transferer_renforts;
    if( !NET_ASN_Tools::CopyAgent( asnMission.pion_a_transferer, GetVariable( nDIAPionATransfererIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.automate_a_renforcer, GetVariable( nDIAAutomateARenforcerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenforts::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Automate_TransfererRenforts::Initialize( DIA_Parameters& /*diaParams*/, uint /*nCurDIAParamIdx*/ )
{
    assert( false );
}
