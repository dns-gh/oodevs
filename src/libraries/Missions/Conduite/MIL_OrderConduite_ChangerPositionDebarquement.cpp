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
#include "MIL_OrderConduite_ChangerPositionDebarquement.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_ChangerPositionDebarquement::nDIAOrderConduiteChangerPositionDebarquementIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement::InitializeDIA
// Created: AGR 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ChangerPositionDebarquement::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteChangerPositionDebarquementIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteChangerPositionDebarquement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement constructor
// Created: AGR 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_OrderConduite_ChangerPositionDebarquement::MIL_OrderConduite_ChangerPositionDebarquement( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement destructor
// Created: AGR 2005-08-17 - 16:30:44
//-----------------------------------------------------------------------------
MIL_OrderConduite_ChangerPositionDebarquement::~MIL_OrderConduite_ChangerPositionDebarquement()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement::Initialize
// Created: AGR 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ChangerPositionDebarquement::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_ChangerPositionDebarquement& asnMission = *asnMsg.u.order_conduite_changer_position_debarquement;
    if( !NET_ASN_Tools::CopyPoint( asnMission, orderConduiteChangerPositionDebarquement_, GetVariable( nDIAOrderConduiteChangerPositionDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement::Initialize
// Created: AGR 2005-08-17 - 16:30:44
// -----------------------------------------------------------------------------
void MIL_OrderConduite_ChangerPositionDebarquement::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyPoint( diaParams[nCurDIAParamIdx++], orderConduiteChangerPositionDebarquement_, GetVariable( nDIAOrderConduiteChangerPositionDebarquementIdx_));

}
