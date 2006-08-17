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
#include "MIL_OrderConduite_Pion_ActiverObstacle.h"

#include "simulation_kernel/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Decision/DEC_Tools.h"




//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ActiverObstacle::InitializeDIA
// Created: AGR 
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Pion_ActiverObstacle::InitializeDIA( const MIL_OrderConduiteType& type )
{
    (void)DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ActiverObstacle constructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_ActiverObstacle::MIL_OrderConduite_Pion_ActiverObstacle( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup )
    : MIL_OrderConduite_ABC( type, pKnowledgeGroup )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ActiverObstacle destructor
// Created: AGR 
//-----------------------------------------------------------------------------
MIL_OrderConduite_Pion_ActiverObstacle::~MIL_OrderConduite_Pion_ActiverObstacle()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ActiverObstacle::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Pion_ActiverObstacle::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 


    return EnumOrderErrorCode::no_error;
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Pion_ActiverObstacle::Initialize
// Created: AGR 
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Pion_ActiverObstacle::Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx )
{
    MIL_OrderConduite_ABC::Initialize( diaParams, nCurDIAParamIdx );


}
