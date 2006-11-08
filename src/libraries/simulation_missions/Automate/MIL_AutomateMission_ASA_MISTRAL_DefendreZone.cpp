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

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_ASA_MISTRAL_DefendreZone.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ASA_MISTRAL_DefendreZone::nDIAZoneIdx_ = 0 ;
int MIL_AutomateMission_ASA_MISTRAL_DefendreZone::nDIAModeDeploiementIdx_ = 0 ;
int MIL_AutomateMission_ASA_MISTRAL_DefendreZone::nDIAAngleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_DefendreZone constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_MISTRAL_DefendreZone::MIL_AutomateMission_ASA_MISTRAL_DefendreZone( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_DefendreZone destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ASA_MISTRAL_DefendreZone::~MIL_AutomateMission_ASA_MISTRAL_DefendreZone()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_DefendreZone::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ASA_MISTRAL_DefendreZone::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneIdx_ = DEC_Tools::InitializeDIAField( "zone_", diaType );
    nDIAModeDeploiementIdx_ = DEC_Tools::InitializeDIAField( "modeDeploiement_", diaType );
    nDIAAngleIdx_ = DEC_Tools::InitializeDIAField( "angle_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_DefendreZone::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ASA_MISTRAL_DefendreZone::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& asnMission = *asnMsg.mission.u.mission_automate_asa_mistral_defendre_zone;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone, GetVariable( nDIAZoneIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyEnumeration( asnMission.mode_deploiement, GetVariable( nDIAModeDeploiementIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyNumeric( asnMission.angle, GetVariable( nDIAAngleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_MISTRAL_DefendreZone::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_DefendreZone::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_MISTRAL_DefendreZone::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& asnMission = *new ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_asa_mistral_defendre_zone;
    asnMsg.mission.u.mission_automate_asa_mistral_defendre_zone  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneIdx_ ), asnMission.zone );
    NET_ASN_Tools::CopyEnumeration( GetVariable( nDIAModeDeploiementIdx_ ), asnMission.mode_deploiement );
    NET_ASN_Tools::CopyNumeric( GetVariable( nDIAAngleIdx_ ), asnMission.angle );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ASA_MISTRAL_DefendreZone::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ASA_MISTRAL_DefendreZone::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_asa_mistral_defendre_zone );
    ASN1T_Mission_Automate_ASA_MISTRAL_DefendreZone& asnMission = *asnMsg.mission.u.mission_automate_asa_mistral_defendre_zone;

    NET_ASN_Tools::Delete( asnMission.zone );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
