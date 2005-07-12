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

#include "Missions_pch.h"
#include "MIL_OrderConduite_Automate_TransfererRenfort.h"

#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Network/NET_ASN_Tools.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

int MIL_OrderConduite_Automate_TransfererRenfort::nDIAPionATransfererIdx_ = 0 ;
int MIL_OrderConduite_Automate_TransfererRenfort::nDIAAutomateARenforcerIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenfort::InitializeDIA
// Created: AGR 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Automate_TransfererRenfort::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAPionATransfererIdx_ = DEC_Tools::InitializeDIAField( "pionATransferer_", diaType );
    nDIAAutomateARenforcerIdx_ = DEC_Tools::InitializeDIAField( "automateARenforcer_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenfort constructor
// Created: AGR 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_TransfererRenfort::MIL_OrderConduite_Automate_TransfererRenfort( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenfort destructor
// Created: AGR 2005-5-9 - 11:49:5
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_TransfererRenfort::~MIL_OrderConduite_Automate_TransfererRenfort()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenfort::Initialize
// Created: AGR 2005-5-9 - 11:49:5
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Automate_TransfererRenfort::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Automate_TransfererRenfort& asnMission = *asnMsg.u.order_conduite_automate_transferer_renfort;
    if( !NET_ASN_Tools::CopyAgent( asnMission.pion_a_transferer, GetVariable( nDIAPionATransfererIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.automate_a_renforcer, GetVariable( nDIAAutomateARenforcerIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_TransfererRenfort::Initialize
// Created: AGR 2005-5-9 - 11:49:5
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Automate_TransfererRenfort::Initialize( DIA_Parameters& /*diaParams*/, uint /*nCurDIAParamIdx*/ )
{
    assert( false );
}
