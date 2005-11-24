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
#include "MIL_OrderConduite_AcquerirObjectif.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_AcquerirObjectif::nDIAOrderConduiteAcquerirObjectifIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AcquerirObjectif::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_AcquerirObjectif::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteAcquerirObjectifIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteAcquerirObjectif_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AcquerirObjectif constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_AcquerirObjectif::MIL_OrderConduite_AcquerirObjectif( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AcquerirObjectif destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_AcquerirObjectif::~MIL_OrderConduite_AcquerirObjectif()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AcquerirObjectif::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_AcquerirObjectif::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_AcquerirObjectif& asnMission = *asnMsg.u.order_conduite_acquerir_objectif;
    if( !NET_ASN_Tools::CopyLocation( asnMission, orderConduiteAcquerirObjectif_, GetVariable( nDIAOrderConduiteAcquerirObjectifIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AcquerirObjectif::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_AcquerirObjectif::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyLocation( diaParams[nCurDIAParamIdx++], orderConduiteAcquerirObjectif_, GetVariable( nDIAOrderConduiteAcquerirObjectifIdx_));

}
