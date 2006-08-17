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
#include "MIL_OrderConduite_ChangerPositionDebarquement.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_ChangerPositionDebarquement::nDIAOrderConduiteChangerPositionDebarquementIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ChangerPositionDebarquement::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteChangerPositionDebarquementIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteChangerPositionDebarquement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_ChangerPositionDebarquement::MIL_OrderConduite_ChangerPositionDebarquement( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement destructor
// Created: AGR 
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
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ChangerPositionDebarquement::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_ChangerPositionDebarquement& asnMission = *asnMsg.u.order_conduite_changer_position_debarquement;
    if( !NET_ASN_Tools::CopyPoint( asnMission, GetVariable( nDIAOrderConduiteChangerPositionDebarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ChangerPositionDebarquement::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_ChangerPositionDebarquement::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyPoint( diaParams[nCurDIAParamIdx++], GetVariable( nDIAOrderConduiteChangerPositionDebarquementIdx_));

}
