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
#include "MIL_OrderConduite_Automate_ActiverObstacle.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_Automate_ActiverObstacle::nDIAOrderConduiteAutomateActiverObstacleIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ActiverObstacle::InitializeDIA
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Automate_ActiverObstacle::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAOrderConduiteAutomateActiverObstacleIdx_ = DEC_Tools::InitializeDIAField( "orderConduiteAutomateActiverObstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ActiverObstacle constructor
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_ActiverObstacle::MIL_OrderConduite_Automate_ActiverObstacle( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ActiverObstacle destructor
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_ActiverObstacle::~MIL_OrderConduite_Automate_ActiverObstacle()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ActiverObstacle::Initialize
// Created: AGR 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Automate_ActiverObstacle::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Automate_ActiverObstacle& asnMission = asnMsg.u.order_conduite_automate_activer_obstacle;
    if( !NET_ASN_Tools::CopyObjectKnowledge( asnMission, GetVariable( nDIAOrderConduiteAutomateActiverObstacleIdx_ ), knowledgeGroup_.GetKSQuerier() ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_ActiverObstacle::Initialize
// Created: AGR 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Automate_ActiverObstacle::Initialize( DIA_Parameters& /*diaParams*/, uint /*nCurDIAParamIdx*/ )
{
    assert( false );
}
