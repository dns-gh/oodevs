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
#include "MIL_OrderConduite_AssignerPositionEmbarquement.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_OrderConduite_AssignerPositionEmbarquement::nDIAOrderConduiteAssignerPositionEmbarquementIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AssignerPositionEmbarquement::InitializeDIA
// Created: AGR 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_AssignerPositionEmbarquement::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteAssignerPositionEmbarquementIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteAssignerPositionEmbarquement_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AssignerPositionEmbarquement constructor
// Created: AGR 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_OrderConduite_AssignerPositionEmbarquement::MIL_OrderConduite_AssignerPositionEmbarquement( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AssignerPositionEmbarquement destructor
// Created: AGR 2005-6-28 - 14:3:4
//-----------------------------------------------------------------------------
MIL_OrderConduite_AssignerPositionEmbarquement::~MIL_OrderConduite_AssignerPositionEmbarquement()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AssignerPositionEmbarquement::Initialize
// Created: AGR 2005-6-28 - 14:3:4
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_AssignerPositionEmbarquement::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_AssignerPositionEmbarquement& asnMission = *asnMsg.u.order_conduite_assigner_position_embarquement;
    if( !NET_ASN_Tools::CopyPoint( asnMission, orderConduiteAssignerPositionEmbarquement_, GetVariable( nDIAOrderConduiteAssignerPositionEmbarquementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_AssignerPositionEmbarquement::Initialize
// Created: AGR 2005-6-28 - 14:3:4
// -----------------------------------------------------------------------------
void MIL_OrderConduite_AssignerPositionEmbarquement::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );

    NET_ASN_Tools::CopyPoint( diaParams[nCurDIAParamIdx++], orderConduiteAssignerPositionEmbarquement_, GetVariable( nDIAOrderConduiteAssignerPositionEmbarquementIdx_));

}
