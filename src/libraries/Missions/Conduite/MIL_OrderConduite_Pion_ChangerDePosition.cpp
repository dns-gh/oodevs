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
#include "MIL_OrderConduite_Pion_ChangerDePosition.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_Pion_ChangerDePosition::nDIAOrderConduitePionChangerDePositionIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ChangerDePosition::InitializeDIA
// Created: AGR 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Pion_ChangerDePosition::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduitePionChangerDePositionIdx_ = DEC_Tools::InitializeDIAField( "orderConduitePionChangerDePosition_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ChangerDePosition constructor
// Created: AGR 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_ChangerDePosition::MIL_OrderConduite_Pion_ChangerDePosition( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ChangerDePosition destructor
// Created: AGR 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_ChangerDePosition::~MIL_OrderConduite_Pion_ChangerDePosition()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ChangerDePosition::Initialize
// Created: AGR 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Pion_ChangerDePosition::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Pion_ChangerDePosition& asnMission = *asnMsg.u.order_conduite_pion_changer_de_position;
    if( !NET_ASN_Tools::CopyPoint( asnMission, orderConduitePionChangerDePosition_, GetVariable( nDIAOrderConduitePionChangerDePositionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ChangerDePosition::Initialize
// Created: AGR 2005-08-01 - 11:23:53
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Pion_ChangerDePosition::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyPoint( diaParams[nCurDIAParamIdx++], orderConduitePionChangerDePosition_, GetVariable( nDIAOrderConduitePionChangerDePositionIdx_));

}
