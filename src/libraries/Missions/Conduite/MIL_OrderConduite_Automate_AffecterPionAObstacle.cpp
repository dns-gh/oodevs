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
#include "MIL_OrderConduite_Automate_AffecterPionAObstacle.h"

#include "MIL/Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_OrderConduite_Automate_AffecterPionAObstacle::nDIAPionIdx_ = 0 ;
int MIL_OrderConduite_Automate_AffecterPionAObstacle::nDIAObstacleIdx_ = 0 ;



//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_AffecterPionAObstacle::InitializeDIA
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_Automate_AffecterPionAObstacle::InitializeDIA( const MIL_OrderConduiteType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );
    nDIAPionIdx_ = DEC_Tools::InitializeDIAField( "pion_", diaType );
    nDIAObstacleIdx_ = DEC_Tools::InitializeDIAField( "obstacle_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_AffecterPionAObstacle constructor
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_AffecterPionAObstacle::MIL_OrderConduite_Automate_AffecterPionAObstacle( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : MIL_OrderConduite_ABC( knowledgeGroup, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_AffecterPionAObstacle destructor
// Created: AGR 2005-08-19 - 11:30:56
//-----------------------------------------------------------------------------
MIL_OrderConduite_Automate_AffecterPionAObstacle::~MIL_OrderConduite_Automate_AffecterPionAObstacle()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_AffecterPionAObstacle::Initialize
// Created: AGR 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_Automate_AffecterPionAObstacle::Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_OrderConduite_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode; 

    const ASN1T_OrderConduite_Automate_AffecterPionAObstacle& asnMission = *asnMsg.u.order_conduite_automate_affecter_pion_a_obstacle;
    if( !NET_ASN_Tools::CopyAgent( asnMission.pion, GetVariable( nDIAPionIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyGenObject( asnMission.obstacle, GetVariable( nDIAObstacleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_Automate_AffecterPionAObstacle::Initialize
// Created: AGR 2005-08-19 - 11:30:56
// -----------------------------------------------------------------------------
void MIL_OrderConduite_Automate_AffecterPionAObstacle::Initialize( DIA_Parameters& /*diaParams*/, uint /*nCurDIAParamIdx*/ )
{
    assert( false );
}
