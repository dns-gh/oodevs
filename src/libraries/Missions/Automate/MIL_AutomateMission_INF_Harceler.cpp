// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_INF_Harceler.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_INF_Harceler::nDIAZoneSurveillanceIdx_ = 0 ;
int MIL_AutomateMission_INF_Harceler::nDIAPointRegroupementIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Harceler constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Harceler::MIL_AutomateMission_INF_Harceler( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Harceler destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_INF_Harceler::~MIL_AutomateMission_INF_Harceler()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Harceler::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_INF_Harceler::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneSurveillanceIdx_ = DEC_Tools::InitializeDIAField( "zoneSurveillance_", diaType );
    nDIAPointRegroupementIdx_ = DEC_Tools::InitializeDIAField( "pointRegroupement_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Harceler::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_INF_Harceler::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_INF_Harceler& asnMission = *asnMsg.mission.u.mission_automate_inf_harceler;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_surveillance, zoneSurveillance_, GetVariable( nDIAZoneSurveillanceIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.point_regroupement, pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Harceler::Terminate()
{
    NET_ASN_Tools::ResetPolygon( pointRegroupement_, GetVariable( nDIAPointRegroupementIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Harceler::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Harceler::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_INF_Harceler& asnMission = *new ASN1T_Mission_Automate_INF_Harceler();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_inf_harceler;
    asnMsg.mission.u.mission_automate_inf_harceler  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneSurveillanceIdx_ ), asnMission.zone_surveillance );
    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAPointRegroupementIdx_ ), asnMission.point_regroupement );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_INF_Harceler::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_INF_Harceler::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_inf_harceler );
    ASN1T_Mission_Automate_INF_Harceler& asnMission = *asnMsg.mission.u.mission_automate_inf_harceler;

    NET_ASN_Tools::Delete( asnMission.zone_surveillance );
    NET_ASN_Tools::Delete( asnMission.point_regroupement );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
