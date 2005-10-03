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

#include "MIL_AutomateMission_INF_Reduire.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_Reduire::nDIAPointsResistanceIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Reduire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Reduire::MIL_AutomateMission_INF_Reduire( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Reduire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Reduire::~MIL_AutomateMission_INF_Reduire()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Reduire::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_Reduire::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPointsResistanceIdx_ = DEC_Tools::InitializeDIAField( "pointsResistance_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Reduire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_Reduire::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_Reduire& asnMission = *asnMsg.mission.u.mission_automate_inf_reduire;
    if( !NET_ASN_Tools::CopyPointList( asnMission.points_resistance, pointsResistance_, GetVariable( nDIAPointsResistanceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Reduire::Terminate()
{
    NET_ASN_Tools::ResetPointList( pointsResistance_, GetVariable( nDIAPointsResistanceIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Reduire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Reduire::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_Reduire& asnMission = *new ASN1T_Mission_Automate_INF_Reduire();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_reduire;
    asnMsg.mission.u.mission_automate_inf_reduire  = &asnMission;

    NET_ASN_Tools::CopyPointList( GetVariable( nDIAPointsResistanceIdx_ ), asnMission.points_resistance );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Reduire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Reduire::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_reduire );
    ASN1T_Mission_Automate_INF_Reduire& asnMission = *asnMsg.mission.u.mission_automate_inf_reduire;

    NET_ASN_Tools::Delete( asnMission.points_resistance );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
