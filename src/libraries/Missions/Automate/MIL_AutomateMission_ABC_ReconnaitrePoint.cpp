// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_ReconnaitrePoint.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_ReconnaitrePoint::nDIAPointAReconnaitreIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReconnaitrePoint constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ReconnaitrePoint::MIL_AutomateMission_ABC_ReconnaitrePoint( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReconnaitrePoint destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_ReconnaitrePoint::~MIL_AutomateMission_ABC_ReconnaitrePoint()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReconnaitrePoint::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_ReconnaitrePoint::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointAReconnaitreIdx_ = DEC_Tools::InitializeDIAField( "pointAReconnaitre_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReconnaitrePoint::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_ReconnaitrePoint::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_ReconnaitrePoint& asnMission = *asnMsg.mission.u.mission_automate_abc_reconnaitre_point;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.point_a_reconnaitre, pointAReconnaitre_, GetVariable( nDIAPointAReconnaitreIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ReconnaitrePoint::Terminate()
{
    NET_ASN_Tools::ResetPolygon( pointAReconnaitre_, GetVariable( nDIAPointAReconnaitreIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReconnaitrePoint::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ReconnaitrePoint::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_ReconnaitrePoint& asnMission = *new ASN1T_Mission_Automate_ABC_ReconnaitrePoint();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_reconnaitre_point;
    asnMsg.mission.u.mission_automate_abc_reconnaitre_point  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAPointAReconnaitreIdx_ ), asnMission.point_a_reconnaitre );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_ReconnaitrePoint::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_ReconnaitrePoint::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_reconnaitre_point );
    ASN1T_Mission_Automate_ABC_ReconnaitrePoint& asnMission = *asnMsg.mission.u.mission_automate_abc_reconnaitre_point;

    NET_ASN_Tools::Delete( asnMission.point_a_reconnaitre );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
